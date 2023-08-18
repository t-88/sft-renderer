#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>


#ifndef SFT_RENDERER_H
#define SFT_RENDERER_H


// meth macros
#define sftr_matrix_det_3x3(a00,a01,a02,a10,a11,a12,a20,a21,a22) (double)((a00*a11*a22+a01*a12*a20+a02*a10*a21) - \
                                                                 (a02*a11*a20+a00*a12*a21+a01*a10*a22))
#define sftr_matrix_det_2x2(a00,a01,a10,a11) (double)(a00*a11 - a01 * a11)

#define sftr_matrix_fill(m,a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15) do {\
    m[0][0] = a0; m[0][1] = a1; m[0][2] =   a2; m[0][3] =  a3;\
    m[1][0] = a4; m[1][1] =  a5; m[1][2] =  a6; m[1][3] =  a7;\
    m[2][0] = a8; m[2][1] =  a9; m[2][2] =  a10; m[2][3] = a11;\
    m[3][0] = a12; m[3][1] = a13; m[3][2] = a14; m[3][3] = a15;\
} while (0);
#define sftr_vector_fill(v,a0,a1,a2,a3) do{ \
    v.x = a0; v.y = a1; v.z =   a2; v.w =  a3;\
} while (0);
#define sftr_vector_zero(v) (sftr_Vector4) {0,0,0,0}
#define sftr_vector_print(v) printf("%f %f %f %f\n",v.x,v.y,v.z,v.w)
#define sftr_vector_to_arr(v) {v.x,v.y,v.z,v.w}
#define sftr_vector_from_arr(arr) (sftr_Vector4) {arr[0],arr[1],arr[2],arr[3]}


#define sftr_to_radians(angle) angle * sftr_PI / 180
#define sftr_PI  ((double) 3.14159265358979311599796346854)

// auto generated functions
#define  sftr_min(type) type sftr_min_##type(type a,type b) { if(a > b) { return b; } return a;}
#define lerp(type)  double lerp_##type(type a, type b,type c) {return a + (b - a) * c;} 




// graphics macros
#define HexToColor(color) (sftr_Color) { \
                            .r = ((color) >> 8 * 3) & 0xFF, \
                            .g = ((color) >> 8 * 2) & 0xFF, \
                            .b = ((color) >> 8 * 1) & 0xFF, \
                            .a = ((color) >> 8 * 0) & 0xFF  \
                         }


// general macros
#define sftr_SWAP(type,a,b) { type t = a; a = b; b = t;  } 



typedef  unsigned int sftr_Int32;

typedef double sftr_Matrix[4][4];

typedef struct sftr_Vector4 {
    double x , y, z ,w;
} sftr_Vector4;

typedef struct sftr_Vector2 {
    double x , y;
} sftr_Vector2;


// should merge those togther maybe
// for filling traingle with colors
typedef struct sftr_Vertex {
    sftr_Vector4 pos;
    sftr_Vector4 color;
} sftr_Vertex;
// for texture mapping 
typedef struct sftr_TexVertex {
    sftr_Vector4 pos;
    sftr_Vector4 coord;
} sftr_TexVertex ;


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




// meth: header
void sftr_matrix_print(sftr_Matrix m);
void sftr_matrix_ident(sftr_Matrix m);
void sftr_matrix_zero(sftr_Matrix m);
void sftr_matrix_transpose(sftr_Matrix in, sftr_Matrix out);
void sftr_matrix_cofactor(sftr_Matrix in, sftr_Matrix out);
void sftr_matrix_adjugate(sftr_Matrix in, sftr_Matrix out);
double sftr_matrix_det(sftr_Matrix in);
void  sftr_matrix_inverse(sftr_Matrix in, sftr_Matrix out);
void sftr_matrix_copy(sftr_Matrix in, sftr_Matrix out);
void sftr_matrix_invert_gauss(sftr_Matrix in,sftr_Matrix out);

void sftr_matrix_mult_matrix(sftr_Matrix m1,sftr_Matrix m2,sftr_Matrix m3); 
sftr_Vector4 sftr_matrix_mult_vector(sftr_Matrix m_in,sftr_Vector4 v_in); 
void sftr_matrix_mult_number(sftr_Matrix out,double val);

