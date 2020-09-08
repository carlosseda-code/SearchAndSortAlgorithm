//
//  main.cpp
//  Algoritmos fundamentales
//
//  Created by Carlos Seda and Rodrigo H on 05/09/20.
//  Copyright © 2020 Carlos Seda. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

// Clase donde tendremos almacenado cada linea del archivo
class Usuarios{
public:
    string linea;
    int id;
};
// Declaracion de funciones
void lecturaVariables(vector<Usuarios>& v);
Usuarios dividirLinea(string linea);
int crearId(string mes, int dia, string hora);
void merge(vector<Usuarios>& izquierda, vector<Usuarios>& derecha, vector<Usuarios>& v);
void ordenaMerge(vector<Usuarios>& v);
int binarySearch(vector<Usuarios>& v, int numBuscar);

// Menu principal
int main(int argc, const char * argv[]) {
    string mes, mes2, hora1 = "00:00:00", hora2 = "23:59:59";
    int dia,dia2, cont = 0;
    int idinicio, idfin;
    int numero = 1;
    ofstream f("registro.txt");
    vector<Usuarios> v;
    
    lecturaVariables(v);
    cout << "------------Registros de acceso------------" << endl;
    cout << "¿Desde que mes le gustaria ver el registro? (Junio, Julio, Agosto, Septiembre, Octubre)" << endl;
    while(cont == 0){
    cout << "Mes: ";
    cin >> mes;
    if (mes == "Junio" || mes == "Julio" || mes == "Agosto" || mes == "Septiembre" || mes == "Octubre")
        cont++;
    }
    cont = 0;
    cout << "Dia: ";
    cin >> dia;
    cout << "¿Y hasta que mes? (Junio, Julio, Agosto, Septiembre, Octubre)" << endl;
    while(cont == 0){
    cout << "Mes: ";
    cin >> mes2;
    if (mes2 == "Junio" || mes2 == "Julio" || mes2 == "Agosto" || mes2 == "Septiembre" || mes2 == "Octubre")
        cont++;
    }
    cout << "Dia: ";
    cin >> dia2;
    cout << "-------------Buscando archivos-------------" << endl;
    idinicio = crearId(mes, dia, hora1);
    idfin = crearId(mes2, dia2, hora2);
    if(idfin < idinicio){
        cout << "Mal ingreso de fechas" << endl;
        cout << "Saliendo del sistema...." << endl;
        return 1;
    }
    ordenaMerge(v);
    int posicionInicial = binarySearch(v, idinicio);
    int posicionFinal = binarySearch(v, idfin) - 1;
    for (int i = posicionInicial; i <= posicionFinal; i++){
        cout << "Registro #"<< numero << ": " << v[i].linea<< endl;
        if (f.is_open())
        f << v[i].linea << endl;
        else cerr << "Error de apertura del archivo." << endl;
        numero++;
    }
}

// Leer todo el archivo y asignar IDs a cada usuario
void lecturaVariables(vector<Usuarios>& v){
    vector<string> variables;
    string linea;
    ifstream archivo;
    Usuarios persona;
    archivo.open("bitacora.txt");
    while (getline(archivo, linea)){
        persona = dividirLinea(linea);
        v.push_back(persona);
    }
        archivo.close();
}

// Divir la linea del archivo y guardar en variables
Usuarios dividirLinea(string linea) {
  vector<string> variables;
  stringstream ss(linea);
  string dato;
  string mes, hora;
  Usuarios nuevo;
  int dia;
  char delimiter = ' ';
  while(getline(ss, dato, delimiter)) {
    variables.push_back(dato);
  }
    mes = variables[0];
    dia = stoi(variables[1]);
    hora = variables[2];
    nuevo.linea = linea;
    nuevo.id = crearId(mes, dia, hora);
    return nuevo;
}

// Crear el ID del usuario recibiendo los datos necesarios
int crearId(string mes, int dia, string hora){
    string id;
    vector<string> variables;
    stringstream ss(hora);
    string dato;
    char delimiter = ':';
    
    if (mes == "Jun" || mes == "Junio")
        id = id + "6";
    else if (mes == "Jul" || mes == "Julio")
        id = id + "7";
    else if (mes == "Aug" || mes == "Agosto")
    id = id + "8";
    else if (mes == "Sep" || mes == "Septiembre")
    id = id + "9";
    else if (mes == "Oct" || mes == "Octubre")
    id = id + "10";
    if (dia >= 10)
        id = id + to_string(dia);
    else id = id + "0" + to_string(dia);
    
    while(getline(ss, dato, delimiter))
      variables.push_back(dato);
    id = id + variables[0];
    id = id + variables[1];
    id = id + variables[2];
    return stoi(id);
}

// Funcion 2 de ordenamiento merge
void merge(vector<Usuarios>& izquierda, vector<Usuarios>& derecha, vector<Usuarios>& v) {
    int i = 0, j = 0, k = 0;
    while (i < izquierda.size() && j < derecha.size()) {
        if (izquierda[i].id <= derecha[j].id) {
            v[k] = izquierda[i];
            i++;
            k++;
        } else {
            v[k] = derecha[j];
            j++;
            k++;
        }
    }
    while (i < izquierda.size()) {
        v[k] = izquierda[i];
        k++;
        i++;
    }
    while (j < derecha.size()) {
        v[k] = derecha[j];
        k++;
        j++;
    }
}

// Con esta funcion ordenaremos todos los ids de forma ascendente
void ordenaMerge(vector<Usuarios>& v){
    int c = 0;
    if (v.size() < 2)
        return;
    int len = v.size();
    int first = 0;
    int mitad = first + (len - first) / 2;
    vector<Usuarios> izquierda,derecha;
    for (int i = 0; i < mitad; i++){
        Usuarios u;
        izquierda.push_back(u);
        izquierda[i] = v[i];
    }
    for (int i = mitad; i < len; i++){
        Usuarios u;
        derecha.push_back(u);
        derecha[c] = v[i];
        c++;
    }
    ordenaMerge(izquierda);
    ordenaMerge(derecha);
    merge(izquierda, derecha, v);
}

// Funcion binary search para encontrar la posicion de la fecha esperada con mayor rapidez
int binarySearch(vector<Usuarios>& v, int numBuscar){
    int n = v.size();
    int first_ocurr = n;
    int izq = 0, der = n - 1;
    while(izq <= der){
        int mid = izq + (der - izq) / 2;
            if(v[mid].id >= numBuscar){
                first_ocurr = mid;
                der = mid - 1;
            }else izq = mid + 1;
    }
    return first_ocurr;
}

