#include "sft_renderer.h"
#include "sft_math.h"
#include "raylib_backend.h"

#include <stdbool.h>
#include <assert.h>


#define  sftr_min(type) \
    type sftr_min_##type(type a,type b) {\
        if(a > b) {\
            return b;\
        }\
        return a;\
    }
sftr_min(int);
sftr_min(double);
sftr_min(float);

sftr_Canvas canvas;
RayLibBackend raylib_backend;



double t;
sftr_Vector4 points[4];
void render();

void sftr_barycentric_inter(sftr_Vector4 a,sftr_Vector4 b,sftr_Vector4 c,sftr_Vector4 p,double* w1,double* w2,double* w3) {
    /* 
        FROM: https://www.youtube.com/watch?v=HYAgJN3x4GA&ab_channel=SebastianLague
        Thx SebastianLague :)
    */
    double k = b.x-a.x;
    *w1 = ((p.y - a.y)  * k - (p.x - a.x) * (b.y - a.y)) / ((a.x - c.x) * (b.y - a.y) + (c.y - a.y) * k);
    *w2 = ((p.x - a.x) - *w1 * (c.x - a.x)) / k;  
    *w3 =   1 - *w1 - *w2;
}



void canvas_draw_bary_traingle(sftr_Canvas canvas,double x1,double y1,double x2,double y2,double x3,double y3,sftr_Matrix screen_space,sftr_Matrix transformation) {
    sftr_Vector4 a = {x1,y1,0,1};
    sftr_Vector4 b = {x2,y2,0,1};
    sftr_Vector4 c = {x3,y3,0,1};

    a = sftr_matrix_mult_vector(screen_space,a);
    b = sftr_matrix_mult_vector(screen_space,b);
    c = sftr_matrix_mult_vector(screen_space,c);

    
    if(a.y > b.y) {sftr_SWAP(sftr_Vector4,b,a);}
    if(a.y > c.y) {sftr_SWAP(sftr_Vector4,a,c);}
    if(b.y > c.y) {sftr_SWAP(sftr_Vector4,b,c);}




    float d12 = (float)(a.x - b.x) / (a.y - b.y);
    float d13 = (float)(a.x - c.x) / (a.y - c.y);
    float b12 = a.y - a.x / d12;
    float b13 = a.y - a.x / d13;


    for (int y = a.y; y < b.y; y++) {
        int x12 = a.x;
        if(d12 != 0) {
            x12 = (y - b12) * d12;
        }
        int x13 = a.x;
        if(d13 != 0) {
            x13 = (y - b13) * d13;
        }
        if(x12 > x13) {
            sftr_SWAP(int,x12,x13);
        }
        for (int x = x12; x < x13; x++) {
            sftr_Vector4 p = {x,y,0,1};
            double w1 , w2 , w3;

            sftr_barycentric_inter(a,b,c,p,&w1,&w2,&w3);

            int r = sftr_min_double(255,255 * w1);
            int g = sftr_min_double(255,255 * w2); 
            int b = sftr_min_double(255,255 * w3);

            sftr_Int32 color = r << 16 | g << 8 | b << 0; 

            sftr_Matrix inv;
            sftr_matrix_inverse(screen_space,inv);


            sftr_Vector4 v = sftr_matrix_mult_vector(inv,(sftr_Vector4){x,y,0,1});
            v = sftr_matrix_mult_vector(transformation,v);
            v = sftr_matrix_mult_vector(screen_space,v);

            canvas_draw_pixel(canvas,v.x,v.y,color);
        }
    }


    

    float d32 = (float)(c.x - b.x) / (c.y - b.y);
    float b32 = b.y -  b.x / d32; 
    for (int y = b.y; y < c.y; y++) {
        int x32 = a.x;
        if(d32 != 0) {
            x32 = (y - b32) * d32;
        }
        int x13 = a.x;
        if(d13 != 0) {
            x13 = (y - b13) * d13;
        }
        if(x13 > x32) {
            sftr_SWAP(int,x13,x32);
        }
        for (int x = x13; x < x32; x++) {
            sftr_Vector4 p = {x,y,0,1};
            double w1 , w2 , w3;

            sftr_barycentric_inter(a,b,c,p,&w1,&w2,&w3);

            int r = sftr_min_double(255,255 * w1);
            int g = sftr_min_double(255,255 * w2); 
            int b = sftr_min_double(255,255 * w3);

            sftr_Int32 color = r << 16 | g << 8 | b << 0; 


            sftr_Matrix inv;
            sftr_matrix_inverse(screen_space,inv);

            sftr_Vector4 v = sftr_matrix_mult_vector(inv,(sftr_Vector4){x,y,0,1});
            v = sftr_matrix_mult_vector(transformation,v);
            v = sftr_matrix_mult_vector(screen_space,v);

            canvas_draw_pixel(canvas,v.x,v.y,color);

        }
    }
}


sftr_Matrix screen_space;
sftr_Matrix rotation;

int main(void) {
    canvas = canvas_new(400,400);
    t = 0;


    sftr_vector_fill(points[0],0   ,0.5,0,1);
    sftr_vector_fill(points[1],-0.5 ,-0.5,0,1);
    sftr_vector_fill(points[2],0.5,-0.5,0,1);


    sftr_matrix_screen_space(screen_space,400,400);



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


    sftr_matrix_rotate_z(rotation,sftr_PI * sin(t));

    canvas_draw_bary_traingle(canvas,points[0].x,points[0].y,
                                     points[1].x,points[1].y,
                                     points[2].x,points[2].y,
                                     screen_space,
                                     rotation);
}
