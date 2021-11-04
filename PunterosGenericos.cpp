/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



/* 
 * File:   PunterosGenericos.cpp
 * Author: jhenn
 * Codigo: 20182841
 * Created on 19 de octubre de 2021, 10:10 AM
 */

#include "PunterosGenericos.h"
#include <iostream>        
#include <iomanip>
#include <fstream>
#include <cstring>        
#include "imprimevoid.h"
using namespace std;

enum E_ST {
    COD_L, NOMBRE_L, AUTOR, COPIAS, STOCK, NIV, PRECIO
};

enum E_NIVEL {
    NUMC_COPIA, ESTADO, FECHA_L, VALOR_L
};

void imprimirLinea(ofstream &Rep, char c, int cant) {
    Rep.fill(c);
    Rep << setw(cant) << c << endl;
    Rep.fill(' ');
}

ifstream abrirArchL(const char *nombre) {
    ifstream arch;
    arch.open(nombre, ios::in);
    if (!arch) {
        cout << "ERROR: no se pudo abrir el archivo " << nombre << endl;
        exit(1);
    }
    return arch;
}

ofstream abrirArchE(const char *nombre) {
    ofstream arch;
    arch.open(nombre, ios::out);
    if (!arch) {
        cout << "ERROR: no se pudo abrir el archivo " << nombre << endl;
        exit(1);
    }
    return arch;
}

void cargalibros(void *&stockGen) { //NO PUSE &

    ifstream arch = abrirArchL("Libros2.csv");

    void**libros, *buffLibr[200], *regLibro;
    int numLib = 0;

    while (1) {
        regLibro = leerRegLibro(arch);
        if (regLibro == nullptr) break;
        buffLibr[numLib] = regLibro;
        numLib++;
    }

    libros = new void*[numLib + 1];
    for (int i = 0; i < numLib; i++) {
        libros[i] = buffLibr[i];
    }

    libros[numLib] = nullptr;

    stockGen = libros;
    imprimelibros(stockGen);
}

void *leerRegLibro(ifstream & arch) {
    void**regLib;

    char rcod[8], rtitulo[500], rautor[200], aux[100], c;
    int rstock;
    double rprecio;

    arch.getline(rcod, 8, ',');
    if (arch.eof()) return nullptr;
    arch.getline(rtitulo, 500, ',');
    arch.getline(rautor, 200, ',');
    arch.getline(aux, 100, ',');

    arch >> rstock >> c>>rprecio;
    arch.get(); //\n

    char *ptrcod = new char[strlen(rcod) + 1];
    strcpy(ptrcod, rcod);
    char*ptrTit = new char[strlen(rtitulo) + 1];
    strcpy(ptrTit, rtitulo);

    char*ptrAutor = new char[strlen(rautor) + 1];
    strcpy(ptrAutor, rautor);

    regLib = new void*[7];

    regLib[COD_L] = ptrcod;
    regLib[NOMBRE_L] = ptrTit;
    regLib[AUTOR] = ptrAutor;
    regLib[COPIAS] = new int(rstock);
    regLib[STOCK] = new int(rstock);
    regLib[NIV] = agregarNivelLibro(rstock);
    regLib[PRECIO] = new double(rprecio);


    //COD_L, NOMBRE_L, AUTOR, COPIAS, STOCK, NIV, PRECIO

    return regLib;
}

void * agregarNivelLibro(int rstock) {

    void** regNivel;
    regNivel = new void*[rstock];

    for (int i = 0; i < rstock; i++) {
        regNivel[i] = agregarEspaciosNivel(i + 1);
    }
    return regNivel;

}

//NUMC_COPIA, ESTADO, FECHA_L, VALOR_L

void * agregarEspaciosNivel(int i) {

    void** reg;
    reg = new void*[4];

    reg[NUMC_COPIA] = new int(i);
    char n[11] = "Disponible";
    char* ptr = new char[11];
    strcpy(ptr, n);
    reg[ESTADO] = ptr;
    reg[FECHA_L] = new int(0);
    reg[VALOR_L] = new double(0);

    return reg;
}

void actualiza(void* stockGen) {

    void** st = (void**) stockGen;

    ifstream arch = abrirArchL("RegistroDePrestamos2.csv");

    int rcod, rdd, rmm, raa, rrfecha, rnumCop;
    char rtip, rcodLib[8], c;
    while (1) {
        arch>>rcod;
        if (arch.eof()) break;
        arch >> rtip>>c;
        arch.getline(rcodLib, 8, ',');
        arch >> rnumCop>>c;
        arch >> rdd >> c >> rmm >> c>>raa;
        rrfecha = rdd + rmm * 100 + raa * 10000;

        int pos = buscarLibro(st, rcodLib);
        if (pos != -1) {
            actualizarLibro(st[pos], rnumCop, rmm, raa, rrfecha);
        }
    }

}

