#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <fstream>
#include <pthread.h> 

//#include "motores.hpp"

using namespace std;

// sleep(1) = usleep(1000000) 1M
int tablaMotores[2][16];
//int tablaMotoresEnSegundos[2][16];

int leerFichero(char motor){
    printf("Leer Fichero\n");
    ifstream fichero;
    int contadorCambios = 0;
    int value = 0;
    int value2 = 0;
    int muestreos = 0;
    int msMuestreos =0;
    //char ruta[100];
	//fprintf(ruta, "/sys/class/gpio%d/value",gpio);// para ir entrando a las diferentes GPIO
	//    fichero = fopen(ruta, "r"); // cambiar para diferentes gpio 164, 165, 178, 166 
    if (motor=='I'){
        int msMuestreos =0;
        fichero.open("/sys/class/gpio/gpio208/value",ios::out); // Tacometro Izquierdo
        fichero >> value;
        fichero.close();
        while(muestreos < 1000){ // hago un pulling durante 5000ms
            fichero.open("/sys/class/gpio/gpio208/value",ios::out); // Tacometro Izquierdo
            fichero >> value2;
            fichero.close();
            if (value2!=value){
                contadorCambios++;
                value=value2; ///actualizo value al actual
            }
            muestreos++;
            usleep(1000);
            msMuestreos+=1000;
        }
        printf("La suma de los números leídos es: %d\n.",contadorCambios);
        printf("El muestreo ha durado %d ms\n",msMuestreos);
    }
    else{
        int msMuestreos =0;
        fichero.open("/sys/class/gpio/gpio177/value",ios::out); // Tacometro derecha
        fichero >> value;
        fichero.close();
        while(muestreos < 1000){ // hago un pulling durante 500ms
            fichero.open("/sys/class/gpio/gpio177/value",ios::out); // Tacometro derecha
            fichero >> value2;
            fichero.close();
            if (value2!=value){
                contadorCambios++;
                value=value2; ///actualizo value al actual
            }
            muestreos++;
            usleep(1000);      
            msMuestreos+=1000;

        }
        printf("La suma de los números leídos es: %d\n.",contadorCambios);
        printf("El muestreo ha durado %d ms\n",msMuestreos);
    }
//    int num;
//    value = fread(&num, sizeof(int), 1, fichero); // leo el primer entero 0

    return contadorCambios;
}

void escribirFichero(int duty_cycle, char motor){ //escribir en duty_cycle
    printf("Escribir Fichero\n");
    printf("Duty cycle de %d\n",duty_cycle);
    ofstream fichero;
    char ruta[100];
    int dc = 0;
    dc = duty_cycle;
    if (motor=='I'){
       //fprintf(ruta, "/sys/class/pwm/pwm-2:0/duty_cycle");
       fichero.open("/sys/class/pwn/pwm-2_0/duty_cycle",ios::out);
    } else{
	    //fprintf(ruta, "/sys/class/pwm/pwm-2:1/duty_cycle");
	    fichero.open("/sys/class/pwn/pwm-2_1/duty_cycle",ios::out);
    }
    fichero << dc << endl;
    fichero.close();
}

void activarMotorIzquierda(){
    printf("Activar Motor Izquierda\n");
    ofstream fichero;
    fichero.open("/sys/class/gpio/gpio164/value", ios::out);
    fichero << 1 << endl;
    fichero.close();
}

void activarMotorDerecha(){
    printf("Activar Motor Derecha\n");
    ofstream fichero;
    fichero.open("/sys/class/gpio/gpio165/value", ios::out);
    fichero << 1 << endl;
    fichero.close();
}

void desactivarMotorIzquierda(){
    printf("Desactivar Motor Izquierda\n");
    ofstream fichero;
    fichero.open("/sys/class/gpio/gpio164/value", ios::out);
    fichero << 0 << endl;
    fichero.close();
}

void desactivarMotorDerecha(){
    printf("Desactivar Motor Derecha\n");
    ofstream fichero;
    fichero.open("/sys/class/gpio/gpio165/value", ios::out);
    fichero << 0 << endl;
    fichero.close();
}

int checkDuty_Cycle(int duty_cycle){
    if (duty_cycle>=300 && duty_cycle < 1200){
        duty_cycle += 150;
    } else if (duty_cycle>=1200 && duty_cycle < 2200){
        duty_cycle += 200;
    } else if (duty_cycle>=2200 && duty_cycle < 3200){
        duty_cycle += 250;
    }
    return duty_cycle;
}

void *funcionamientoMotorIzquierda(){
    sleep(1);
    printf("Estoy en el motor izquierda\n");
    int i = 0; // Motor izquierda
    int j;
    int duty_cycle=300; // de 300 a 3200
    int contadorCambiosI = 0;
    // TENGO QUE ENABLEAR EL 164 echo 1 > enable en esa gpio
    activarMotorIzquierda();
    for (j=0; j<16;j++){ // Hago esto para los 50 valores diferentes
        escribirFichero(duty_cycle, 'I'); // modifico el duty_cycle I motor izquierdo
        contadorCambiosI = leerFichero('I');
        tablaMotores[i][j]=contadorCambiosI;
        //tablaMotoresEnSegundos[i][j]=contadorCambiosI*2;
        duty_cycle = checkDuty_Cycle(duty_cycle); //aumenta duty cycle en función del valor anterior
    }
    desactivarMotorIzquierda();
    return NULL;
}

//void *funcionamientoMotorDerecha(){
void *funcionamientoMotorDerecha(){
    sleep(1);
    printf("Estoy en el motor derecha\n");
    int i = 1; // Motor derecha 
    int j;
    int duty_cycle=300; // de 300 a 3200
    int contadorCambiosD = 0;
    // TENGO QUE ENABLEAR EL 165 echo 1 > enable en esa gpio
    activarMotorDerecha();
    for (j=0; j<16; j++){ // Hago esto para los 50 valores diferentes
        escribirFichero(duty_cycle, 'D'); //modifico el duty_cycle D motor derecho
        // Pongo duty_cycle +=j; RIGHTFORDWARD
        contadorCambiosD = leerFichero('D');
        tablaMotores[i][j]=contadorCambiosD;
        //tablaMotoresEnSegundos[i][j] = contadorCambiosD*2;
        duty_cycle = checkDuty_Cycle(duty_cycle); //aumenta duty cycle en función del valor anterior
    }
    desactivarMotorDerecha();
    return NULL;    
}

int main(){
    /*  
    pthread_t thread_id;
    printf("Antes del Thread Motor Izquierdo\n");
    pthread_create(&thread_id, NULL, threadMotorIzquierda, NULL);
    pthread_join(thread_id, NULL);
    printf("Después del Thread Motor Izquierdo\n");
    printf("Antes del Thread Motor Derecho\n");
    pthread_create(&thread_id, NULL, threadMotorIzquierda, NULL);
    pthread_join(thread_id, NULL);
    printf("Después del Thread Motor Derecho\n");
    printf("Impresión de la tabla de tics/50ms para diferentes valores de duty_cycle");
    */
    printf("Antes del Motor Izquierdo\n");
    funcionamientoMotorIzquierda();
    printf("Después del Motor Izquierdo\n");
    printf("Antes del Motor Derecho\n");
    funcionamientoMotorDerecha();
    printf("Después del Motor Derecho\n");
    int i,j;
    printf("Tabla Tic/s \n");
    for (int i = 0; i < 2; i++){
        for (int j = 0; j < 50; j++){
            printf("%d ",tablaMotores[i][j]);
        }
        printf("\n");
    }
    exit(0);
}
