#include <stdlib.h>
#include <time.h>

#include "raylib_backend.h"

// u need to define it last
#define SFTR_RENDERER_IMPL
#include "sft_renderer.h"


#define lerp(type)  double lerp_##type(type a, type b,type c) {return a + (b - a) * c;} 
lerp(int);
lerp(double);
lerp(float);

typedef struct sftr_Vector2 {
    double x , y;
} sftr_Vector2;


sftr_Canvas canvas;
RayLibBackend raylib_backend;


double t;
sftr_Vector4 points[4];
void render();


#define GRID_SIZE 1
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


}
