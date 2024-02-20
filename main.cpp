#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <fmt/core.h>
#include <fstream>
#include <SFML/Graphics.hpp>


#include <chrono>
namespace ch=std::chrono;

#define MAX_NUMEROS 90000


std::vector<int> randomicos(){
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_int_distribution<> dist(0,255);
    std::vector<int> datos(MAX_NUMEROS);

    for (int i = 0; i < MAX_NUMEROS; ++i) {
        int x= dist(gen);
        datos[i]=x;
    }
    return datos;
}

std::vector<int> read_file() {
    std::fstream fs("C:/Users/josue/Desktop/Universidad/ProgParalela/Grupal_final/datos4.txt", std::ios::in);
    std::string line;
    std::vector<int> ret;
    while (std::getline(fs, line)) {
        ret.push_back(std::stoi(line));
    }
    fs.close();
    return ret;
}

std::vector<int> frecuencias(const int *a, const int n){
    fmt::print("Valor - Frecuencia\n");
    int contador=0;
    std::vector<int> datos (255);

    for (int i = 0; i < 256; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i==a[j]){
                contador++;
            }
        }
        datos[i]=contador;
        fmt::print("[{}] - [{}]\n",i,contador);
        contador=0;
    }
    return datos;
}

std::vector<int> frecuenciasOMP(const int *a, const int n){
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

void drawHistogram(const std::vector<int>& frequencies) {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Histograma");

    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error cargando la fuente Arial" << std::endl;
        return;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);

    std::vector<sf::RectangleShape> bars;

    for (int i = 0; i < 256; ++i) {
        sf::RectangleShape bar(sf::Vector2f(2, frequencies[i] * 0.1)); // Ajuste el factor multiplicador para cambiar la altura
        // Colores diferentes para cada barra
        bar.setFillColor(sf::Color(i, 100, 200)); // Ajusta los valores de color según tu preferencia
        bar.setPosition(i * 3, 600 - frequencies[i] * 0.1); // Ajuste el factor multiplicador aquí también
        bars.push_back(bar);
    }

    // Escala en el lado derecho del gráfico
    for (int i = 0; i <= 10; ++i) {
        sf::RectangleShape scale(sf::Vector2f(10, 2));
        scale.setFillColor(sf::Color::White);
        scale.setPosition(790, 600 - i * 50);
        window.draw(scale);

        sf::Text scaleText;
        scaleText.setFont(font);
        scaleText.setCharacterSize(15);
        scaleText.setFillColor(sf::Color::White);
        scaleText.setString(fmt::format("{}", i * 500));
        scaleText.setPosition(810, 600 - i * 50);
        window.draw(scaleText);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black);

        for (const auto& bar : bars) {
            window.draw(bar);
        }

        window.display();
    }
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
    fmt::println("VERSION PARALELA");
    auto start1 = ch::high_resolution_clock::now();
    std::vector<int> frecuenciaOMP= frecuenciasOMP(datosRandom.data(), datosRandom.size());
    auto end1 = std::chrono::high_resolution_clock::now();
    ch::duration<double, std::milli> tiempo1 = end1-start1;
    fmt::println("Tiempo Paralelo: {}ms", tiempo1.count());

    fmt::print("Valor - Frecuencia\n");
    for (int i = 0; i < 256; ++i) {
        fmt::println("[{}] - [{}]", i, frecuenciaOMP[i]);
    }

    drawHistogram(frecuenciaOMP);

    //GENERAR txt de datos
//    std::ofstream archivo("C:\\Users\\josue\\Desktop\\Universidad\\ProgParalela\\Grupal_final\\numeros13.txt");
//    for (int dato : datosRandom) {
//        archivo << dato << std::endl;
//    }
//
//    archivo.close();
//    std::cout << "Datos guardados en datos.txt" << std::endl;


    return 0;
}


