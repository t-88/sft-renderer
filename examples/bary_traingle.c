#include "sft_renderer.h"
#include "raylib_backend.h"


sftr_Canvas canvas;
RayLibBackend raylib_backend;



double t;
sftr_Vector4 points[4];
void render();


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


    sftr_matrix_rotate_z(rotation,t * 0.5);


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
