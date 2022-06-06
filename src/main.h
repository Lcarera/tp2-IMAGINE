#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


vector<string> separarDatos(string datos);
void aplicarFiltros(vector<string> filtros, unsigned int nthreads, vector<string> p1, vector<string> p2, ppm& img1, ppm& img2);
void filasPorThread(string filtro, unsigned int nThreads,  float p1, float p2, ppm& primeraImagen, ppm& segundaImagen);

#endif