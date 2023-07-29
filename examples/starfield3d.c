
#include "sft_renderer.h"
#include "raylib_backend.h"
#include <time.h>


typedef struct Star {
    double x , y;
    double z;
} Star;


#define START_COUNT 1000
static Star stars[START_COUNT];


static float STAR_SPEED = 0.8;
static double FOV = sftr_to_radians(90); 
static sftr_Canvas canvas; 
static RayLibBackend raylib_backend;


Star gen_star() {
    return (Star) {     .x = 2 * ((rand() / (double)RAND_MAX) - 0.5f),
                        .y = 2 * ((rand() / (double)RAND_MAX) - 0.5f),
                        .z = (rand() / (double)RAND_MAX) + 0.00001f
                  };
}

void render() {
    canvas_clear(canvas,0x000000);

    for (size_t i = 0; i < START_COUNT; i++) {
        stars[i].z -=  STAR_SPEED *  raylib_backend.dt;

        if(stars[i].z < 0) {
            stars[i] = gen_star();
        }

        int x = ((stars[i].x / (tan(FOV / 2) * stars[i].z)) * (canvas.w / 2.f) + canvas.w / 2.f);
        int y = ((stars[i].y / (tan(FOV / 2) * stars[i].z)) * (canvas.h / 2.f) + canvas.h / 2.f);
        if(x < 0 || x > canvas.w ||y < 0 || y > canvas.h) {
            stars[i] = gen_star();
        } else {
            canvas_draw_pixel(canvas,x,y,0xFFFFFF);
        }
    }
    
}
int main() {

    canvas = canvas_new(800,800);
    srand(time(NULL));

    for (size_t i = 0; i < START_COUNT; i++) {
        stars[i] = gen_star();
    }
    
    raylib_backend = (RayLibBackend) {
        .canvas = &canvas,
        .render = render,
    };


    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);
    return 0;
}