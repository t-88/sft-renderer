#ifndef SFT_MATH_H 
#define SFT_MATH_H 

typedef double sftr_Matrix[4][4];


typedef struct sftr_Vector4 {
    double x , y, z ,w;
} sftr_Vector4;



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
#define sftr_vector_zero(v) (sftr_Vector4) {0,0,0,0}
#define sftr_vector_print(v) printf("%f %f %f %f\n",v.x,v.y,v.z,v.w)

void sftr_matrix_mult_matrix(sftr_Matrix m1,sftr_Matrix m2,sftr_Matrix m3) { 
    sftr_matrix_zero(m3);
    for (size_t j = 0; j < 4; j++)
        for (size_t i = 0; i < 4; i++) 
            for (size_t k = 0; k < 4; k++)
                m3[j][i] += m1[j][k] * m2[k][i]; 
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

#define sftr_vector_to_arr(v) {v.x,v.y,v.z,v.w}
#define sftr_vector_from_arr(arr) (sftr_Vector4) {arr[0],arr[1],arr[2],arr[3]}

sftr_Vector4 sftr_matrix_mult_vector(sftr_Matrix m1,sftr_Vector4 v1) { 
    double arr_v[] = {0,0,0,0};
    double arr_v1[] = sftr_vector_to_arr(v1);

    for (size_t j = 0; j < 4; j++)
        for (size_t k = 0; k < 4; k++)
            arr_v[j] += m1[j][k] * arr_v1[k];

    sftr_Vector4 v2; 
    v2 = sftr_vector_from_arr(arr_v);
    return v2;
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
void sftr_matrix_mult_number(sftr_Matrix out,double val) {
    for (size_t j = 0; j < 4; j++) 
        for (size_t i = 0; i < 4; i++) 
            out[j][i] *= val;
}
void  sftr_matrix_inverse(sftr_Matrix in, sftr_Matrix out) {
    sftr_matrix_adjugate(in,out);
    double det = sftr_matrix_det(in);
    assert(det != 0 && "[Math Error] Cant inverse matrix, det = 0");
    sftr_matrix_mult_number(out,1/det);
}
 


#endif // SFT_MATH_H 