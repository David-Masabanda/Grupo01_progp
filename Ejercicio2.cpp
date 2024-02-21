#include <iostream>
#include <vector>
#include <chrono>
#include <fmt/core.h>
#include <omp.h>
#include <fmt/ranges.h>
#include <fstream>

namespace ch = std::chrono;


std::vector<int> read_file() {
    std::fstream fs("C:/Users/josue/Desktop/Universidad/ProgParalela/Grupal_final/numeros3.txt", std::ios::in);
    std::string line;
    std::vector<int> ret;
    while (std::getline(fs, line)) {
        ret.push_back(std::stoi(line));
    }
    fs.close();
    return ret;
}

std::vector<int> scanSerial(const std::vector<int>& a) {
    std::vector<int> salida(a.size());

    // Inicializar el primer elemento del resultado con el primer elemento del input
    salida[0] = a[0];

    // Calcular la suma acumulativa
    for (int i = 1; i < a.size(); ++i) {
        salida[i] = salida[i - 1] + a[i];
    }
    return salida;
}


std::vector<int> scanParalelo(const std::vector<int>& a) {
    int n = a.size();
    std::vector<int> salida(n);

    // Inicializar el primer elemento del resultado con el primer elemento del input
    salida[0] = a[0];

    // Calcular la suma acumulativa (paralelo)
    #pragma omp parallel for
    for (int i = 1; i < n; ++i) {
        salida[i] = salida[i - 1] + a[i];
    }

    return salida;
}

int main() {
    std::vector<int> datos = {10, 20, 10, 5, 15,10,5,15};
    //std::vector<int> datos = read_file();

    fmt::println("VERSION SERIAL");
    auto start = ch::high_resolution_clock::now();

    std::vector<int> nuevo =scanSerial(datos);
    fmt::println("Entrada: {}", datos);
    fmt::println("Salida: {}", nuevo);

    auto end = ch::high_resolution_clock::now();
    ch::duration<double, std::milli> tiempo = end - start;
    fmt::print("Tiempo de proceso: {}\n", tiempo.count());
    fmt::println("-------------------------------------------------------");


    fmt::println("VERSION OPENMP");
    auto start1 = ch::high_resolution_clock::now();

    std::vector<int> nuevo1 =scanParalelo(datos);
    fmt::println("Entrada: {}", datos);
    fmt::println("Salida: {}", nuevo1);

    auto end1 = ch::high_resolution_clock::now();
    ch::duration<double, std::milli> tiempo1 = end1 - start1;
    fmt::print("Tiempo de proceso: {}\n", tiempo1.count());


    return 0;
}
