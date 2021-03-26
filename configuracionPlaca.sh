#!/bin/sh 
###Codigo

route add default gw 192.168.1.1

cd /sys/class/gpio
echo 177 > export #Tacómetro der 
echo 208 > export #Tacómetro izq
echo 164 > export #Motor Izq forward (out)
echo 165 > export #Motor Der forward (out)
echo 166 > export #Motor Der backward (out)
echo 178 > export #Motor Izq backward (out)
echo 234 > export #Trigger (out)
echo 209 > export #Echo (in)
cd gpio164
echo out > direction
echo 0 > value
cd ../gpio165
echo out > direction
echo 0 > value
cd ../gpio166
echo out > direction
echo 0 > value
cd ../gpio178
echo out > direction
echo 0 > value
## Configuración Trigger y Echo HC-SR04
cd ../gpio234
echo out > direction
cd ../gpio209
echo in > direction
### Motores pwm
cd ../../pwm/pwmchip2
echo 0 > export
echo 1 > export
cd ../pwm-2:0
echo 4000 > period
echo 2000 > duty_cycle
echo 1 > enable
cd ../pwm-2:1
echo 4000 > period
echo 2000 > duty_cycle
echo 1 > enable


