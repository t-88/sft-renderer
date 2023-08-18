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



sftr_Canvas texture;
unsigned char* img;
int w , h ,comp;

sftr_Vector4 points[3];
sftr_Vector4 points1[3];


sftr_Matrix screen_space;
sftr_Matrix rotate_x;
sftr_Matrix rotate_z;



int main(void) {
    canvas = canvas_new(width ,height);
    srand(time(NULL));



    img = stbi_load("star_wars_logo.png",&w,&h,&comp,3);
    if(stbi_failure_reason()) {
        printf("%s\n",stbi_failure_reason());
        exit(1);
    }

    texture = canvas_new(w,h);
    canvas_copy_img(texture,img,3);


    sftr_vector_fill(points[0],-1/2.f   ,1/2.f,0,1);
    sftr_vector_fill(points[1],-1/2.f ,-1 /2.f,0,1);
    sftr_vector_fill(points[2],1/2.f,-1/2.f,0,1);
    
    sftr_vector_fill(points1[0],-1/2.f   ,1/2.f,0,1);
    sftr_vector_fill(points1[1],1 /2.f,1   /2.f,0,1);
    sftr_vector_fill(points1[2],1 /2.f,-1  /2.f,0,1);
 

    sftr_matrix_screen_space(screen_space,400,400);
    sftr_matrix_rotate_x(rotate_x,0);
    sftr_matrix_rotate_z(rotate_z,0);



    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    canvas_destroy(texture);

    return 0;
}


void render()  {
    canvas_clear(canvas,0x000000);


    
    sftr_Vertex ps[6];

    sftr_matrix_rotate_x(rotate_x,sin(raylib_backend.t / 10) * 2 * sftr_PI);
    sftr_matrix_rotate_z(rotate_z,cos(raylib_backend.t / 5) * 2 * sftr_PI);
    for (size_t i = 0; i < 6; i++) {
        ps[i].pos = sftr_matrix_mult_vector(rotate_x,points[i]);
        ps[i].pos = sftr_matrix_mult_vector(rotate_z,ps[i].pos);
        ps[i].pos = sftr_matrix_mult_vector(screen_space,ps[i].pos) ;
    }


    sftr_TexVertex p[6];
    p[0] = (sftr_TexVertex) {ps[0].pos,  (sftr_Vector4){.x =  0 , . y = 0}};
    p[1] = (sftr_TexVertex) {ps[1].pos,  (sftr_Vector4){.x =  0 , . y = 1}};
    p[2] = (sftr_TexVertex) {ps[2].pos,  (sftr_Vector4){.x =  1 , . y = 1}};

    p[3] = (sftr_TexVertex) {ps[3].pos,  (sftr_Vector4){.x =  0 , . y = 0}};
    p[4] = (sftr_TexVertex) {ps[4].pos,  (sftr_Vector4){.x =  1 , . y = 0}};
    p[5] = (sftr_TexVertex) {ps[5].pos,  (sftr_Vector4){.x =  1 , . y = 1}};


    canvas_draw_traingle_textured(canvas,p[0],p[1],p[2],texture);
    canvas_draw_traingle_textured(canvas,p[3],p[4],p[5],texture);

}
