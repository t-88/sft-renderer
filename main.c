#include "sft_renderer.h"
#include "raylib_backend.h"


sftr_Canvas canvas;
RayLibBackend raylib_backend;


double x = 1;
void render() {
    canvas_clear(canvas,0xFFFF00);
    canvas_draw_rect(canvas,x,0,50,50,0x000000);
    x += 100 * raylib_backend.dt;
}

int main(void) {
    canvas = canvas_new(400,400);

    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);


    canvas_destroy(canvas);
    return 0;
}