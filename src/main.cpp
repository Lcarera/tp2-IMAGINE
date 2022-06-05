#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include "main.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <sstream>
#define ONE_OVER_BILLION 1E-9


using namespace std;

// El siguiente es un template basico que pueden usar como base

int main(int argc , char* argv[]){
	
	// Asumimos que los filtros sin p1 se escriben primero (por lo tanto, el primer p1 es no nulo)
	// Asumimos que Zoom no se puede encadenar

	if(string(argv[1]) == "-help"){
		cout << "Uso: ./main <filtro> <nthreads> <[p1]> <img1> <custom_output> <[p2]> <img2>" << endl;
		return 0; 
	}
	if(string(argv[1]) == "-filtros"){
		cout << "shades" << endl;
		cout << "merge" << endl;
		cout << "brightness" << endl;
		cout << "boxblur" << endl;
		cout << "crop" << endl;
		cout << "zoom" << endl;
	}
	
	string filter = string(argv[1]);
	unsigned int n = atoi(argv[2]);
	string p1 = string(argv[3]);
	string img1(argv[4]);
	string out = string(argv[5]);
	string p2 = string(argv[6]);
	string img2(argv[7]);
	vector<string> filtros = separarDatos(filter);
	vector<string> listaPUno = separarDatos(p1);
	vector<string> listaPDos = separarDatos(p2);

	ppm imagen1(img1);
	ppm imagen2 = ppm();
	if (img2.length() > 4) 
	{
		ppm imagen2(img2);
		cout << "Se ingresaron dos imagenes"<< endl;
	}
	else{cout << "Se ingreso una imagen"<< endl;}
		
	cout << "Aplicando filtros"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);
	aplicarFiltros(filtros, n, listaPUno, listaPDos, imagen1, imagen2);
   	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf("%lf s\n", accum);

	cout << "Escribiendo imagen" << endl;
	imagen1.write(out);	
	    
	cout << "Listo" << endl;
	return 0;
}

vector<string> separarDatos(string datos)
{
	//Recibe un string con multiples datos y los separa e ingresa a una lista
	vector<string> datosSeparados;
	const char delim = ' ';
    stringstream ss(datos);
    string s;
    while (std::getline(ss, s, delim)) {
        datosSeparados.push_back(s);
    }

	return datosSeparados;
}

void aplicarFiltros(vector<string> filtros, unsigned int nThreads, vector<string> p1, vector<string> p2, ppm& primeraImagen, ppm& segundaImagen)
{
	for(unsigned int i = 0; i < filtros.size(); i++)
	{	
		filasPorThread(filtros[i], nThreads,  stof(p1[i]) , stof(p2[i]), primeraImagen, segundaImagen);
	}
}

void filasPorThread(string filtro, unsigned int nThreads,  float p1, float p2, ppm& primeraImagen, ppm& segundaImagen)
{
	unsigned int filasThread = (int)(primeraImagen.height/nThreads);
	unsigned int filasExtra = primeraImagen.height - (filasThread * nThreads);

	vector<thread> threads;
	ppm imagenNueva = ppm();
	if (filtro == "boxblur")
		imagenNueva = ppm(primeraImagen.width - 2, primeraImagen.height - 2);
	if (filtro == "crop")
		imagenNueva = ppm(primeraImagen.width - p1, primeraImagen.height - p2);
	if (filtro == "zoom")
		imagenNueva = ppm(primeraImagen.width * p1, primeraImagen.height * p1);

	for (unsigned int i = 0; i < nThreads; i++)
	{
		unsigned int comienzoAltura = i * filasThread;
		unsigned int finalAltura = (i + 1) * filasThread;

		if (i == nThreads - 1)
		{
			finalAltura += filasExtra;
		}

		if (filtro == "plain")
			threads.push_back(thread(plain, ref(primeraImagen), p1, comienzoAltura, finalAltura));
		if (filtro == "shades")
			threads.push_back(thread(shades, ref(primeraImagen), p1, comienzoAltura, finalAltura));
		if (filtro == "merge")
			threads.push_back(thread(merge, ref(primeraImagen), ref(segundaImagen), p1, comienzoAltura, finalAltura));
		if (filtro == "brightness")
			threads.push_back(thread(brightness, ref(primeraImagen), p1, comienzoAltura, finalAltura));
		if (filtro == "crop")
			threads.push_back(thread(crop, ref(primeraImagen), ref(imagenNueva), p1, p2, comienzoAltura, finalAltura));
		if (filtro == "boxblur")
			threads.push_back(thread(boxBlur, ref(primeraImagen), ref(imagenNueva), comienzoAltura, finalAltura));
		if (filtro == "zoom")
			threads.push_back(thread(zoom, ref(primeraImagen), ref(imagenNueva), p1, comienzoAltura, finalAltura));

		cout << "Thread: " << i << " Start: " << comienzoAltura << " End: " << finalAltura << endl;
	}
	for (unsigned int i = 0; i < nThreads; i++)
	{
		threads[i].join();
	}
	if (filtro == "boxblur" || filtro == "crop" ||  filtro == "zoom")
		primeraImagen = imagenNueva;
}