INCLUDES = -Iincludes/ -lm -lraylib  -g

main:
	cc $@.c -o $@ $(INCLUDES)
	./$@
	rm $@
show: 
	feh dev-imgs/raycaster_saved/$(idx).ppm


starfield3d:
	cc examples/$@.c -o examples/$@ $(INCLUDES)
	./examples/$@
	rm examples/$@
tringle_transformations:
	cc examples/$@.c -o examples/$@ $(INCLUDES)
	./examples/$@
	rm examples/$@
bary_traingle:
	cc examples/$@.c -o examples/$@ $(INCLUDES)
	./examples/$@
	rm examples/$@

rendering_imgs:
	cc examples/$@.c -o examples/$@ $(INCLUDES)
	./examples/$@
	rm examples/$@


texture_mapping:
	cc examples/$@.c -o examples/$@ $(INCLUDES)
	./examples/$@
	rm examples/$@

perspective_correction:
	cc examples/$@.c -o examples/$@ $(INCLUDES)
	./examples/$@
	rm examples/$@

raytracer:
	cc examples/$@.c -o examples/$@ $(INCLUDES)
	./examples/$@
	rm examples/$@

