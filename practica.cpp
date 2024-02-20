#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main() {
    // Ruta del archivo de entrada y salida
    std::string archivo_entrada = "C:/Users/josue/Desktop/xd.txt";
    std::string archivo_salida = "C:/Users/josue/Desktop/numeros.txt";

    // Abre el archivo de entrada
    std::ifstream archivo_in(archivo_entrada);
    // Verifica si se pudo abrir el archivo correctamente
    if (!archivo_in.is_open()) {
        std::cerr << "Error al abrir el archivo de entrada." << std::endl;
        return 1;
    }

    // Abre el archivo de salida
    std::ofstream archivo_out(archivo_salida);
    // Verifica si se pudo abrir el archivo correctamente
    if (!archivo_out.is_open()) {
        std::cerr << "Error al abrir el archivo de salida." << std::endl;
        archivo_in.close(); // Cierra el archivo de entrada antes de salir
        return 1;
    }

    // Lee el archivo de entrada línea por línea
    std::string linea;
    while (std::getline(archivo_in, linea)) {
        // Utiliza un stringstream para dividir la línea en tokens separados por comas
        std::stringstream ss(linea);
        std::string token;
        while (std::getline(ss, token, ',')) {
            // Elimina los espacios en blanco al inicio y al final del token
            token.erase(0, token.find_first_not_of(" \t\n\r\f\v"));
            token.erase(token.find_last_not_of(" \t\n\r\f\v") + 1);
            // Escribe cada token en una nueva línea en el archivo de salida
            archivo_out << token << std::endl;
        }
    }

    // Cierra los archivos
    archivo_in.close();
    archivo_out.close();

    std::cout << "Datos separados correctamente." << std::endl;

    return 0;
}