#include "sft_renderer.h"
#include "sft_math.h"
#include "raylib_backend.h"


#include <stdbool.h>
#include <assert.h>
#include <math.h>

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


typedef struct sftr_Vertex {
    sftr_Vector4 pos;
    sftr_Vector4 color;
} sftr_Vertex;

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



void canvas_draw_bary_traingle(sftr_Canvas canvas,sftr_Vertex a,sftr_Vertex b,sftr_Vertex c) {
    int min_x = a.pos.x;
    if(min_x > b.pos.x) min_x = b.pos.x;
    if(min_x > c.pos.x) min_x = c.pos.x;

    int max_x = a.pos.x;
    if(max_x < b.pos.x) max_x = b.pos.x;
    if(max_x < c.pos.x) max_x = c.pos.x;

    int min_y = a.pos.y;
    if(min_y > b.pos.y) min_y = b.pos.y;
    if(min_y > c.pos.y) min_y = c.pos.y;

    int max_y = a.pos.y;
    if(max_y < b.pos.y) max_y = b.pos.y;
    if(max_y < c.pos.y) max_y = c.pos.y;

    for (int y = min_y; y < max_y; y++) {
        for (int x = min_x; x < max_x; x++) {
            double w1,w2,w3;
            sftr_Vector4 p = {x,y,0,1};

            sftr_barycentric_inter(a.pos,b.pos,c.pos,p,&w1,&w2,&w3);
            if(w1 >= 0 && w2 >= 0 && w1 + w2 <= 1) {
                int r = sftr_min_double(255,a.color.x * w1) + sftr_min_double(255,a.color.y * w1) + sftr_min_double(255,a.color.z * w1);
                int g = sftr_min_double(255,b.color.x * w2) + sftr_min_double(255,b.color.y * w2) + sftr_min_double(255,b.color.z * w2);
                int b = sftr_min_double(255,c.color.x * w3) + sftr_min_double(255,c.color.y * w3) + sftr_min_double(255,c.color.z * w3);


                sftr_Int32 color = r << 16 | g << 8 | b << 0; 

                canvas_draw_pixel(canvas,p.x,p.y,color);
            }
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


    sftr_matrix_rotate_z(rotation,t);


    sftr_Vertex ps[3];
    ps[0].color = (sftr_Vector4){255,0,0,0};
    ps[1].color = (sftr_Vector4){0,255,0,0};
    ps[2].color = (sftr_Vector4){0,0,255,0};
    for (size_t i = 0; i < 3; i++) {
        ps[i].pos = sftr_matrix_mult_vector(rotation,points[i]) ;
        ps[i].pos = sftr_matrix_mult_vector(screen_space,ps[i].pos) ;
    }
    

    canvas_draw_bary_traingle(canvas,ps[0],
                                     ps[1],
                                     ps[2]);
}
