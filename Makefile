#DRAGAN Marius 312CB
build:
	gcc -o tema2 tema2.c 
run:
	./tema2 $(ARGS)
clean:
	rm tema2
valgrind:
	valgrind --leak-check=yes --track-origins=yes ./tema2 $(ARGS)