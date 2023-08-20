#ifndef RAYLIB_BACKEND_H
#define RAYLIB_BACKEND_H

#include <raylib.h>
#include "sft_renderer.h"


typedef struct RayLibBackend {
    sftr_Canvas* canvas;
    void (*render)();
    float dt;
    float t;

    Image raylib_img;
    Texture2D raylib_texture;
    Color* raylib_pixels;
} RayLibBackend;

void raylib_backend_apply_to_texture(RayLibBackend* backend);


void raylib_backend_run(RayLibBackend* backend) {
    if(backend->canvas == NULL) {
        printf("[ERROR] backend->canvas == NULL\n");
        return;
    }

    if(backend->canvas->w == 0 || backend->canvas->h == 0) {
        printf("[ERROR] width or height of canvas == 0\n");
        return;
    }

    int w = backend->canvas->w,
        h = backend->canvas->h;
    SetTraceLogLevel(69);
    InitWindow(w,h,"raylib-backend");
    SetTargetFPS(60);



    backend->raylib_img = (Image) {
            .width =  w,
            .height = h,
            .mipmaps = 1, 
            .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
        };
    backend->raylib_pixels = (Color *)calloc(w*h, sizeof(Color));

    backend->t = 0;


    while (!WindowShouldClose()) {
        backend->dt =  GetFrameTime();
        backend->t +=   backend->dt;
        backend->render();

        raylib_backend_apply_to_texture(backend);
        
    }
    
    free(backend->raylib_pixels);
}

void raylib_backend_apply_to_texture(RayLibBackend* backend) {
    int w = backend->canvas->w;
    int h = backend->canvas->h;
    for (int y = 0; y < h ; y++) {
        for (int x = 0; x < w; x++) {
            backend->raylib_pixels[x + y * w] = (Color) {
                                                .r = backend->canvas->pixels[x + (h - y) * w].color.r,
                                                .g = backend->canvas->pixels[x + (h - y) * w].color.g,
                                                .b = backend->canvas->pixels[x + (h - y) * w].color.b,
                                                .a = backend->canvas->pixels[x + (h - y) * w].color.a
                                            };
        }            
    }
    backend->raylib_img.data = backend->raylib_pixels;
    backend->raylib_texture = LoadTextureFromImage(backend->raylib_img);
    BeginDrawing();
        DrawTextureRec(backend->raylib_texture, (Rectangle) { 0, 0, (float)w, (float)-h }, (Vector2) { 0, 0 }, WHITE);
    EndDrawing();
    // memory leak??
    UnloadTexture(backend->raylib_texture);
}


#endif