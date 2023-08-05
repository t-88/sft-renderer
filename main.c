#include "sft_renderer.h"
#include "sft_math.h"
#include "raylib_backend.h"

#include <stdbool.h>
#include <assert.h>


sftr_Canvas canvas;
RayLibBackend raylib_backend;

sftr_Matrix translated;
sftr_Matrix rotated;

sftr_Vector4 points[4];
void render();

int main(void) {
    canvas = canvas_new(400,400);



    sftr_matrix_translate((sftr_Vector4){-150-50,-150-50,0,1} ,translated);
    sftr_vector_fill(points[0],150,150,0,1);
    sftr_vector_fill(points[1],150,250,0,1);
    sftr_vector_fill(points[2],250,250,0,1);


    render();
    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    return 0;
}


void render() {
    canvas_clear(canvas,0x0000000);
    

    sftr_matrix_rotate_z(rotated,sin(raylib_backend.dt));
    for (size_t i = 0; i < 3; i++) {
        points[i] = sftr_matrix_mult_vector(translated,points[i]);
        points[i] = sftr_matrix_mult_vector(rotated,points[i]);
        
        sftr_Matrix inv;
        sftr_matrix_inverse(translated,inv);
        points[i] = sftr_matrix_mult_vector(inv,points[i]);
    }

    canvas_draw_traingle(canvas,points[0].x,points[0].y,
                                points[1].x,points[1].y,
                                points[2].x,points[2].y,
                                0xFF0000);
}
