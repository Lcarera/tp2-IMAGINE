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
	aplicarFiltros(filtros, n, listaPUno, listaPDos, imagen1, imagen1);
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

void aplicarFiltros(vector<string> filtros, unsigned int nthreads, vector<string> p1, vector<string> p2, ppm& primeraImagen, ppm& segundaImagen)
{
	for(int i = 0; i < filtros.size(); i++)
	{
		
		float variable = stof(p1[i]);
		if (filtros[i] == "plain")
			plain(primeraImagen, variable, 0, primeraImagen.height + 1);
		if (filtros[i] == "shades")
			shades(primeraImagen, variable, 0, primeraImagen.height + 1);
		if (filtros[i] == "merge")
			merge(primeraImagen, segundaImagen, variable, 0, primeraImagen.height + 1);
		if (filtros[i] == "brightness")
			brightness(primeraImagen, variable, 0, primeraImagen.height + 1);
		if (filtros[i] == "crop")
			crop(primeraImagen, variable, stof(p2[i]), 0, primeraImagen.height + 1);
		if (filtros[i] == "boxblur")
			boxBlur(primeraImagen, 0, primeraImagen.height + 1);
		if (filtros[i] == "zoom")
			zoom(primeraImagen, variable, 0, primeraImagen.height + 1);
	}
}