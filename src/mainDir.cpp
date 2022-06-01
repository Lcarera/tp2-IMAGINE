#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <unistd.h>
#include <fstream>      // std::ofstream
#include <dirent.h>

#define ONE_OVER_BILLION 1E-9

using namespace std;
int main(int argc , char* argv[]) {

    if(string(argv[1]) == "-help"){
		cout << "Uso: ./main <filtro> <nthreads> <[p1]> <dir> <out_dir> <[p2]> " << endl;
		return 0; 
	}
	if(string(argv[1]) == "-filtros"){
		cout << "shades" << endl;
		cout << "brightness" << endl;
		cout << "boxblur" << endl;
		cout << "crop" << endl;
		cout << "zoom" << endl;
	}
    string filter = string(argv[1]);
	unsigned int n = atoi(argv[2]);
	float p1 = atof(argv[3]);
	string folder(argv[4]);
    folder = folder.substr(folder.length() - 1, 1) != "/" ? folder + "/": folder;
	string out_dir = string(argv[5]);
	float p2 = atof(argv[6]);

	
		
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
            if (filter == "plain")
                plain(imagen1, (unsigned char)p1);
            if (filter == "shades")
                shades(imagen1, (unsigned char)p1);
            if (filter == "brightness")
                brightness(imagen1, p1);
            if (filter == "crop")
                crop(imagen1, p1, p2);
            if (filter == "boxblur")
                boxBlur(imagen1);
            if (filter == "zoom")
                zoom(imagen1, p1);
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