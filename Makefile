main:
	cc main.c -o main -Iincludes/ -lm -lraylib
	./main
	rm main


starfield3d:
	cc examples/starfield3d.c -o examples/starfield3d -Iincludes/ -lm -lraylib
	./examples/starfield3d
	rm examples/starfield3d

  