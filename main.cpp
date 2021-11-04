/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Jhenny Cervera Vargas
 * Codigo: 20182841
 * Created on 19 de octubre de 2021, 10:09 AM
 */


#include "PunterosGenericos.h"
/*
 * 
 */
int main(int argc, char** argv) {

    void* stock;
    cargalibros(stock);
    actualiza(stock);
    imprimestock(stock);

    return 0;
}

