#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

void brightness(ppm& img, float brillo, unsigned int comienzoAltura, unsigned int finAltura);
void shades(ppm& img, float shades, unsigned int comienzoAltura, unsigned int finAltura);
void merge(ppm& img1, ppm& img2, float alpha, unsigned int comienzoAltura, unsigned int finAltura);
void boxBlur(ppm &img, ppm& nuevaImg, unsigned int comienzoAltura, unsigned int finAltura);
void crop(ppm& img, ppm& nuevaImg, int recortes, unsigned int comienzoAltura, unsigned int finAltura);
void zoom(ppm &img, ppm& nuevaImg, int n, unsigned int comienzoAltura, unsigned int finAltura);
void plain(ppm &img, float c, unsigned int comienzoAltura, unsigned int finAltura);

// MULTI-THREAD FILTERS

#endif