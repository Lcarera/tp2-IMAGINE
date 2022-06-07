#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "./filters.h"
#include "./helper.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <sstream>
#include <dirent.h>

#define ONE_OVER_BILLION 1E-9

using namespace std;
int main(int argc , char* argv[]) {

    if(string(argv[1]) == "-help"){
		cout << "Uso: ./main <filtro> <nthreads> <[p1]> <dir> <out_dir> <[p2]> <img2>" << endl;
		return 0; 
	}
	if(string(argv[1]) == "-filtros"){
		cout << "shades" << endl;
		cout << "brightness" << endl;
		cout << "boxblur" << endl;
		cout << "crop" << endl;
		cout << "zoom" << endl;
        cout << "merge" << endl;
	}
    string filter = string(argv[1]);
	unsigned int n = atoi(argv[2]);
	string p1 = string(argv[3]);
	string folder(argv[4]);
	string out_dir = string(argv[5]);
	string p2 = string(argv[6]);
    string img2(argv[7]);
	vector<string> filtros = separarDatos(filter);
	vector<string> listaPUno = separarDatos(p1);
	vector<string> listaPDos = separarDatos(p2);

    ppm imagen2 = ppm();
    if (img2.length() > 4) 
	{
		ppm imagen2(img2);
	}
	
		
	cout << "Aplicando filtros"<< endl;
	struct timespec start, stop;    	
	clock_gettime(CLOCK_REALTIME, &start);
    DIR *dir; struct dirent *diread;
    vector<string> files;
    if ((dir = opendir(folder.c_str())) != nullptr) {
        while ((diread = readdir(dir)) != nullptr) {
            files.push_back(diread->d_name);
        }
        closedir (dir);
    } else {
        perror ("opendir");
        return EXIT_FAILURE;
    }
    string out;
    for (auto file : files)
    {
        string esPpm;
        esPpm = file.length() > 4 ? file.substr(file.length() - 4, 4): "";
        if (esPpm == ".ppm") 
        {
            ppm imagen1(folder + file);
            aplicarFiltros(filtros, n, listaPUno, listaPDos, imagen1, imagen2);
            out = out_dir + filter + "_" + file ;
            cout << "Escribiendo imagen " << file << endl;
            imagen1.write(out);	
                
            cout << "Listo" << endl;
        }
    }
	
   	clock_gettime(CLOCK_REALTIME, &stop);

	double accum;
	accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf("%lf s\n", accum);

	
	return 0;
 
}