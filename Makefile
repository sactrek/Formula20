setir.out: main.o motores.o hcsr04.o
	g++ -g main.o motores.o hcsr04.o

motores.o: motores.cpp motores.hpp
	g++ -g motores.cpp

hcsr04.o : hcsr04.cpp hcsr04.hpp
	g++ -g hcsr04.cpp

main.o: main.cpp motores.cpp hcsr04.cpp
	g++ -g main.cpp

.PHONY: clean
clean:
	rm -rf *.o