void sftr_matrix_rowop_swap(sftr_Matrix in,int src,int dest);
void sftr_matrix_rowop_mult(sftr_Matrix in,int row,double val); 
void sftr_matrix_rowop_add(sftr_Matrix in,int src,int dest); 
void sftr_matrix_rowop_mult_add(sftr_Matrix in,int src,double val ,int dest); 

void sftr_matrix_scale(sftr_Vector4 v,sftr_Matrix m);
void sftr_matrix_screen_space(sftr_Matrix out,int w, int h);
void sftr_matrix_translate(sftr_Vector4 v,sftr_Matrix m);
void sftr_matrix_rotate_z(sftr_Matrix m,double angle);
void sftr_matrix_rotate_x(sftr_Matrix m,double angle);
void sftr_matrix_rotate_y(sftr_Matrix m,double angle);
 

//graphics header
int canvas_to_ppm(sftr_Canvas canvas,const char* file_name);

sftr_String string_from_int(int val);
void string_destroy(sftr_String* str);

sftr_Canvas canvas_new(int w, int h);
void canvas_destroy(sftr_Canvas canvas);
sftr_Color canvas_hex_to_color(sftr_Int32 color);


void canvas_clear(sftr_Canvas canvas,sftr_Int32 color);
void canvas_draw_pixel(sftr_Canvas canvas,int x,int y,sftr_Int32 c);
void canvas_draw_line(sftr_Canvas canvas,int x1,int y1, int x2,int y2,sftr_Int32 c);
void canvas_draw_rect(sftr_Canvas canvas, int x, int y, int w , int h, sftr_Int32 color);
void canvas_draw_circle(sftr_Canvas canvas,int x,int y,int r , sftr_Int32 color);
void canvas_draw_traingle(sftr_Canvas canvas,int x1,int y1,int x2,int y2,int x3,int y3,sftr_Int32 color);
void canvas_draw_traingle_lined(sftr_Canvas canvas,int x1,int y1,int x2,int y2,int x3,int y3,sftr_Int32 color);
void canvas_draw_bary_traingle(sftr_Canvas canvas,sftr_Vertex a,sftr_Vertex b,sftr_Vertex c);
void canvas_copy_img(sftr_Canvas canvas,unsigned char* img,int comp);




void sftr_barycentric_inter(sftr_Vector4 a,sftr_Vector4 b,sftr_Vector4 c,sftr_Vector4 p,double* w1,double* w2,double* w3);

#endif



#ifdef SFTR_RENDERER_IMPL

sftr_min(int);
sftr_min(double);
sftr_min(float);

lerp(int);
lerp(double);
lerp(float);

// matrix row ops
void sftr_matrix_rowop_swap(sftr_Matrix in,int src,int dest) {
    for (size_t i = 0; i < 4; i++) {
        sftr_SWAP(double,in[src][i],in[dest][i]);
    }
}
void sftr_matrix_rowop_mult(sftr_Matrix in,int row,double val) { 
    for (size_t i = 0; i < 4; i++) {
        in[row][i] *= val;
    }
}
void sftr_matrix_rowop_add(sftr_Matrix in,int src,int dest) { 
    for (size_t i = 0; i < 4; i++) {
        in[dest][i] += in[src][i];
    }
}
void sftr_matrix_rowop_mult_add(sftr_Matrix in,int src,double val ,int dest) { 
    for (int i = 0; i < 4; i++) {
        in[dest][i] += in[src][i] * val;
    }
}


