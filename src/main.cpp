#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
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
	float p1 = atof(argv[3]);
	string img1(argv[4]);
	string out = string(argv[5]);
	float p2 = atof(argv[6]);
	string img2(argv[7]);

	ppm imagen1(img1);
	ppm imagen2 = ppm();
	if (img2.length() > 4) 
	{
		ppm imagen2(img2);
	}	
		
	cout << "Aplicando filtros"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);

	if (filter == "plain")
		plain(imagen1, (unsigned char)p1);
	if (filter == "shades")
		shades(imagen1, (unsigned char)p1);
	if (filter == "merge")
		merge(imagen1, imagen2, p1);
	if (filter == "brightness")
		brightness(imagen1, p1);
	if (filter == "crop")
		crop(imagen1, p1, p2);
	if (filter == "boxblur")
		boxBlur(imagen1);
	if (filter == "zoom")
		zoom(imagen1, p1);
   	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf("%lf s\n", accum);

	cout << "Escribiendo imagen" << endl;
	imagen1.write(out);	
	    
	cout << "Listo" << endl;
	return 0;
}