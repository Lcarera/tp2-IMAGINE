#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

void brightness(ppm& img, float brillo);
void shades(ppm& img, float shades);
void merge(ppm& img1, ppm& img2, float alpha);
void boxBlur(ppm &img);
void crop(ppm& img, int filas, int columnas);
void zoom(ppm &img, /* ppm &img_zoomed, */ int n);
void plain(ppm &img, float c);

// MULTI-THREAD FILTERS

#endif