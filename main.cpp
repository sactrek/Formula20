#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <fstream>
#include <pthread.h>
#include "motores.h"
#include "hcsr04.h"

int tablaMotores[2][16];

void iniciarlizarMotores(){
    motores::funcionamientoMotorIzquierda();

    int i,j;
    printf("Tabla Tic/s \n");
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 50; j++){
            printf("%d ",tablaMotores[i][j]);
        }
        printf("\n");
    }
}


void medicionDistancia(){

}


int main(){
    iniciarlizarMotores();
    medicionDistancia();

    
}
