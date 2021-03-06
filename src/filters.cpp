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


void plain(ppm& img, float c, unsigned int comienzoAltura, unsigned int finAltura)
{

	for(unsigned int i = comienzoAltura; i < finAltura; i++)
		for(unsigned int j = 0; j < img.width; j++)			
			img.setPixel(i, j, pixel(c,c,c));

}

void shades(ppm& img, float shades, unsigned int comienzoAltura, unsigned int finAltura)
{
	float g;
	float gPrima;
	float rango = (255/(int(shades)-1));
	pixel nuevoPixel = pixel();
	for(unsigned int i = comienzoAltura; i < finAltura; i++){
		for(unsigned int j = 0; j < img.width; j++){
			gPrima = (img.getPixel(i,j).cumsum() )/ 3;
			g = (gPrima/rango)*rango;
			nuevoPixel.r = g;
			nuevoPixel.g = g;
			nuevoPixel.b = g;
			img.setPixel(i, j, nuevoPixel.truncate());
		}				
	}
}

void merge(ppm& img1, ppm& img2, float alpha, unsigned int comienzoAltura, unsigned int finAltura)
{
	pixel pixelUno;
	pixel pixelDos; 
	try
	{
		float alpha2 = 1 - alpha;
		if (alpha > 1 || alpha < 0)
		{
			throw "El porcentaje debe estar entre 0 y 1";
		}
		
		for(unsigned int i = comienzoAltura; i < finAltura; i++)
		{	
			for(unsigned int j = 0; j < img1.width; j++)
			{
				pixelUno = img1.getPixel(i, j).mult(alpha);
				pixelDos = img2.getPixel(i,j).mult(alpha2);
				pixelUno.addp(pixelDos);
				img1.setPixel(i,j, pixelUno.truncate());
			}
		}
	}
	catch(string error)
	{
		cout << error << endl;
	}
			
			
}

void brightness(ppm& img, float brillo, unsigned int comienzoAltura, unsigned int finAltura)
{
	pixel pixelUno;
	try
	{
		if (brillo > 1 || brillo < -1)
		{
			throw "El brillo debe estar entre -1 y 1";
		}

		for(unsigned int i = comienzoAltura; i < finAltura; i++)
		{
			for(unsigned int j = 0; j < img.width; j++)
			{
				pixelUno = img.getPixel(i, j);
				pixelUno.add(255 * brillo);
				img.setPixel(i,j, pixelUno.truncate());
				
			}
		}
	}
	catch(char error)
	{
		cout << error << endl;
	}
	
}

void crop(ppm& img, ppm& nuevaImg, int recorte, unsigned int comienzoAltura, unsigned int finAltura)
{ 	
	pixel nuevoPixel;	
	for(unsigned int i = recorte; i < finAltura; i++)
	{
		for(unsigned int j = recorte; j < img.width; j++)
		{
			nuevoPixel = img.getPixel(i,j);
			nuevaImg.setPixel(i - recorte,j - recorte, nuevoPixel.truncate());				
		}
	}

}

void boxBlur(ppm &img, ppm& nuevaImg, unsigned int comienzoAltura, unsigned int finAltura) 
{
	pixel resultado;
	for (unsigned int i = 1; i < finAltura - 1; i++)
	{
		for (unsigned int j = 1; j < img.width - 1; j++)
		{
			resultado = pixel();
			resultado.addp(img.getPixel(i - 1, j - 1));
			resultado.addp(img.getPixel(i - 1, j));
			resultado.addp(img.getPixel(i - 1, j + 1));
			resultado.addp(img.getPixel(i, j - 1));
			resultado.addp(img.getPixel(i, j));
			resultado.addp(img.getPixel(i, j + 1));
			resultado.addp(img.getPixel(i + 1, j - 1));
			resultado.addp(img.getPixel(i + 1, j));
			resultado.addp(img.getPixel(i + 1, j + 1));
			resultado.r = resultado.r / 9;
			resultado.g = resultado.g / 9;
			resultado.b = resultado.b / 9;

			nuevaImg.setPixel(i - 1, j - 1, resultado.truncate());
		}
	}
}

void zoom(ppm &img, ppm& nuevaImg, int n, unsigned int comienzoAltura, unsigned int finAltura)
{
	pixel pixelNuevo;
	for (unsigned int i = comienzoAltura; i < finAltura; i++)
	{
		for (unsigned int j = 0; j < img.width; j++)
		{
			pixelNuevo = img.getPixel(i, j);

			unsigned int resultadoAltura = (j + (j * (n - 1)));
			unsigned int resultadoAncho = (i + (i * (n - 1)));
			for (unsigned int arregloAltura = 0; arregloAltura < n; arregloAltura++)
			{
				for (unsigned int arregloAncho = 0; arregloAncho < n; arregloAncho++)
				{
					nuevaImg.setPixel(resultadoAncho + arregloAltura, resultadoAltura + arregloAncho, pixelNuevo);
				}
			}
		}
	}
}