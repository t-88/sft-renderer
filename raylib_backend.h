#ifndef RAYLIB_BACKEND_H
#define RAYLIB_BACKEND_H

#include <raylib.h>
#include "sft_renderer.h"


typedef struct RayLibBackend {
    sftr_Canvas* canvas;
    void (*render)();
    float dt;
} RayLibBackend;

void raylib_backend_run(RayLibBackend* backend) {
    if(backend->canvas == NULL) {
        printf("[ERROR] backend->canvas == NULL\n");
        exit(0);
    }

    int w = backend->canvas->w,
        h = backend->canvas->h;
    SetTraceLogLevel(69);
    InitWindow(w,h,"raylib-backend");
    SetTargetFPS(60);



    Image image = (Image) {
            .width =  w,
            .height = h,
            .mipmaps = 1, 
            .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
        };
    Texture2D texture;
    Color *pixels = (Color *)calloc(w*h, sizeof(Color));


    while (!WindowShouldClose()) {

        backend->dt =  GetFrameTime();
        backend->render();

        for (int y = 0; y < h ; y++) {
            for (int x = 0; x < w; x++) {
                pixels[x + y * w] = (Color) {
                                                    .r = backend->canvas->pixels[x + (h - y) * w].color.r,
                                                    .g = backend->canvas->pixels[x + (h - y) * w].color.g,
                                                    .b = backend->canvas->pixels[x + (h - y) * w].color.b,
                                                    .a = backend->canvas->pixels[x + (h - y) * w].color.a
                                                };
            }            
        }
        image.data = pixels;
        texture = LoadTextureFromImage(image);
        BeginDrawing();
            DrawTextureRec(texture, (Rectangle) { 0, 0, (float)w, (float)-h }, (Vector2) { 0, 0 }, WHITE);
        EndDrawing();
    }
    


    UnloadTexture(texture);
    free(pixels);
}


#endif