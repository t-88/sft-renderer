#include "sft_renderer.h"
#include "sft_math.h"
#include "raylib_backend.h"

#include <stdbool.h>
#include <assert.h>


sftr_Canvas canvas;
RayLibBackend raylib_backend;

sftr_Matrix translated;
sftr_Matrix rotated_y;
sftr_Matrix rotated_x;
sftr_Matrix rotated_z;
sftr_Matrix scaled;
sftr_Matrix screen_space;


double t;
sftr_Vector4 points[4];
void render();




int main(void) {
    canvas = canvas_new(400,400);
    t = 0;


    sftr_vector_fill(points[0],0   ,0.5,0,1);
    sftr_vector_fill(points[1],0.5 ,0,0,1);
    sftr_vector_fill(points[2],-0.5,0,0,1);

    sftr_matrix_rotate_y(rotated_y,0.1);
    sftr_matrix_rotate_x(rotated_x,0.01);
    sftr_matrix_rotate_z(rotated_z,0.01);
    sftr_matrix_scale((sftr_Vector4){1,1,1,1},scaled);
    sftr_matrix_translate((sftr_Vector4){0,0,0,1},translated);

    for (size_t i = 0; i < 3; i++) {
        points[i] = sftr_matrix_mult_vector(scaled,points[i]);
    }

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

    sftr_Vector4 ps[3];

    for (size_t i = 0; i < 3; i++) {
        points[i] = sftr_matrix_mult_vector(rotated_z,points[i]);
        points[i] = sftr_matrix_mult_vector(rotated_y,points[i]);
        points[i] = sftr_matrix_mult_vector(rotated_x,points[i]);

        ps[i] = sftr_matrix_mult_vector(screen_space,points[i]);
    }

    canvas_draw_traingle(canvas,ps[0].x,ps[0].y,
                                ps[1].x,ps[1].y,
                                ps[2].x,ps[2].y,
                                0xFF0000);
}
