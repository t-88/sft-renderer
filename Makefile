INCLUDES = -Iincludes/ -lm -lraylib 

main:
	cc $@.c -o $@ $(INCLUDES)
	./$@
	rm $@


starfield3d:
	cc examples/$@.c -o examples/$@ $(INCLUDES)
	./examples/$@
	rm examples/$@
tringle_transformations:
	cc examples/$@.c -o examples/$@ $(INCLUDES)
	./examples/$@
	rm examples/$@


  