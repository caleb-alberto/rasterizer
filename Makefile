build: main.c
	gcc tgaimage.c main.c -o main

clean: main
	rm main
