#include "sft_renderer.h"
#include <assert.h>
void canvas_draw_rect(Canvas canvas, int x, int y, int w , int h, Int32 color) {
    assert(w >= 0 && "[Error] in canvas_draw_rect: rect width cant be negative");
    assert(h >= 0 && "[Error] in canvas_draw_rect: rect height cant be negative");

    if(x > canvas.w || y > canvas.h) {
        return;
    }

    if(x + w > canvas.w) { w = canvas.w - x ; }
    if(y + h > canvas.h) { h = canvas.h - y ; }
    if(x < 0) { x = 0; }
    if(y < 0) { y = 0; }




    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++) {
            canvas_draw_pixel(canvas, i + x, j + y,color);
        }
    

}

int main() {
    Canvas canvas = canvas_new(100,100);
    
    canvas_draw_rect(canvas,0,0,10,10,0xFFFF00);

    canvas_to_ppm(canvas,"output.ppm");


    canvas_destroy(canvas);
}