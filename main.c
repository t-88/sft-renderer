#include <stdlib.h>
#include <time.h>


#include "raylib_backend.h"

#include "stb_image.h"


// u need to define it last
#define SFTR_RENDERER_IMPL
#include "sft_renderer.h"


sftr_Canvas canvas;
RayLibBackend raylib_backend;



static const size_t width =  400;
static const size_t height =  400;


void render();
int main(void) {
    canvas = canvas_new(width ,height);
    srand(time(NULL));



    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    return 0;
}


void render()  {
    canvas_clear(canvas,0x101010);

    
}
