#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include "helper.h"
#include <thread>  
#include <atomic>  
#include <sstream>
#define BLACK 0

using namespace std;


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

void aplicarFiltros(vector<string> filtros, unsigned int nThreads, vector<string> p1, ppm& primeraImagen, ppm& segundaImagen)
{
	for(unsigned int i = 0; i < filtros.size(); i++)
	{	
		filasPorThread(filtros[i], nThreads,  stof(p1[i]), primeraImagen, segundaImagen);
	}
}

void filasPorThread(string filtro, unsigned int nThreads,  float p1, ppm& primeraImagen, ppm& segundaImagen)
{
	unsigned int filasThread = (int)(primeraImagen.height/nThreads);
	unsigned int filasExtra = primeraImagen.height - (filasThread * nThreads);
	vector<thread> threads;
	ppm imagenNueva = ppm();
	if (filtro == "boxblur")
		imagenNueva = ppm(primeraImagen.width - 2, primeraImagen.height - 2);
	if (filtro == "crop")
		imagenNueva = ppm(primeraImagen.width - p1, primeraImagen.height - p1);
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
			threads.push_back(thread(crop, ref(primeraImagen), ref(imagenNueva), p1, comienzoAltura, finalAltura));
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
	{
		primeraImagen = imagenNueva;
	}
}