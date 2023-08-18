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

sftr_Vector4 points[6];


sftr_Matrix screen_space;
sftr_Matrix rotate_x;
sftr_Matrix rotate_z;



int main(void) {
    canvas = canvas_new(width ,height);
    srand(time(NULL));



    img = stbi_load("checkboard.png",&w,&h,&comp,3);
    if(stbi_failure_reason()) {
        printf("%s\n",stbi_failure_reason());
        exit(1);
    }

    texture = canvas_new(w,h);
    canvas_copy_img(texture,img,3);


    sftr_vector_fill(points[0],-1/1.5f   ,1/1.5f,2.f,1);
    sftr_vector_fill(points[1],-1/1.5f ,-1 /1.5f,1.f,1);
    sftr_vector_fill(points[2],1/1.5f,-1/1.5f   ,1.f,1);
    
    sftr_vector_fill(points[3],-1/1.5f   ,1/1.5f,2.f,1);
    sftr_vector_fill(points[4],1 /1.5f,1   /1.5f,2.f,1);
    sftr_vector_fill(points[5],1 /1.5f,-1  /1.5f,1.f,1);
 

    sftr_matrix_screen_space(screen_space,400,400);
    sftr_matrix_rotate_x(rotate_x,0);
    sftr_matrix_rotate_z(rotate_z,0);



    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    canvas_destroy(texture);
       // p[i].coord.x /= ps[i].pos.z;
        // p[i].coord.y /= ps[i].pos.z;
        // p[i].pos.w = 1.f/ps[i].pos.z;
    return 0;
}


void render()  {
    canvas_clear(canvas,0x696969);


    
    sftr_Vector4 ps[6];
    sftr_TexVertex p[6];

    p[0].coord = (sftr_Vector4){.x =  0.f , .y = 0.f };
    p[1].coord = (sftr_Vector4){.x =  0.f , .y = 1.f };
    p[2].coord = (sftr_Vector4){.x =  1.f , .y = 1.f };
    p[3].coord = (sftr_Vector4){.x =  0.f  , .y = 0.f};
    p[4].coord = (sftr_Vector4){.x =  1.f  , .y = 0.f};
    p[5].coord = (sftr_Vector4){.x =  1.f  , .y = 1.f};


    for (size_t i = 0; i < 6; i++) {
        ps[i].x = points[i].x / points[i].z;
        ps[i].y = points[i].y / points[i].z;
        ps[i].w = 1;

        ps[i] = sftr_matrix_mult_vector(screen_space,ps[i]);

        p[i].pos = ps[i];
        p[i].pos.w = 1.f / points[i].z;
        p[i].coord.x = p[i].coord.x / points[i].z;
        p[i].coord.y = p[i].coord.y / points[i].z;
    }

    canvas_draw_traingle_textured(canvas,p[0],p[1],p[2],texture);
    canvas_draw_traingle_textured(canvas,p[3],p[4],p[5],texture);

    // canvas_draw_traingle_lined(canvas,ps[0].x,ps[0].y,
                                    //   ps[1].x,ps[1].y,
                                    //   ps[2].x,ps[2].y,
                                    //   0xFF00FF
                            //  );
    // canvas_draw_traingle_lined(canvas,ps[3].x,ps[3].y,
                                    //   ps[4].x,ps[4].y,
                                    //   ps[5].x,ps[5].y,
                                    //   0xFF00FF
                            //  );

}
