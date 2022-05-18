#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>  

#define BLACK 0

using namespace std;

// COMPLETAR :)
unsigned char truncate_pixel(float color)
{ 
    return (color > 255) ? 255 : ((color < 0) ? 0: (unsigned char)color); 
}
// Filtro plano como ejemplo

void plain(ppm& img, unsigned char c)
{

	for(int i = 0; i < img.height; i++)
		for(int j = 0; j < img.width; j++)			
			img.setPixel(i, j, pixel(c,c,c));

}

void shades(ppm& img, unsigned char shades)
{
	float g;
	float gPrima;
	float rango = (255/int(shades)-1);
	for(int i = 0; i < img.height; i++){
		for(int j = 0; j < img.width; j++){
			gPrima = (int)((img.getPixel(i,j).r + img.getPixel(i,j).g + img.getPixel(i,j).b )/ 3);
			g = truncate_pixel((gPrima/rango)*rango);
			img.setPixel(i, j, pixel(g,g,g));
		}				
	}
}