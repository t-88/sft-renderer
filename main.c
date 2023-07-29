#include "sft_renderer.h"
#include "raylib_backend.h"


sftr_Canvas canvas;
RayLibBackend raylib_backend;

void render() {
    canvas_clear(canvas,0x0000000);
    canvas_draw_traingle(canvas,50,10,200,200,50,300);
}

int main(void) {
    canvas = canvas_new(400,400);

    render();
    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    return 0;
}