int buscarLibro(void **libros, char* codLib) {

    for (int i = 0; libros[i] != nullptr; i++) {
        void** regLi = (void**) libros[i];
        char* cod = (char*) regLi[COD_L];
        if (strcmp(cod, codLib) == 0) {
            return i;
        }
    }
    return -1;
}

void actualizarLibro(void* librGen, int rnumCop, int rmm, int raa, int rrfecha) {

    void** libro = (void**) librGen;

    int* stock = (int*) libro[STOCK];
    double precio = *(double*) libro[PRECIO];

    (*stock)--;

    actualizarCopiaNivel(libro[NIV], rnumCop, rmm, raa, rrfecha, precio);
}

void actualizarCopiaNivel(void* libNivelGen, int rnumCop, int rmm, int raa, int rrfecha, double precio) {

    void** librNivel = (void**) libNivelGen;

    actualizaCopiaLibro(librNivel[rnumCop-1], rmm, raa, rrfecha, precio);  //FALTO PONER rnumCop-1

}

void actualizaCopiaLibro(void* &copiaGen, int rmm, int raa, int rrfecha, double precio) {

    void** copia = (void**) copiaGen;


    int *fecha = (int*) copia[FECHA_L];
    double * valor = (double*) copia[VALOR_L];
    char* estado = (char*) copia[ESTADO ];

    char prest[9] = "Prestamo";
    char perd[8]= "Perdida";

    if ((10 -rmm) <= 2) { //era 10 - rmm <= , no rmm -10 <2
        (*fecha) = rrfecha;
        delete estado;
        char* ptrPrest = new char[9];
        strcpy(ptrPrest, prest);
        estado = ptrPrest;
        
    } else {
        (*fecha) = rrfecha;
        (*valor) = precio;
        delete estado;
        char* ptrPerd = new char[8];
        strcpy(ptrPerd, perd);
        estado = ptrPerd;
    }
}

void  imprimestock(void * stock){
    void** st=(void**)stock;
    imprimelibros(stock);
    ordenaLibros(st);
    imprimelibros(stock);
    imprimeLibros(st);
}

void ordenaLibros( void** stock){
    int i;
    for(i=0; stock[i]!= nullptr; i++);
    
    quickSortNombres(stock, 0, i-1); 
    
}

void quickSortNombres(void** stock, int ini, int fin){
    
    if(ini>= fin) return;
 
    intercambiar(stock, ini, (ini +fin)/2);
    
    int pivote= ini;
    for(int i= ini +1 ; i<=fin; i++){
        if(compararNombre(stock[i], stock[ini]) < 0){ //menor a mayor
            pivote++;
            intercambiar(stock, i, pivote); 
        }
    }
    intercambiar(stock, ini, pivote);
    quickSortNombres(stock,ini, pivote -1);
    quickSortNombres(stock,pivote +1,fin);
    
}

void intercambiar(void**stock, int i, int j){
    
    void* aux;
    
    aux= stock[i];
    stock[i]= stock[j];
    stock[j]= aux;
    
}

int compararNombre( void* i, void* j){
    
    void** regi=(void**)i;
    void** regj=(void**)j;
    
    char* nombrei= (char*)regi[NOMBRE_L];
    char* nombrej= (char*)regj[NOMBRE_L];
    
    return strcmp(nombrei,nombrej) ;
    
}


void imprimeLibros(void** st){
    
    ofstream arch= abrirArchE( "ReporteStock.txt");
    
    for(int i=0; st[i]!= nullptr; i++){
        void** regSt= (void**)st[i];
        int numCopias= *(int*)regSt[COPIAS];
        arch<<left<<setw(10)<<(char*)regSt[COD_L]<<setw(30)<<(char*)regSt[NOMBRE_L]
                  <<setw(30)<<(char*)regSt[AUTOR]<<setw(10)<<numCopias
                  <<*(int*)regSt[STOCK]<<endl;
        arch<<"Detalle de copias:"<<endl;
        imprimirDetallesLibro(regSt[NIV], arch, numCopias);
    }
    
    
}

void imprimirDetallesLibro( void * nivel, ofstream & arch, int numCopias){
    
    void** regNiv= (void**)nivel;
    
    for(int i=0; i<numCopias; i++){
        void** regNiveli= (void**)regNiv[i];
        
        int copia= *(int*)regNiveli[NUMC_COPIA];
        int fecha= *(int*)regNiveli[FECHA_L];
        char* estado= (char*)regNiveli[ESTADO];
        double valor= *(double*)regNiveli[VALOR_L];
        
        arch<<setw(10)<< copia<< setw(20)<<estado<<setw(10)
                <<fecha<<valor<<endl;
    }
}

//COD_L, NOMBRE_L, AUTOR, COPIAS, STOCK, NIV, PRECIO
//NUMC_COPIA, ESTADO, FECHA_L, VALOR_L