// meth implmetation
// matrix helpers
void sftr_matrix_print(sftr_Matrix m) {
    for (size_t j = 0; j < 4; j++) {
        for (size_t i = 0; i < 4; i++)
            printf("%f ",m[j][i]);
        printf("\n");
    }
}
void sftr_matrix_ident(sftr_Matrix m) {
    for (size_t j = 0; j < 4; j++) 
        for (size_t i = 0; i < 4; i++) 
            if (i == j)
                m[j][i] = 1;
}
void sftr_matrix_zero(sftr_Matrix m) {
    for (size_t j = 0; j < 4; j++) 
        for (size_t i = 0; i < 4; i++) 
            m[j][i] = 0;
}
void sftr_matrix_transpose(sftr_Matrix in, sftr_Matrix out) {
    sftr_matrix_fill(
        out,
        in[0][0],in[1][0],in[2][0],in[3][0],
        in[0][1],in[1][1],in[2][1],in[3][1],
        in[0][2],in[1][2],in[2][2],in[3][2],
        in[0][3],in[1][3],in[2][3],in[3][3]);

}
void sftr_matrix_cofactor(sftr_Matrix in, sftr_Matrix out) {
    sftr_matrix_fill(
            out,
            sftr_matrix_det_3x3(in[1][1],in[1][2],in[1][3],in[2][1],in[2][2],in[2][3],in[3][1],in[3][2],in[3][3]),
            -sftr_matrix_det_3x3(in[1][0],in[1][2],in[1][3],in[2][0],in[2][2],in[2][3],in[3][0],in[3][2],in[3][3]),
            sftr_matrix_det_3x3(in[1][0],in[1][1],in[1][3],in[2][0],in[2][1],in[2][3],in[3][0],in[3][1],in[3][3]),
            -sftr_matrix_det_3x3(in[1][0],in[1][1],in[1][2],in[2][0],in[2][1],in[2][2],in[3][0],in[3][1],in[3][2]),

            -sftr_matrix_det_3x3(in[0][1],in[0][2],in[0][3],in[2][1],in[2][2],in[2][3],in[3][1],in[3][2],in[3][3]),
            sftr_matrix_det_3x3(in[0][0],in[0][2],in[0][3],in[2][0],in[2][2],in[2][3],in[3][0],in[3][2],in[3][3]),
            -sftr_matrix_det_3x3(in[0][0],in[0][1],in[0][3],in[2][0],in[2][1],in[2][3],in[3][0],in[3][1],in[3][3]),
            sftr_matrix_det_3x3(in[0][0],in[0][1],in[0][2],in[2][0],in[2][1],in[2][2],in[3][0],in[3][1],in[3][2]),

            sftr_matrix_det_3x3(in[0][1],in[0][2],in[0][3],in[1][1],in[1][2],in[1][3],in[3][1],in[3][2],in[3][3]),
            -sftr_matrix_det_3x3(in[0][0],in[0][2],in[0][3],in[1][0],in[1][2],in[1][3],in[3][0],in[3][2],in[3][3]),
            sftr_matrix_det_3x3(in[0][0],in[0][1],in[0][3],in[1][0],in[1][1],in[1][3],in[3][0],in[3][1],in[3][3]),
            -sftr_matrix_det_3x3(in[0][0],in[0][1],in[0][2],in[1][0],in[1][1],in[1][2],in[3][0],in[3][1],in[3][2]),

            -sftr_matrix_det_3x3(in[0][1],in[0][2],in[0][3],in[1][1],in[1][2],in[1][3],in[2][1],in[2][2],in[2][3]),
            sftr_matrix_det_3x3(in[0][0],in[0][2],in[0][3],in[1][0],in[1][2],in[1][3],in[2][0],in[2][2],in[2][3]),
            -sftr_matrix_det_3x3(in[0][0],in[0][1],in[0][3],in[1][0],in[1][1],in[1][3],in[2][0],in[2][1],in[2][3]),
            sftr_matrix_det_3x3(in[0][0],in[0][1],in[0][2],in[1][0],in[1][1],in[1][2],in[2][0],in[2][1],in[2][2])
    );
}
void sftr_matrix_adjugate(sftr_Matrix in, sftr_Matrix out) {
    sftr_Matrix tmp;
    sftr_matrix_cofactor(in,tmp);
    sftr_matrix_transpose(tmp,out);
}
double sftr_matrix_det(sftr_Matrix in) {
    double det;
    det = in[0][0] * sftr_matrix_det_3x3(
        in[1][1],in[1][2],in[1][3],
        in[2][1],in[2][2],in[2][3],
        in[3][1],in[3][2],in[3][3]
    );
    det -= in[0][1] * sftr_matrix_det_3x3(
        in[1][0],in[1][2],in[1][3],
        in[2][0],in[2][2],in[2][3],
        in[3][0],in[3][2],in[3][3]
    );
    det += in[0][2] * sftr_matrix_det_3x3(
        in[1][0],in[1][1],in[1][3],
        in[2][0],in[2][1],in[2][3],
        in[3][0],in[3][1],in[3][3]
    );
    det -= in[0][3] * sftr_matrix_det_3x3(
        in[1][0],in[1][1],in[1][2],
        in[2][0],in[2][1],in[2][2],
        in[3][0],in[3][1],in[3][2]
    );

    return det;
}
void  sftr_matrix_inverse(sftr_Matrix in, sftr_Matrix out) {
    sftr_matrix_adjugate(in,out);
    double det = sftr_matrix_det(in);
    assert(det != 0 && "[Math Error] Cant inverse matrix, det = 0");
    sftr_matrix_mult_number(out,1/det);
}
void sftr_matrix_copy(sftr_Matrix in, sftr_Matrix out) {
    for (size_t j = 0; j < 4; j++) {
        for (size_t i = 0; i < 4; i++) {
            out[j][i] = in[j][i];
        }
    }
}
void sftr_matrix_invert_gauss(sftr_Matrix in,sftr_Matrix out) {
    sftr_matrix_ident(out);
    sftr_Matrix cp;
    sftr_matrix_copy(in,cp);
    


    // check for pivot
    for (int i = 0; i < 4; i++) {
        if(cp[i][i] == 0) {
            int val = -1;
            int row;
            if(i < 3) {
                for (int j = i + 1; j < 4; j++) {
                    int m = abs(cp[j][i]); 
                    if(m > val) {
                        val = m;
                        row = j;
                    }
                }
            }
            if(i > 0) {
                for (int j = i - 1; j >= 0; j--) {
                    int m = abs(cp[j][i]); 
                    if(m > val) {
                        val = m;
                        row = j;
                    }
                }
            }

            if(val == 0) {
                assert(0 && "Matrix cannot be inverted");
            }
            sftr_matrix_rowop_swap(cp,i,row);
            sftr_matrix_rowop_swap(out,i,row);
        }
    }


    // zero out first section
    for (int j = 1; j < 4; j++) {
        for (int i = 0; i < j; i++) {
            if(cp[j][i] != 0) {

                double val = -cp[j][i]/cp[i][i];

                sftr_matrix_rowop_mult_add(cp, i,val,j);
                sftr_matrix_rowop_mult_add(out,i,val,j);
    
                assert(cp[j][i] == 0 && "WTF, this must be a zero");
            } 
        }
    }
    

    // one the pivot
    for (int j = 0; j < 4; j++) {
        double val = 1/cp[j][j];
        sftr_matrix_rowop_mult(cp,j,val);
        sftr_matrix_rowop_mult(out,j,val);
    }

    // zero out second section
    for (int j = 2; j >= 0; j--) {
        for (int i = 3; i > j; i--) {
            if(cp[j][i] != 0) {
                sftr_matrix_rowop_mult_add(out, i ,-cp[j][i],j);
            }
        }
    }
}



