#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HexToColor(color) (Color) { \
                            .r = ((color) >> 8 * 3) & 0xFF, \
                            .g = ((color) >> 8 * 2) & 0xFF, \
                            .b = ((color) >> 8 * 1) & 0xFF, \
                            .a = ((color) >> 8 * 0) & 0xFF  \
                         }

#define canvas_draw_pixel(canvas,x,y,c) canvas.pixels[(x) + (y) * canvas.w].color = canvas_hex_to_color((c)) 
#define SWAP(type,a,b) { type t = a; a = b; b = t;  } 





typedef  unsigned int Int32;

typedef struct String {
    char* val;
    size_t count;
} String;




typedef struct Color {
    int r , g , b , a;

} Color;
typedef struct Pixel {
    Color color;
} Pixel;

typedef struct Canvas {
    int w , h;
    Pixel* pixels; 
} Canvas;


String string_from_int(int val) {
    // generate a string from integer, need to clean mem ur self

    String str;
    if(val == 0) {
        str.count = 1;
        str.val = malloc(str.count);
        str.val[0] = '0';
    } else {
        str.count = (int) log10(val) + 1;
        str.val = malloc(str.count);
    }
    size_t idx = 0;
    while (val != 0) {
        int mod = val % 10;
        val /= 10;
        str.val[str.count - idx - 1] = '0' + mod;
        idx++;
    }
    return str;
}
void string_destroy(String* str) {
    free(str->val);
}

Canvas canvas_new(int w, int h) {
    Canvas canvas = {.w = w, .h = h};
    canvas.pixels = (Pixel*) malloc(sizeof(Pixel) * w * h);
    for (size_t i = 0; i < canvas.w * canvas.h; i++) {
        canvas.pixels[i].color.r = 0;
        canvas.pixels[i].color.g = 0;
        canvas.pixels[i].color.b = 0;
    }
    
    return canvas;
}
void canvas_destroy(Canvas canvas) {
    free(canvas.pixels);
}

Color canvas_hex_to_color(Int32 color) {
    if(color <= 0xFFFFFF) {
        color <<= 8;
    } 
    return HexToColor(color);
}
void canvas_clear(Canvas canvas,Int32 color) {
    Color c = canvas_hex_to_color(color); 
    for (size_t y = 0; y < canvas.h; y++)
        for (size_t x = 0; x < canvas.w; x++) {
            canvas.pixels[x + y * canvas.w].color = c;
        }
}

int canvas_to_ppm(Canvas canvas,const char* file_name) {
    FILE* f = fopen(file_name,"wb");
    if(f == NULL)  {
        goto canvas_to_ppm_save_exit;
    }

    fwrite("P3\n",1,3,f);

    String str_w = string_from_int(canvas.w);
    String str_h = string_from_int(canvas.h);
    fwrite(str_w.val,1,str_w.count,f);
    string_destroy(&str_w);
    fwrite(" ",1,1,f);
    fwrite(str_h.val,1,str_h.count,f);
    string_destroy(&str_h);
    fwrite(" ",1,1,f);
    fwrite("255",1,3,f);
    fwrite("\n",1,1,f);


    for (size_t i = 0; i < canvas.h * canvas.w; i++) {
            String r = string_from_int(canvas.pixels[i].color.r);            
            String g = string_from_int(canvas.pixels[i].color.g);            
            String b = string_from_int(canvas.pixels[i].color.b);            

            fwrite(r.val,1,r.count,f);
            fwrite(" ",1,1,f);
            fwrite(g.val,1,g.count,f);
            fwrite(" ",1,1,f);
            fwrite(b.val,1,b.count,f);
            fwrite("\n",1,1,f);

            string_destroy(&r);
            string_destroy(&g);
            string_destroy(&b);
        }
    

    canvas_to_ppm_save_exit:
    fclose(f);  
}


void canvas_draw_line(Canvas canvas,int x1,int y1, int x2,int y2,Int32 c) {

    double a = 0;
    if(x1 != x2) {
        if(x1 > x2) {
            SWAP(int,y1,y2);
            SWAP(int,x1,x2);
        }
        a = (double)(y1 - y2)/(x1 - x2); 
        for (size_t x = x1; x < x2; x++) {
            canvas_draw_pixel(canvas,x,(int) (a * x + y1),c);
        }
    } else {
        if(y1 > y2) {
            SWAP(int,y1,y2);
        }
        for (size_t y = y1; y < y2; y++) {
            canvas_draw_pixel(canvas,x1,y,c);
        }
    }

}


int main() {
    Canvas canvas = canvas_new(100,100);
    
    canvas_clear(canvas,0x0000000);
    canvas_draw_line(canvas,canvas.w,canvas.h,0,0,0xFF00FF);
    canvas_draw_line(canvas,canvas.w,0,0,canvas.h,0xFFFFFF);
    canvas_draw_line(canvas,canvas.w / 2,0,canvas.w/2,canvas.h,0x0000FF);
    canvas_draw_line(canvas,0,canvas.h / 2,canvas.w/2,canvas.h / 2,0x555555);

    canvas_to_ppm(canvas,"output.ppm");


    canvas_destroy(canvas);
}