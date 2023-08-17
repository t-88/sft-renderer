#include <stdlib.h>
#include <time.h>

#include "raylib_backend.h"

// u need to define it last
#define SFTR_RENDERER_IMPL
#include "sft_renderer.h"


sftr_Canvas canvas;
RayLibBackend raylib_backend;



static const size_t width =  400;
static const size_t height =  400;

void render();

// void draw_line(sftr_Canvas canvas, int x0 , int y0, int x1 , int y1,sftr_Int32 color) {
//     // lines are leave pixels not filled bc i focus on one axis
//     // we can find multiple values of y for one x 
//     // and same for y
//     // we have to calculate dx and dy and take the smallest one and draw accordingly

//     int dx = x0 - x1;
//     int dy = y0 - y1;
//     float a = (float) dy / dx;

//     if(dx > dy) {
//         if (x0 > x1) {
//             sftr_SWAP(int,x0,x1);
//             sftr_SWAP(int,y0,y1);
//         }

//         float y = y0;
//         for (int x = x0; x <= x1; x++) {
//             canvas_draw_pixel(canvas,x,y,color);
//             y = y + a;
//         }
//     } else {
//         if (y0 > y1) {
//             sftr_SWAP(int,x0,x1);
//             sftr_SWAP(int,y0,y1);
//         }

//         float x = x0;
//         for (int y = y0; y <= y1; y++) {
//             canvas_draw_pixel(canvas,x,y,color);
//             x = x + a;
//         }

//     }

// }

int main(void) {
    canvas = canvas_new(width ,height);
    srand(time(NULL));


    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    return 0;
}


void render() {
    canvas_clear(canvas,0x0000000);


}