// matrix  mult
void sftr_matrix_mult_matrix(sftr_Matrix m1,sftr_Matrix m2,sftr_Matrix m3) { 
    sftr_matrix_zero(m3);
    for (size_t j = 0; j < 4; j++)
        for (size_t i = 0; i < 4; i++) 
            for (size_t k = 0; k < 4; k++)
                m3[j][i] += m1[j][k] * m2[k][i]; 
}
sftr_Vector4 sftr_matrix_mult_vector(sftr_Matrix m_in,sftr_Vector4 v_in) { 
    double arr_v[] = {0,0,0,0};
    double arr_v1[] = sftr_vector_to_arr(v_in);

    for (size_t j = 0; j < 4; j++)
        for (size_t k = 0; k < 4; k++)
            arr_v[j] += m_in[j][k] * arr_v1[k];

    sftr_Vector4 v2; 
    v2 = sftr_vector_from_arr(arr_v);
    return v2;
}
void sftr_matrix_mult_number(sftr_Matrix out,double val) {
    for (size_t j = 0; j < 4; j++) 
        for (size_t i = 0; i < 4; i++) 
            out[j][i] *= val;
}



// matrix tranformation
void sftr_matrix_scale(sftr_Vector4 v,sftr_Matrix m) {
    sftr_matrix_fill(
        m,
        v.x , 0 , 0 , 0,
        0 , v.y , 0 , 0,
        0 , 0 , v.z , 0,
        0 , 0 , 0 , 1
    );
}
void sftr_matrix_screen_space(sftr_Matrix out,int w, int h) {
    float w_2 = w/2; 
    float h_2 = h/2; 
    sftr_matrix_fill(
        out,
        w_2 , 0 , 0 , w_2,
        0 , -h_2 , 0 , h_2,
        0 , 0 , 1 , 0,
        0 , 0 , 0 , 1
    );
}
void sftr_matrix_translate(sftr_Vector4 v,sftr_Matrix m) {
    sftr_matrix_fill(
        m,
        1 , 0 , 0 , v.x,
        0 , 1 , 0 , v.y,
        0 , 0 , 1 , v.z,
        0 , 0 , 0 , 1
    );
}
void sftr_matrix_rotate_z(sftr_Matrix m,double angle) {
    double c = cos(angle);
    double s = sin(angle);
    sftr_matrix_fill(
        m,
        c , -s , 0 , 0,
        s , c  , 0 , 0,
        0 , 0  , 1 , 0,
        0 , 0  , 0 , 1
    );
}
void sftr_matrix_rotate_x(sftr_Matrix m,double angle) {
    double c = cos(angle);
    double s = sin(angle);
    sftr_matrix_fill(
        m,
        1 , 0 , 0, 0,
        0 , c , -s, 0,
        0 , s , c, 0,
        0 , 0  , 0 , 1
    );
}
void sftr_matrix_rotate_y(sftr_Matrix m,double angle) {
    double c = cos(angle);
    double s = sin(angle);
    sftr_matrix_fill(
        m,
        c , 0 , s , 0,
        0 ,  1 , 0 , 0,
        -s , 0  , c , 0,
        0 , 0  , 0 , 1
    );
}


