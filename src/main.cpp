#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include "helper.h"
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
		cout << "Uso: ./main <filtro> <nthreads> <[p1]> <img1> <custom_output> <img2>" << endl;
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
	string img2(argv[6]);
	vector<string> filtros = separarDatos(filter);
	vector<string> listaPUno = separarDatos(p1);
	ppm imagen1(img1);
	ppm imagen2(img2);
		
	cout << "Aplicando filtros"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);
	aplicarFiltros(filtros, n, listaPUno, imagen1, imagen2);
   	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf("%lf s\n", accum);

	cout << "Escribiendo imagen" << endl;
	imagen1.write(out);	
	    
	cout << "Listo" << endl;

	ofstream file;
	file.open("../pruebas/resultados.csv", ios::app);
	string outPrueba = to_string(imagen1.width) + "," + to_string(accum) + "," + to_string(n) + "\n";
	cout << "Printeando esto: " << outPrueba;
	file << outPrueba;
	file.close();
	return 0;
}

