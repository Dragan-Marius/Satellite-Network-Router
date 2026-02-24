#DRAGAN Marius 312CB
build:
	gcc -o SatelliteNetwork SatelliteNetwork.c 
run:
	./SatelliteNetwork $(ARGS)
clean:
	rm SatelliteNetwork
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./SatelliteNetwork $(ARGS)