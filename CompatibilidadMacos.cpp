#include <iostream>
#include "CompatibilidadMacos.h"

void limpiarBuffer() {
  std::cin.clear(); // Limpiar el estado del flujo
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignorar caracteres restantes
}

// Función para pausar la ejecución
void pausar() {
#ifdef _WIN32
  system("pause");  // Pausar la ejecución en Windows
#else
  std::cout << std::endl << "Presiona Enter para continuar..." << std::endl;
  system("read -r");  // Pausar la ejecución en macOS y Linux
#endif
}

void limpiar() {
#ifdef _WIN32
  std::system("cls");  // Limpiar la pantalla en Windows
#else
  std::system("clear");  // Limpiar la pantalla en macOS y Linux
#endif
}