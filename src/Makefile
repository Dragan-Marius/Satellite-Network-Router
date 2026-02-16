#DRAGAN Marius 312CB
build:
	gcc -o SatteliteNetwork SatteliteNetwork.c 
run:
	./SatteliteNetwork $(ARGS)
clean:
	rm SatteliteNetwork
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./SatteliteNetwork $(ARGS)