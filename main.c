#include <stdlib.h>
#include <time.h>


#include "raylib_backend.h"

#include "stb_image.h"


// u need to define it last
#define SFTR_RENDERER_IMPL
#include "sft_renderer.h"




sftr_Canvas canvas;
RayLibBackend raylib_backend;



void render();
int main(void) {
    srand(time(NULL));

    canvas = canvas_new(800 ,600);


    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    return 0;
}


void render()  {
    canvas_clear(canvas,0x101010);

}
