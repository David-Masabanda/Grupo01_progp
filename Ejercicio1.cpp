#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <fmt/core.h>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <cmath>

#include <chrono>
namespace ch=std::chrono;

#define MAX_NUMEROS 2000000

#define GRUPOS 10

#define INTERVALO 30


std::vector<int> randomicos(){
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<> dist(0,100);
    std::vector<int> datos(MAX_NUMEROS);

    for (int i = 0; i < MAX_NUMEROS; ++i) {
        int x= dist(gen);
        datos[i]=x;
    }
    return datos;
}

std::vector<int> read_file() {
    std::fstream fs("C:/Users/josue/Desktop/Universidad/ProgParalela/Grupal_final/numeros/datos4.txt", std::ios::in);
    std::string line;
    std::vector<int> ret;
    while (std::getline(fs, line)) {
        ret.push_back(std::stoi(line));
    }
    fs.close();
    return ret;
}

// Versiones seriales
std::vector<int> frecuencias(const int *a, const int n){

    int block=std::ceil((double)255/GRUPOS);
    int contador=0;
    std::vector<int> datos (255);
    std::vector<int> datosFinal (GRUPOS);

    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i==a[j]){
                contador++;
            }
        }
        datos[i]=contador;
        contador=0;
    }

    int indice= 0;
    fmt::println("Grupo - Frecuencia");
    for (int i = 0; i < GRUPOS; ++i) {
        int suma = 0;
        for (int j = 0; j < block; ++j) {
            if (indice<256){
                suma += datos[indice++];
            }
        }
        datosFinal[i] = suma;
        fmt::println("[{}], [{}]", i, suma);
    }

    return datosFinal;
}

std::vector<int> frecuencias2(const int *a, const int n, int intervalo) {
    fmt::print("Intervalo - Frecuencia\n");
    std::vector<int> datos(256 / intervalo, 0);

    for (int i = 0; i < n; ++i) {
        int index = a[i] / intervalo; // Calcular el índice del intervalo
        datos[index]++;
    }

    for (int i = 0; i < datos.size(); ++i) {
        if (i == datos.size() - 1) {
            fmt::print("[{}-{}] - [{}]\n", i * intervalo, 255, datos[i]);
        } else {
            fmt::print("[{}-{}] - [{}]\n", i * intervalo, (i + 1) * intervalo - 1, datos[i]);
        }
    }

    return datos;
}

// Versiones en OpenMP
std::vector<int> frecuenciasOMP(const int *a, const int n){
    std::vector<int> datos (GRUPOS);
    int block=std::ceil((double)255/GRUPOS);

    #pragma omp parallel default(none) shared (a, n, datos, block)
    {
        std::vector<int> tmp_datos (GRUPOS);

        int start;
        int end;
        #pragma omp for

        for (int i = 0; i <GRUPOS ; ++i) {
            int tmp_contador=0;
            start=(i*block);
            end = (i == GRUPOS - 1) ? 256 : (start + block);

            for (int j = 0; j < n; ++j) {
                if (a[j]>=start && a[j]<end){
                    tmp_contador++;
                }
            }
            tmp_datos[i]=tmp_contador;
        }

        #pragma omp critical
        {
            for (int i = 0; i < GRUPOS; i++) {
                datos[i] +=  tmp_datos[i];
            }
        }
    }

    return datos;
}

std::vector<int> frecuenciasOMP2(const int *a, const int n, int intervalo) {
    fmt::print("Intervalo - Frecuencia\n");
    std::vector<int> datos(256 / intervalo, 0);

    #pragma omp parallel for default(none) shared (a, n, datos, intervalo)
    for (int i = 0; i < n; ++i) {
        int index = a[i] / intervalo; // Calcular el índice del intervalo

        #pragma omp atomic
        datos[index]++;
    }

    for (int i = 0; i < datos.size(); ++i) {
        if (i == datos.size() - 1) {
            fmt::print("[{}-{}] - [{}]\n", i * intervalo, 255, datos[i]);
        } else {
            fmt::print("[{}-{}] - [{}]\n", i * intervalo, (i + 1) * intervalo - 1, datos[i]);
        }
    }

    return datos;
}


std::vector<int> frecuenciasOMP3(const int *a, const int n){
    std::vector<int> datos (256);

    #pragma omp parallel default(none) shared (a, n, datos)
    {
        std::vector<int> tmp_datos (256);

        #pragma omp for
        for (int i = 0; i <256 ; ++i) {
            int tmp_contador=0;
            for (int j = 0; j < n; ++j) {
                if (i==a[j]){
                    tmp_contador++;
                }
            }
            tmp_datos[i]=tmp_contador;
        }

        #pragma omp critical
        {
            for (int i = 0; i < 256; i++) {
                datos[i] +=  tmp_datos[i];
            }
        }
    }

    return datos;
}

int main() {

    std::vector<int> datosRandom=read_file();
    fmt::println("Size vector: {}\n", datosRandom.size());

    //Version serial
    fmt::println("VERSION SERIAL");
    auto start = ch::high_resolution_clock::now();
    std::vector<int> frecuenciaSerial= frecuencias(datosRandom.data(), datosRandom.size());
    auto end = std::chrono::high_resolution_clock::now();
    ch::duration<double, std::milli> tiempo = end-start;
    fmt::println("Tiempo Serial: {}ms", tiempo.count());
    fmt::println("-------------------------------------------------------");


    //Version paralela
    fmt::println("VERSION OPENMP");
    auto start1 = ch::high_resolution_clock::now();
    std::vector<int> frecuenciaOMP= frecuenciasOMP(datosRandom.data(), datosRandom.size());
    auto end1 = std::chrono::high_resolution_clock::now();
    ch::duration<double, std::milli> tiempo1 = end1-start1;
    fmt::println("Tiempo Paralelo: {}ms", tiempo1.count());

    fmt::print("Grupo - Frecuencia\n");
    for (int i = 0; i < GRUPOS; ++i) {
        fmt::println("[{}] - [{}]", i, frecuenciaOMP[i]);
    }

    fmt::println("-------------------------------------------------------");
    //Version serial
    fmt::println("VERSION SERIAL 2");
    auto start2 = ch::high_resolution_clock::now();
    std::vector<int> frecuenciaSerial2 = frecuencias2(datosRandom.data(), datosRandom.size(), INTERVALO);
    auto end2 = std::chrono::high_resolution_clock::now();
    ch::duration<double, std::milli> tiempo2 = end2 - start2;
    fmt::println("Tiempo Serial: {}ms", tiempo.count());
    fmt::println("-------------------------------------------------------");

    //Version paralela
    fmt::println("VERSION PARALELA 2");
    auto start3 = ch::high_resolution_clock::now();
    std::vector<int> frecuenciaOMP2 = frecuenciasOMP2(datosRandom.data(), datosRandom.size(), INTERVALO);
    auto end3 = std::chrono::high_resolution_clock::now();
    ch::duration<double, std::milli> tiempo3 = end3 - start3;
    fmt::println("Tiempo Paralelo: {}ms", tiempo3.count());



    //GENERAR txt de datos
//    std::ofstream archivo("C:\\Users\\josue\\Desktop\\Universidad\\ProgParalela\\Grupal_final\\numeros4.txt");
//    for (int dato : datosRandom) {
//        archivo << dato << std::endl;
//    }
//
//    archivo.close();
//    std::cout << "Datos guardados en datos.txt" << std::endl;


    return 0;
}