// meth 
void sftr_barycentric_inter(sftr_Vector4 a,sftr_Vector4 b,sftr_Vector4 c,sftr_Vector4 p,double* w1,double* w2,double* w3) {
    /* 
        FROM: https://www.youtube.com/watch?v=HYAgJN3x4GA&ab_channel=SebastianLague
        Thx SebastianLague :)
    */
    double k = b.x-a.x;
    double det = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
    *w1 = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / det; 
    *w2 = ((b.y - a.y) * (p.x - c.x) + (a.x - c.y) * (p.y - c.y)) / det;  
    *w3 =   1 - *w1 - *w2;
}




// strings
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

// graphics
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

// saving
int canvas_to_ppm(sftr_Canvas canvas,const char* file_name) {
    // writes the canvas to ppm
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


//graphics draw ops
void canvas_clear(sftr_Canvas canvas,sftr_Int32 color) {
    sftr_Color c = canvas_hex_to_color(color); 
    for (size_t y = 0; y < canvas.h; y++)
        for (size_t x = 0; x < canvas.w; x++) {
            canvas.pixels[x + y * canvas.w].color = c;
        }
}
void canvas_draw_pixel(sftr_Canvas canvas,int x,int y,sftr_Int32 c) {
    if(0 <= x  && x < canvas.w &&  0 <= y && y < canvas.h) {
        canvas.pixels[x + y * canvas.w].color = canvas_hex_to_color(c);
    }
} 
void canvas_draw_line(sftr_Canvas canvas,int x0,int y0, int x1,int y1,sftr_Int32 color) {

#if 0
    // still dont understand wtf is going on 
    // check out https://zingl.github.io/bresenham.html
    //TODO: implement it from scratch then simplfy

    int dx = abs(x1-x0),  sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1-y0),  sy = y0 < y1 ? 1 : -1; 
        
    int err = (dx>dy ? dx : -dy)/2, e2;

    for(;;){
        canvas_draw_pixel(canvas,x0,y0,color);

        if (x0==x1 && y0==y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }



#else    
    //simplest way to draw line
    // DDA?

    // lines leave pixels not filled bc i focus on one axis
    // we can find multiple values of y for one x 
    // and same for y
    // we have to calculate dx and dy and take the smallest one and draw accordingly



    int dx = x0 - x1;
    int dy = y0 - y1;

    // we figure out what axis has the most pixels 
    // then we draw a line using that axis
    // we get the equation using
    // P = P0 + (P1 - P0) * t


    // should use abs 
    if(abs(dx) > abs(dy)) {
        if (x0 > x1) {
            sftr_SWAP(int,x0,x1);
            sftr_SWAP(int,y0,y1);
        }

        float y = y0;
        float a = (float) dy / dx;

        for (int x = x0; x <= x1; x++) {
            canvas_draw_pixel(canvas,x,y,color);
            y = y + a;
        }
    } else {
        if (y0 > y1) {
            sftr_SWAP(int,x0,x1);
            sftr_SWAP(int,y0,y1);
        }


        float x = x0;
        float a = (float) dx / dy  ;

        for (int y = y0; y <= y1; y++) {
            canvas_draw_pixel(canvas,(int)x,y,color);
            x = x + a;
        }
    }    
#endif


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
void canvas_draw_traingle(sftr_Canvas canvas,int x1,int y1,int x2,int y2,int x3,int y3,sftr_Int32 color) {
    // TODO: make better
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
        canvas_draw_line(canvas,x12,y,x13,y,color);
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
        canvas_draw_line(canvas,x13,y,x32,y,color);
    }
}
void canvas_draw_traingle_lined(sftr_Canvas canvas,int x1,int y1,int x2,int y2,int x3,int y3,sftr_Int32 color) {
    // used to test line drawing
    canvas_draw_line(canvas,x1,y1,x2,y2,color);
    canvas_draw_line(canvas,x1,y1,x3,y3,color);
    canvas_draw_line(canvas,x2,y2,x3,y3,color);
}
void canvas_draw_bary_traingle(sftr_Canvas canvas,sftr_Vertex a,sftr_Vertex b,sftr_Vertex c) {
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

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            double w1,w2,w3;
            sftr_Vector4 p = {x,y,0,1};

            sftr_barycentric_inter(a.pos,b.pos,c.pos,p,&w1,&w2,&w3);
            
            if(w1 >= 0 && w2 >= 0 && w1 + w2 <= 1) {
                int r = sftr_min_double(255,a.color.x * w1) + sftr_min_double(255,a.color.y * w1) + sftr_min_double(255,a.color.z * w1);
                int g = sftr_min_double(255,b.color.x * w2) + sftr_min_double(255,b.color.y * w2) + sftr_min_double(255,b.color.z * w2);
                int b = sftr_min_double(255,c.color.x * w3) + sftr_min_double(255,c.color.y * w3) + sftr_min_double(255,c.color.z * w3);


                sftr_Int32 color = r << 16 | g << 8 | b << 0; 

                canvas_draw_pixel(canvas,p.x,p.y,color);
            }
        }
    }

}
void canvas_copy_img(sftr_Canvas canvas,unsigned char* img,int comp) {
    // only supports imgs with r, g ,b
    // canvas should be the same size as img
    // load image using stbi_load
    
    for (int y = 0; y < canvas.h; y++) {
        for (int x = 0; x < canvas.w; x++) {
            canvas.pixels[x + y * canvas.w].color.r = img[(x + y * canvas.w) * comp + 0];
            canvas.pixels[x + y * canvas.w].color.g = img[(x + y * canvas.w) * comp + 1];
            canvas.pixels[x + y * canvas.w].color.b = img[(x + y * canvas.w) * comp + 2];
            canvas.pixels[x + y * canvas.w].color.a = 0xFF;
        }
    }
}


#endif
