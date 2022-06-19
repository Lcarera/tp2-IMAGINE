#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "../filters.h"
#include "../helper.h"
#include "loader.h"
#include <thread>
#include <unistd.h>
#include <fstream> // std::ofstream
#include <sstream>
#include <dirent.h>

#define ONE_OVER_BILLION 1E-9

using namespace std;
int main(int argc, char *argv[])
{

    if (string(argv[1]) == "-help")
    {
        cout << "Uso: ./loader <filtro> <nthreads> <[p1]> <dir> <out_dir> <img2>" << endl;
        return 0;
    }
    if (string(argv[1]) == "-filtros")
    {
        cout << "shades" << endl;
        cout << "brightness" << endl;
        cout << "boxblur" << endl;
        cout << "crop" << endl;
        cout << "zoom" << endl;
    }
    string filter = string(argv[1]);
    unsigned int n = atoi(argv[2]);
    string p1 = string(argv[3]);
    string folder(argv[4]);
    string out_dir = string(argv[5]);
    string img2(argv[6]);
    vector<string> filtros = separarDatos(filter);
    vector<string> listaPUno = separarDatos(p1);

    ppm imagen2 = ppm();
    if (img2.length() > 4)
    {
        ppm imagen2(img2);
    }

    cout << "Aplicando filtros" << endl;

    DIR *dir;
    struct dirent *diread;
    vector<ppm> files;
    vector<string> filesNombres;
    if ((dir = opendir(folder.c_str())) != nullptr)
    {
        while ((diread = readdir(dir)) != nullptr)
        {
            string file = diread->d_name;
            string esPpm;
            esPpm = file.length() > 4 ? file.substr(file.length() - 4, 4) : "";
            if (esPpm == ".ppm")
            {
                string ruta = folder + file;
                ppm nuevaFoto(ruta);
                files.push_back(nuevaFoto);
                filesNombres.push_back(file);
            }
        }
        closedir(dir);
    }
    else
    {
        perror("opendir");
        return EXIT_FAILURE;
    }
    struct timespec start, stop;
    clock_gettime(CLOCK_REALTIME, &start);
    int widthTotal = 0;
    fotosPorThread(files, n, folder, filtros, listaPUno, imagen2, widthTotal, out_dir, filesNombres);

    clock_gettime(CLOCK_REALTIME, &stop);
    double accum;
    accum = (stop.tv_sec - start.tv_sec) + (stop.tv_nsec - start.tv_nsec) * ONE_OVER_BILLION;
    printf("%lf s\n", accum);
    ofstream file;
    file.open("pruebas/resultados.csv", ios::app);
    string outPrueba = to_string(widthTotal) + "," + to_string(accum) + "," + to_string(n) + "\n";
    cout << "Printeando esto: " << outPrueba;
    file << outPrueba;
    file.close();
    cout << "Listo" << endl;

    return 0;
}

void fotosPorThread(vector<ppm> &files, unsigned int nThreads, string folder, vector<string> filtros, vector<string> listaPUno, ppm &imagen2, int &widthTotal, string out_dir, vector<string> filesNombres)
{
    unsigned int fotosThread = (int)(files.size() / nThreads);
    unsigned int fotosExtra = files.size() - (fotosThread * nThreads);
    vector<thread> threads;

    for (unsigned int i = 0; i < nThreads; i++)
    {
        unsigned int comienzoFotos = i * fotosThread;
        unsigned int finalFotos = (i + 1) * fotosThread;

        if (i == nThreads - 1)
        {
            finalFotos += fotosExtra;
        }
        vector<ppm> fotos(finalFotos - comienzoFotos + 1);
        vector<string> nombresFotos(finalFotos - comienzoFotos + 1);
        auto startFotos = files.begin() + comienzoFotos;
        auto endFotos = files.begin() + finalFotos + 1;

        auto startNombres = filesNombres.begin() + comienzoFotos;
        auto endNombres = filesNombres.begin() + finalFotos + 1;
        copy(startFotos, endFotos, fotos.begin());
        copy(startNombres, endNombres, nombresFotos.begin());
        threads.push_back(thread(cargarFotos, ref(fotos), folder, filtros, listaPUno, nThreads, ref(imagen2), ref(widthTotal), out_dir, nombresFotos));
    }
    for (unsigned int i = 0; i < nThreads; i++)
    {
        threads[i].join();
    }
}

void cargarFotos(vector<ppm> &files, string folder, vector<string> filtros, vector<string> listaPUno, unsigned int nThreads, ppm &imagen2, int &widthTotal, string out_dir, vector<string> filesNombres)
{
    for (int x = 0; x < files.size(); x++)
    {
        string out;
        string filtrosUsados;
        aplicarFiltros(filtros, nThreads, listaPUno, files[x], imagen2);
        widthTotal += files[x].width;
        for (unsigned int i = 0; i < filtros.size(); i++)
            filtrosUsados = filtrosUsados + filtros[i] + "_";
        out = out_dir + filtrosUsados + filesNombres[x];
        cout << "Escribiendo imagen " << filesNombres[x] << endl;
        files[x].write(out);
    }
}