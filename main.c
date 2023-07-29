#include "sft_renderer.h"
#include "raylib_backend.h"


sftr_Canvas canvas;
RayLibBackend raylib_backend;



void canvas_draw_circle(sftr_Canvas canvas,int x,int y,int r , sftr_Int32 color) {
    if(r <= 0) {
        assert(0 && "[Error] canvas_draw_circle: cant have zero or nigative radius");
        return;
    }
    for (int j = -r; j < r; j++) {
        for (int i =  -r; i < r; i++) { 
            if(i*i + j*j <= r*r) {
                canvas_draw_pixel(canvas,x+i,y+j,color);
            }
        }
    }
    
}
void canvas_draw_traingle(sftr_Canvas canvas,int x1,int y1,int x2,int y2,int x3,int y3) {
    // top y
    if(y1 > y2) {sftr_SWAP(int,y2,y1);sftr_SWAP(int,x2,x1);}
    if(y1 > y3) {sftr_SWAP(int,y1,y3);sftr_SWAP(int,x1,x3);}
    // min y
    if(y2 > y3) {sftr_SWAP(int,y2,y3);sftr_SWAP(int,x2,x3);}



    double d12 = (x1-x2)/(y1-y2); 
    double d13 = (x1-x3)/(y1-y3);


    canvas_draw_circle(canvas,x1,y1,5,0xFF00FF);
    canvas_draw_circle(canvas,x2,y2,5,0xFF00FF);
    canvas_draw_circle(canvas,x3,y3,5,0xFF00FF);

    canvas_draw_line(canvas,x1,y1,x2,y2,0xFFFFFF);
    canvas_draw_line(canvas,x1,y1,x3,y3,0xFFFFFF);
    canvas_draw_line(canvas,x2,y2,x3,y3,0xFFFFFF);


    for (size_t y = y1; y < y2; y++) {
        canvas_draw_line(canvas,y/d13,y,y/d12,y,0xFFFFFF);
    }
    
}

void render() {
    canvas_clear(canvas,0x0000000);
    
    canvas_draw_circle(canvas,100,100,5,0xFF00FF);
    canvas_draw_circle(canvas,50,200,5,0xFF00FF);
    canvas_draw_line(canvas,100,100,50,200,0xFFFFFF);

    // canvas_draw_traingle(canvas,100,100,50,200,150,300);
}

int main(void) {
    canvas = canvas_new(400,400);

    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    return 0;
}