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

// Filtro plano como ejemplo

void plain(ppm& img, unsigned char c)
{

	for(unsigned int i = 0; i < img.height; i++)
		for(unsigned int j = 0; j < img.width; j++)			
			img.setPixel(i, j, pixel(c,c,c));

}

void shades(ppm& img, unsigned char shades)
{
	float g;
	float gPrima;
	float rango = (255/int(shades)-1);
	pixel nuevoPixel = pixel();
	for(unsigned int i = 0; i < img.height; i++){
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

void merge(ppm& img1, ppm& img2, float alpha)
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
		
		for(unsigned int i = 0; i < img1.height; i++)
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

void brightness(ppm& img, float brillo)
{
	pixel pixelUno;
	try
	{
		if (brillo > 1 || brillo < -1)
		{
			throw "El brillo debe estar entre -1 y 1";
		}

		for(unsigned int i = 0; i < img.height; i++)
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

void crop(ppm& img, int filas, int columnas)
{ 	
	pixel nuevoPixel;
	ppm nuevaImg = ppm(img.width - columnas, img.height - filas);
	for(int i = filas; i < img.height; i++)
	{
		for(int j = columnas; j < img.width; j++)
		{
			nuevoPixel = img.getPixel(i,j);
			nuevaImg.setPixel(i - filas,j - columnas, nuevoPixel.truncate());				
		}
	}

	img = nuevaImg;
}

void boxBlur(ppm &img) {
	ppm imagenNueva = ppm(img.width - 2, img.height - 2);
	pixel resultado;
	for (unsigned int i = 1; i < img.height - 1; i++)
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

			imagenNueva.setPixel(i - 1, j - 1, resultado.truncate());
		}
	}
	img = imagenNueva;
}

void zoom(ppm &img, /* ppm &img_zoomed, */ int n)
{
	pixel pixelNuevo;
    ppm imagenZoomeada(img.width * n, img.height * n);
	for (unsigned int i = 0; i < img.height; i++)
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
					imagenZoomeada.setPixel(resultadoAncho + arregloAltura, resultadoAltura + arregloAncho, pixelNuevo);
				}
			}
		}
	}
    img = imagenZoomeada;
}