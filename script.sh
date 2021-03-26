#!/bin/bash
route add default gw 192.168.1.1
cd /sys/class/gpio/
echo 177 > export
echo 208 > export
echo 164 > export
echo 166 > export
echo 165 > export
echo 178 > export
echo 209 > export
cd gpio164
sh -c "echo out > direction"
cd ../*65
sh -c "echo out > direction"
cd ../*66
sh -c "echo out > direction"
cd ../*78
sh -c "echo out > direction"
cd ../*209
sh -c "echo in > direction"
cd ../../pwm/pwmchip2/
echo 0 > export
echo 1 > export

