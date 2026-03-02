build: main.c
	gcc -Wall tgaimage.c main.c -o main

clean: main
	rm main
