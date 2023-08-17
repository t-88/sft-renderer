#include <stdlib.h>
#include <time.h>

#include "raylib_backend.h"

#define SFTR_RENDERER_IMPL
#include "sft_renderer.h"


sftr_Canvas canvas;
RayLibBackend raylib_backend;


double t;
sftr_Vector4 points[4];
void render();


#define GRID_SIZE 5
static const size_t width =  400;
static const size_t height =  400;

int main(void) {
    canvas = canvas_new(width ,height);
    t = 0;
    srand(time(NULL));


    render();
    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    return 0;
}


void render() {

    canvas_clear(canvas,0x0000000);
    t += raylib_backend.dt;


    for (size_t y = 0; y < height; y+=GRID_SIZE) {
        for (size_t x = 0; x < width; x+=GRID_SIZE) {
            sftr_Int32 color = (rand() % 255) << 16 | (rand() % 255) << 8 |(rand() % 255) << 0;

            canvas_draw_rect(canvas,x,y,GRID_SIZE,GRID_SIZE,color);
        }
    }
    

}
