#ifndef SFT_RENDERER_H
#define SFT_RENDERER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>





#define HexToColor(color) (sftr_Color) { \
                            .r = ((color) >> 8 * 3) & 0xFF, \
                            .g = ((color) >> 8 * 2) & 0xFF, \
                            .b = ((color) >> 8 * 1) & 0xFF, \
                            .a = ((color) >> 8 * 0) & 0xFF  \
                         }

// #define canvas_draw_pixel(canvas,x,y,c) canvas.pixels[(x) + (y) * canvas.w].color = canvas_hex_to_color((c)) 
#define sftr_SWAP(type,a,b) { type t = a; a = b; b = t;  } 
const double sftr_PI = 3.14159265358979311599796346854;
#define sftr_to_radians(angle) angle * sftr_PI / 180;




typedef  unsigned int sftr_Int32;

typedef struct sftr_String {
    char* val;
    size_t count;
} sftr_String;




typedef struct sftr_Color {
    unsigned char r , g , b , a;

} sftr_Color;
typedef struct Pixel {
    sftr_Color color;
} Pixel;

typedef struct sftr_Canvas {
    int w , h;
    Pixel* pixels; 
} sftr_Canvas;


sftr_String string_from_int(int val) {
    // generate a string from integer, need to clean mem ur self

    sftr_String str;
    if(val == 0) {
        str.count = 1;
        str.val = (char*) malloc(str.count);
        str.val[0] = '0';
    } else {
        str.count = (int) log10(val) + 1;
        str.val = (char*) malloc(str.count);
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
void string_destroy(sftr_String* str) {
    free(str->val);
}

sftr_Canvas canvas_new(int w, int h) {
    sftr_Canvas canvas = {.w = w, .h = h};
    canvas.pixels = (Pixel*) malloc(sizeof(Pixel) * w * h);
    for (size_t i = 0; i < canvas.w * canvas.h; i++) {
        canvas.pixels[i].color.r = 0;
        canvas.pixels[i].color.g = 0;
        canvas.pixels[i].color.b = 0;
    }
    
    return canvas;
}
void canvas_destroy(sftr_Canvas canvas) {
    free(canvas.pixels);
}

sftr_Color canvas_hex_to_color(sftr_Int32 color) {
    if(color <= 0xFFFFFF) {
        color <<= 8;
        color |= 0xFF;
    } 
    return HexToColor(color);
}
void canvas_clear(sftr_Canvas canvas,sftr_Int32 color) {
    sftr_Color c = canvas_hex_to_color(color); 
    for (size_t y = 0; y < canvas.h; y++)
        for (size_t x = 0; x < canvas.w; x++) {
            canvas.pixels[x + y * canvas.w].color = c;
        }
}

int canvas_to_ppm(sftr_Canvas canvas,const char* file_name) {
    sftr_String str_w;
    sftr_String str_h;

    FILE* f = fopen(file_name,"wb");
    if(f == NULL)  {
        goto canvas_to_ppm_save_exit;
    }

    fwrite("P3\n",1,3,f);

    str_w = string_from_int(canvas.w);
    str_h = string_from_int(canvas.h);
    fwrite(str_w.val,1,str_w.count,f);
    string_destroy(&str_w);
    fwrite(" ",1,1,f);
    fwrite(str_h.val,1,str_h.count,f);
    string_destroy(&str_h);
    fwrite(" ",1,1,f);
    fwrite("255",1,3,f);
    fwrite("\n",1,1,f);


    for (size_t i = 0; i < canvas.h * canvas.w; i++) {
            sftr_String r = string_from_int(canvas.pixels[i].color.r);            
            sftr_String g = string_from_int(canvas.pixels[i].color.g);            
            sftr_String b = string_from_int(canvas.pixels[i].color.b);            

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

void canvas_draw_pixel(sftr_Canvas canvas,int x,int y,sftr_Int32 c) {
    if(0 <= x  && x < canvas.w &&  0 <= y && y < canvas.h) {
        canvas.pixels[x + y * canvas.w].color = canvas_hex_to_color(c);
    }
} 


//TODO: Understand this shit plz
void canvas_draw_line(sftr_Canvas canvas,int x1,int y1, int x2,int y2,sftr_Int32 c) {
    // bresenham algorithm
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int slope = dy > dx;

    if (slope) {
        sftr_SWAP(int,x1, y1);
        sftr_SWAP(int,x2, y2);
    }

    if (x1 > x2) {
        sftr_SWAP(int,x1, x2);
        sftr_SWAP(int,y1, y2);
    }

    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    int err = dx / 2;
    int y = y1;
    int ystep = y1 < y2 ? 1 : -1;

    for (int x = x1; x <= x2; x++) {
        if(slope) {
            canvas_draw_pixel(canvas,y,x,c);
        } else {
            canvas_draw_pixel(canvas,x,y,c);
        }
        err -= dy;

        if (err < 0) {
            y += ystep;
            err += dx;
        }
    }
    


}


void canvas_draw_rect(sftr_Canvas canvas, int x, int y, int w , int h, sftr_Int32 color) {
    assert(w >= 0 && "[Error] in canvas_draw_rect: rect width cant be negative");
    assert(h >= 0 && "[Error] in canvas_draw_rect: rect height cant be negative");

    if(x > canvas.w || y > canvas.h) {
        return;
    }

    if(x + w > canvas.w) { w = canvas.w - x ; }
    if(y + h > canvas.h) { h = canvas.h - y ; }
    if(x < 0) { x = 0; }
    if(y < 0) { y = 0; }




    for (int j = 0; j < h; j++)
        for (int i = 0; i < w; i++) {
            canvas_draw_pixel(canvas, i + x, j + y,color);
        }
    

}




void canvas_draw_circle(sftr_Canvas canvas,int x,int y,int r , sftr_Int32 color) {
    if(r <= 0) {
        assert(0 && "[Error] canvas_draw_circle: cant have zero or nigative radius");
        return;
    }
    for (int j = -r; j < r; j++) {
        for (int i =  -r; i < r; i++) { 
            if(i*i + j*j <= r*r) {
                canvas_draw_pixel(canvas,x+i,y+j,color);
            }
        }
    }
    
}

// TODO: make better
void canvas_draw_traingle(sftr_Canvas canvas,int x1,int y1,int x2,int y2,int x3,int y3) {
    // top y
    if(y1 > y2) {sftr_SWAP(int,y2,y1);sftr_SWAP(int,x2,x1);}
    if(y1 > y3) {sftr_SWAP(int,y1,y3);sftr_SWAP(int,x1,x3);}
    // min y
    if(y2 > y3) {sftr_SWAP(int,y2,y3);sftr_SWAP(int,x2,x3);}


    float d12 = (float)(x1 - x2) / (y1 - y2);
    float d13 = (float)(x1 - x3) / (y1 - y3);

    float b12 = y1 - x1 / d12;
    float b13 = y1 - x1 / d13;

    for (int y = y1; y < y2; y++) {
        int x12 = x1;
        if(d12 != 0) {
            x12 = (y - b12) * d12;
        }
        int x13 = x1;
        if(d13 != 0) {
            x13 = (y - b13) * d13;
        }
        canvas_draw_line(canvas,x12,y,x13,y,0xFFFFFF);
    }

    float d32 = (float)(x3 - x2) / (y3 - y2);
    float b32 = y2 -  x2 / d32; 
    for (int y = y2; y < y3; y++) {
        int x32 = x1;
        if(d32 != 0) {
            x32 = (y - b32) * d32;
        }
        int x13 = x1;
        if(d13 != 0) {
            x13 = (y - b13) * d13;
        }
        canvas_draw_line(canvas,x13,y,x32,y,0xFFFFFF);
    }
}


#endif