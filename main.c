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


#define sftr_Color_From_Pixel(pixle) ((pixle).color.r << 16 | (pixle).color.g << 8 | (pixle).color.b << 0)
void canvas_draw_traingle_textured(sftr_Canvas canvas,sftr_TexVertex a,sftr_TexVertex b,sftr_TexVertex c,sftr_Canvas texture) {
    // bounding box
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

    // filling the bounding box
    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            double w1,w2,w3;
            sftr_Vector4 p = {x,y,0,1};

            sftr_barycentric_inter(a.pos,b.pos,c.pos,p,&w1,&w2,&w3);
            if(w1 >= 0 && w2 >= 0 && w1 + w2 <= 1) {
                int tex_x = (a.coord.x * w1 +  b.coord.x * w2 + c.coord.x * w3) * texture.w;
                int tex_y = (a.coord.y * w1 +  b.coord.y * w2 + c.coord.y * w3) * texture.h;
                // printf("%d %d\n",tex_x,tex_y);

                Pixel pixel = texture.pixels[(int)(tex_x + tex_y * texture.w )];
                sftr_Int32 color = sftr_Color_From_Pixel(pixel);

                canvas_draw_pixel(canvas,p.x,p.y,color);
            }
        }
    }

}

void render();



sftr_Canvas texture;
unsigned char* img;
int w , h ,comp;

sftr_Vector4 points[4];
sftr_Matrix screen_space;



int main(void) {
    canvas = canvas_new(width ,height);
    srand(time(NULL));



    img = stbi_load("images.png",&w,&h,&comp,3);
    if(stbi_failure_reason()) {
        printf("%s\n",stbi_failure_reason());
        exit(1);
    }

    texture = canvas_new(w,h);
    canvas_copy_img(texture,img,3);


    // TODO: bug in bary coords gives nan
    sftr_vector_fill(points[0],-0.5   ,0.5,0,1);
    sftr_vector_fill(points[1],-0.5 ,-0.5,0,1);
    sftr_vector_fill(points[2],0.5,-0.5,0,1);
    
    sftr_matrix_screen_space(screen_space,400,400);



    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    canvas_destroy(texture);

    return 0;
}


void render()  {
    canvas_clear(canvas,0x000000);


    
    sftr_Vertex ps[3];
    ps[0].color = (sftr_Vector4){255,0,0,0};
    ps[1].color = (sftr_Vector4){0,255,0,0};
    ps[2].color = (sftr_Vector4){0,0,255,0};
    for (size_t i = 0; i < 3; i++) {
        ps[i].pos = sftr_matrix_mult_vector(screen_space,points[i]) ;
    }

    // canvas_draw_bary_traingle(canvas,ps[0],
                                    //  ps[1],
                                    //  ps[2]);

    sftr_TexVertex p[3];
    p[0] = (sftr_TexVertex) {ps[0].pos,  (sftr_Vector4){.x =  0 , . y = 0}};
    p[1] = (sftr_TexVertex) {ps[1].pos,  (sftr_Vector4){.x =  0 , . y = 1}};
    p[2] = (sftr_TexVertex) {ps[2].pos,  (sftr_Vector4){.x =  1 , . y = 1}};
    canvas_draw_traingle_textured(canvas,p[0],p[1],p[2],texture);

    canvas_draw_traingle_lined(canvas,ps[0].pos.x,ps[0].pos.y,
                                      ps[1].pos.x,ps[1].pos.y,
                                      ps[2].pos.x,ps[2].pos.y,
                                      0xFF0000
                             );
}
