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

void merge(ppm& img1, ppm& img2, float alpha)
{
	
	int r;
	int g;
	int b;
	try
	{
		float alpha2 = 1 - alpha;
		if (alpha > 1 || alpha < 0)
		{
			throw "El porcentaje debe estar entre 0 y 1";
		}
		
		for(int i = 0; i < img1.height; i++)
		{	
		
			for(int j = 0; j < img1.width; j++)
			{
				img1.getPixel(i, j);
				img2.getPixel(i,j);

				r = truncate_pixel(img1.getPixel(i,j).r * alpha + img2.getPixel(i,j).r * alpha2);
				g = truncate_pixel(img1.getPixel(i,j).g * alpha + img2.getPixel(i,j).g * alpha2);
				b = truncate_pixel(img1.getPixel(i,j).b * alpha + img2.getPixel(i,j).b * alpha2);
				img1.setPixel(i,j, pixel(r,g,b));
				
			}
		}
	}
	catch(string error)
	{
		cout << error << endl;
	}
			
			
}

void brightness(ppm& img, float brillo, int start, int end)
{
	int r;
	int g;
	int b;

	try
	{
		if (brillo > 1 || brillo < -1)
		{
			throw "El brillo debe estar entre -1 y 1";
		}

		for(int i = 0; i < img.height; i++)
		{
			for(int j = 0; j < img.width; j++)
			{
				img.getPixel(i, j);

				r = truncate_pixel(img.getPixel(i,j).r + 255 * brillo);
				g = truncate_pixel(img.getPixel(i,j).g + 255 * brillo);
				b = truncate_pixel(img.getPixel(i,j).b + 255 * brillo);
				img.setPixel(i,j, pixel(r,g,b));
				
			}
		}
	}
	catch(char error)
	{
		cout << error << endl;
	}
	
}