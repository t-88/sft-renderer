main:
	cc main.c -o main -Iincludes/ -lm -lraylib
	./main


starfield3d:
	cc examples/starfield3d.c -o examples/starfield3d -Iincludes/ -lm -lraylib
	./examples/starfield3d

  