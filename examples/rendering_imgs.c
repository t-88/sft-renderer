#include <stdlib.h>
#include <time.h>


#include "raylib_backend.h"

#include "stb_image.h"


// u need to define it last
#define SFTR_RENDERER_IMPL
#include "sft_renderer.h"


sftr_Canvas canvas;
RayLibBackend raylib_backend;



void render();


unsigned char* img;
int w , h ,comp;


int main(void) {
    srand(time(NULL));

    // load img with color comp = 3
    img = stbi_load("examples/star_wars_logo.png",&w,&h,&comp,3);
    if(stbi_failure_reason()) {
        printf("%s\n",stbi_failure_reason());
        exit(1);
    }

    canvas = canvas_new(w,h);
    canvas_copy_img(canvas,img,3);
    


    raylib_backend.canvas = &canvas;
    raylib_backend.render = render;
    raylib_backend_run(&raylib_backend);

    canvas_destroy(canvas);

    return 0;
}


void render()  {
    
}
