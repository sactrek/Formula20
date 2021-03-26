#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>  //Header file for sleep(). man 3 sleep for details. 
#include <fstream>
#include <pthread.h> 
#include <chrono>
#include <ctime> 

using namespace std;

float round(float var) 
{
    float value = (int)(var * 100 + .5); 
    return (float)value / 100; 
} 


float distance_measurement(){
    chrono::time_point<chrono::system_clock> pulseStart, pulseEnd; 
    chrono::duration<float> pulseDuration;
    //printf("Iniciamos medición con el sensor HC-SR04\n");
    //GPIO.output(TRIG,True)
    //printf("Trigger True\n");
    ofstream fichero;
    fichero.open("/sys/class/gpio/gpio234/value",ios::out);
    fichero << 1 << endl;
    fichero.close();
    // time.sleep()
    sleep(0.00001);
    //GPIO.output(TRIG,False);
    //printf("Trigger False\n");
    fichero.open("/sys/class/gpio/gpio234/value",ios::out);
    fichero << 0 << endl;
    fichero.close();
    int contador = 0;
    int value = -1;
    
    //printf("Echo value\n");
    ifstream fEcho;
    fEcho.open("/sys/class/gpio/gpio209/value", ios::out);
    fEcho >> value;
    fEcho.close();
    while(value==0){
        //printf("Value == 0\n");
        pulseStart = chrono::system_clock::now();
        contador++;
        ifstream fEcho;
        fEcho.open("/sys/class/gpio/gpio209/value", ios::out);
        fEcho >> value;
        fEcho.close();
    }
    while(value==1){
        //printf("Value == 1\n");
        pulseEnd = chrono::system_clock::now();
        ifstream fEcho;
        fEcho.open("/sys/class/gpio/gpio209/value", ios::out);
        fEcho >> value;
        fEcho.close();
    }

    //printf("Post while\n");
    pulseDuration = pulseEnd - pulseStart;
    //printf("Pulse Duration %f \n", pulseDuration.count());
    float distancia = 0;
    distancia = pulseDuration.count();
    distancia = distancia*17150;
    distancia = round(distancia);
    return distancia;
}


int main(){
    float distancia;
    distancia = distance_measurement();
    while(1){
        printf("Distancia: %f \n", distancia);
        sleep(1);
        if (distancia <= 5){
            printf("Cuidao manin, estás muy cerca\n");
            break;
        } else{
            distancia = distance_measurement();
        }
    }
}