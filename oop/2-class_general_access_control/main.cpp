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
  private: 
    std::string nombre;
    std::string cuenta;
    double saldo;
    double tipo_de_interes;

  public: 
    void AsignarNombre(std::string nom){
      if(nom.length() == 0){
        std::cout << "Error: cadena vacía\n";
        return;
      }
      nombre = nom;
    }

    std::string ObtenerNombre(){
      return nombre;
    }

    void AsignarCuenta(std::string cuenta){
      if(cuenta.length() == 0){
        std::cout << "Error: cuenta no válida\n";
        return;
      }
      this->cuenta = cuenta;
    }

    std::string ObtenerCuenta(){
      return cuenta;
    }

    double ObtenerSaldo(){
      return saldo;
    }

    void Ingreso(double cantidad){
      if(cantidad < 0){
        std::cout << "Error: cantidad negativa\n";
        return;
      }
      saldo = saldo + cantidad;
    }

    void Reintegro(double cantidad){
      if(saldo - cantidad < 0){
        std::cout << "Error: no dispone de saldo\n";
        return;
      }
      saldo = saldo - cantidad;
    }

    void AsignarTipoDeInteres(double tipo){
      if(tipo < 0){
        std::cout << "Error: tipo de interés no válido\n";
        return;
      }
      tipo_de_interes = tipo;
    }

    double ObtenerTipoDeInteres(){
      return tipo_de_interes;
    }
};

int main(){

  // Crear un objeto de la clase Cuenta
  Cuenta cuenta01;

  // Asignar a su atributo nombre el valor "Rinzler"
  cuenta01.AsignarNombre("Rinzler");
  cuenta01.AsignarCuenta("Cuenta sencilla");
  cuenta01.AsignarTipoDeInteres(2.5);

  cuenta01.Ingreso(12000);
  cuenta01.Reintegro(3000);

  std::cout << cuenta01.ObtenerNombre() << std::endl;
  std::cout << cuenta01.ObtenerCuenta() << std::endl;
  std::cout << cuenta01.ObtenerSaldo() << std::endl;
  std::cout << cuenta01.ObtenerTipoDeInteres() << std::endl;

  return 0;
}
