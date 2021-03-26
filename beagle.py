
enable = 0
echo 0 > enable OS.system?
Motor duty_cycle = 700
enable = 1

El motor se enciende
Saco durante 1 segundo, los tics (sys/class/gpio/gpio177/value)
Almaceno valor de tics a array.LadorMotor.duty_cycle

enable = 0
Motor duty_cycle = 700 + el incremento del rango #100 -> 800
enable = 1

El motor se enciende
Saco durante 1 segundo, los tics 
Almaceno valor de tics a array.LadorMotor.duty_cycle