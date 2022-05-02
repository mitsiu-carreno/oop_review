#include <iostream>
#include <string>

/*
  Clase de objetos Cuenta
  Atributos:
    nombre, cuenta, saldo y tipo de interés
  Métodos:
    asignar/obtener nombre
    asignar/obtener cuenta
    obtener saldo
    ingreso
    reintegro
    asingar/obtener tipo de interés
*/
class Cuenta{
  private: std::string nombre;
  private: std::string cuenta;
  private: double saldo;
  private: double tipo_de_interes;

  public: void AsignarNombre(std::string nom){
    if(nom.length() == 0){
      std::cout << "Error: cadena vacía\n";
      return;
    }
    nombre = nom;
  }


  public: double ObtenerSaldo(){
    return saldo;
  }

  
};

int main(){

  // Crear un objeto de la clase Cuenta
  Cuenta cuenta01;
  // Asignar a su atributo nombre el valor "Rinzler"
  cuenta01.AsignarNombre("Rinzler");
  return 0;
}
