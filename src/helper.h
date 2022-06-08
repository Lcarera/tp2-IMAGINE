#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


vector<string> separarDatos(string datos);
void aplicarFiltros(vector<string> filtros, unsigned int nthreads, vector<string> p1, ppm& img1, ppm& img2);
void filasPorThread(string filtro, unsigned int nThreads,  float p1, ppm& primeraImagen, ppm& segundaImagen);

#endif