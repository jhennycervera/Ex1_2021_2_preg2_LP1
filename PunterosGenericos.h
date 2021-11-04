/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PunterosGenericos.h
 * Author: jhenn
 *
 * Created on 19 de octubre de 2021, 10:10 AM
 */

#ifndef PUNTEROSGENERICOS_H
#define PUNTEROSGENERICOS_H

#include <iostream>        
#include <fstream>
using namespace std;

void imprimirLinea(ofstream &, char, int);
ifstream abrirArchL(const char *nombre);
ofstream abrirArchE(const char *nombre);


void  cargalibros(void * &stockGen);
void *leerRegLibro(ifstream & arch);
void * agregarNivelLibro( int rstock);
void * agregarEspaciosNivel(int i);

void  actualiza(void* stockGen);
int buscarLibro(void **libros, char* codLib) ;
void actualizarLibro(void* librGen , int rnumCop, int rmm, int raa, int rrfecha);
void actualizarCopiaNivel(void* libNivelGen, int rnumCop,int rmm, int raa, int rrfecha, double precio );
//enum E_NIVEL{ NUMC_COPIA, ESTADO, FECHA_L, VALOR_L};
void  actualizaCopiaLibro(void* &copiaGen ,int rmm, int raa,int rrfecha, double precio );


void  imprimestock(void * stock);
void ordenaLibros( void** stock);
void quickSortNombres(void** stock, int ini, int fin);
void intercambiar(void**stock, int i, int j);
int compararNombre( void* i, void* j);
void imprimeLibros(void** st);
void imprimirDetallesLibro( void * nivel, ofstream & arch, int );
#endif /* PUNTEROSGENERICOS_H */
