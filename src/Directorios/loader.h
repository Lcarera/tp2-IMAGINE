#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "../ppm.h"
#include <atomic>

void fotosPorThread(vector<ppm> &files, unsigned int nThreads, string folder, vector<string> filtros, vector<string> listaPUno, ppm &imagen2, int &widthTotal, string out_dir, vector<string> filesNombres);
void cargarFotos(vector<ppm> &files, string folder, vector<string> filtros, vector<string> listaPUno, unsigned int nThreads, ppm &imagen2, int &widthTotal, string out_dir, vector<string> filesNombres);

#endif