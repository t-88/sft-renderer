#include <stdlib.h>
#include <time.h>


#include "raylib_backend.h"

#include "stb_image.h"


// u need to define it last
#define SFTR_RENDERER_IMPL
#include "sft_renderer.h"

#define sftr_color_black (sftr_Color) {0,0,0,0xFF};
#define sftr_ColorToHex(color) (color).r << 16 | (color).g << 8 | (color).b << 0
#define clamp(type) type sftr_clamp_##type(type val,type min,type max) {if (val > max) {val = max;} else if(val < min) {val = min;} return val; }
clamp(int);
clamp(float);
clamp(double);


float sftr_rand_float() {
    return (float) rand() / RAND_MAX;
}
sftr_Vector4 sftr_vector_random_unit() {
    sftr_Vector4 vec;
    // while (true) {
        vec = (sftr_Vector4){.x = sftr_rand_float() * 2 - 1,.y = sftr_rand_float() * 2 - 1,.z = sftr_rand_float() * 2 - 1};
        // if(sftr_vector_length_sqred(vec) < 1) {
            // break;
        // }
    // }
    return sftr_vector_normalize(vec);
}
bool sftr_vector_is_zero(sftr_Vector4 vec) {
    float near_zero = 1e-8;
    return fabs(vec.x) <= near_zero && fabs(vec.y) <= near_zero && fabs(vec.z) <= near_zero;
}
sftr_Vector4 sftr_vector_reflect(sftr_Vector4 normal,sftr_Vector4 vec) {
    return sftr_vector_sub(vec,sftr_vector_multn(normal,2 * sftr_vector_dot(vec,normal)));
}



typedef struct sftr_Ray {
    sftr_Vector4 orig;
    sftr_Vector4 dir;
} sftr_Ray;
sftr_Vector4 sftr_ray_at(sftr_Ray ray, double t) {
    return (sftr_Vector4) {
        ray.orig.x + t * ray.dir.x,
        ray.orig.y + t * ray.dir.y,
        ray.orig.z + t * ray.dir.z
    };
}

typedef enum MaterialType {
    Lambertian = 0,
    Metal,
} MaterialType;
typedef struct Material_t {
    MaterialType type;
    sftr_Vector4 attenuation;
    double fuzz;
} Material_t;



typedef struct sftr_HitRecord {
    double t;
    sftr_Vector4 normal;
    sftr_Vector4 point;
    bool front_face;
    Material_t mat;
} sftr_HitRecord;
void sftr_ray_set_face_normal(sftr_HitRecord* record,sftr_Ray ray,sftr_Vector4 outward_normal) {
    // NOTE: expecting normal to be a unit vector
    record->front_face = sftr_vector_dot(ray.dir,outward_normal) < 0;
    record->normal = record->front_face ? outward_normal : sftr_vector_multn(outward_normal,-1);
}

bool ray_material_lambertian(sftr_Ray* ray,sftr_HitRecord* rec,sftr_Vector4* color,sftr_Ray* scatter)  {
    (*scatter) = (sftr_Ray) {
        .orig = rec->point,
        .dir =  sftr_vector_add(rec->normal,sftr_vector_random_unit()),
    };
    (*color) = rec->mat.attenuation;
    return true;
}
bool ray_material_metal(sftr_Ray* ray,sftr_HitRecord* rec,sftr_Vector4* color,sftr_Ray* scatter)  {
    (*scatter) = (sftr_Ray) {
        .orig = rec->point,
        .dir =  sftr_vector_add(
                    sftr_vector_reflect(rec->normal,sftr_vector_normalize(ray->dir)),
                    sftr_vector_multn(sftr_vector_random_unit(),rec->mat.fuzz)
                ),
    };
    (*color) = (sftr_Vector4){0.9f,0.9f,0.9f}; 
    return sftr_vector_dot(rec->normal,scatter->dir) > 0;
}
bool ray_material_apply(sftr_HitRecord* rec,sftr_Ray* ray,sftr_Vector4* color,sftr_Ray* scatter) {
    switch (rec->mat.type) {
        case Lambertian:
            return ray_material_lambertian(ray,rec,color,scatter);
        break;
        case Metal: 
            return ray_material_metal(ray,rec,color,scatter);
        break;
        default:
            assert(false && "Unexpected MaterialType");
            break;
    }
    return false;
}




typedef struct sftr_Ray_Shpere sftr_Ray_Shpere;
struct sftr_Ray_Shpere {
    sftr_Vector4 orig;
    float radius;
    bool (*hit)(sftr_Ray_Shpere* shpere,sftr_Ray ray,float t_min,float t_max,sftr_HitRecord* hit);
    Material_t mat;
};

bool ray_hit_shpere(sftr_Ray_Shpere* shpere,sftr_Ray ray,float t_min,float t_max,sftr_HitRecord* hit);
sftr_Ray_Shpere sftr_ray_create_shpere(sftr_Vector4 orig,float r,Material_t mat) {
    return (sftr_Ray_Shpere) {
        .orig = orig,
        .radius = r,
        .hit = ray_hit_shpere,
        .mat = mat,
    };
}

bool ray_hit_shpere(sftr_Ray_Shpere* shpere,sftr_Ray ray,float t_min,float t_max,sftr_HitRecord* hit) {
    sftr_Vector4 oc = sftr_vector_sub(ray.orig,shpere->orig); 
    
    float a = sftr_vector_length_sqred(ray.dir);
    float half_b = sftr_vector_dot(ray.dir,oc);
    float c = sftr_vector_length_sqred(oc) - shpere->radius * shpere->radius;
    
    
    float delta = half_b*half_b -  a * c;
    float sqrt_delta = sqrt(delta);

    float root = (-half_b - sqrt_delta) / a;
    if(!(t_min < root && root < t_max)) {
        root = (-half_b + sqrt_delta) / a; 
        if(!(t_min < root && root < t_max)) {
            return false;
        }
    }

    hit->t = root;
    hit->point = sftr_ray_at(ray,root);
    sftr_ray_set_face_normal(
        hit,
        ray,
        sftr_vector_divn(
                        sftr_vector_sub(hit->point,shpere->orig),
                        shpere->radius
                    )
    );
    hit->mat = shpere->mat;
    return true; 
}

#define MAX_HITABLES 10
static int hitables_idx = 0; 
void* hitables[MAX_HITABLES];

void add_hitable(void* object) {
    if(hitables_idx >= MAX_HITABLES) {
        printf("[Error] couldn't add_hitable list out of range\n");
        return;
    }
    hitables[hitables_idx++] = object;
}
bool world_hit(sftr_Ray ray,double ray_min,double ray_max,sftr_HitRecord* record) {
    bool did_hit = false;
    sftr_HitRecord temp_record;

    for (int i = 0; i < hitables_idx; i++) {
        if(((sftr_Ray_Shpere*)hitables[i])->hit(hitables[i],ray,ray_min,ray_max,&temp_record)) {
            ray_max = temp_record.t;
            (*record) = temp_record;
            did_hit = true;
        }
    }


    return did_hit;
}




#define SAMPLE_COUNT 50
static const float one_over_sample_count = 1.f / SAMPLE_COUNT;

#define DISPLAY_LOADING 1
#define USE_RAYLIB 1
static const int max_depth = 5; 


sftr_Canvas canvas;
RayLibBackend raylib_backend;



float aspect_ratio = 16.f / 9.f;
int width, height;
float vw_w, vw_h;

sftr_Vector4 camera_pos;
sftr_Vector4 vec_u , vec_v;
sftr_Vector4 d_pixel_u , d_pixel_v;
sftr_Vector4 vw_top_left; 
sftr_Vector4 pixel_top_left; 


int iteration = 0;

void render();
int main(void) {
    srand(time(NULL));

    width = 1000;
    height = width / aspect_ratio; 
    canvas = canvas_new(width ,height);

    vw_h = 2.f;
    vw_w = vw_h * (float) width / height; 

    camera_pos = (sftr_Vector4){0,0,0};
    vec_u = (sftr_Vector4){vw_w,0,0};
    vec_v = (sftr_Vector4){0,-vw_h,0};

    d_pixel_u = (sftr_Vector4){vw_w / width,0,0};
    d_pixel_v = (sftr_Vector4){0,-vw_h / height,0};

    vw_top_left = (sftr_Vector4){-vec_u.x / 2,-vec_v.y / 2,-1};
    pixel_top_left = (sftr_Vector4){
                                    vw_top_left.x + 0.5 * d_pixel_u.x,
                                    vw_top_left.y + 0.5 * d_pixel_v.y,
                                    vw_top_left.z
                                    };


    sftr_Ray_Shpere shpere1 = sftr_ray_create_shpere((sftr_Vector4){0,0,-1},0.4,      (Material_t){.type=Metal     ,.attenuation = {0.5,0.5,0.5},.fuzz = 0.1f});
    sftr_Ray_Shpere shpere2 = sftr_ray_create_shpere((sftr_Vector4){0,-100.5f,-1},100,(Material_t){.type=Lambertian     ,.attenuation = {0.5,0.5,0.5}});
    sftr_Ray_Shpere shpere3 = sftr_ray_create_shpere((sftr_Vector4){1,0,-1},0.5,      (Material_t){.type=Lambertian     ,.attenuation = {0.5,0.5,0.5}});
    add_hitable(&shpere1);
    add_hitable(&shpere2);
    add_hitable(&shpere3);

    if(!USE_RAYLIB) {
        render();
    } else {
        raylib_backend.canvas = &canvas;
        raylib_backend.render = render;
        raylib_backend_run(&raylib_backend);
    }

    canvas_destroy(canvas);
    return 0;
}

sftr_Color sftr_color_interplate(sftr_Color start,sftr_Color end,float t) {
    return (sftr_Color) {
        start.r + (end.r - start.r) * t,
        start.g + (end.g - start.g) * t,
        start.b + (end.b - start.b) * t
    };
}


sftr_Vector4 sftr_random_on_hemisphere(sftr_Vector4 normal) {
    sftr_Vector4 vec = sftr_vector_random_unit(); 
    if(sftr_vector_dot(vec,normal) > 0) {
        return vec;
    }
    return sftr_vector_multn(vec,-1);
}

sftr_Color ray_color(sftr_Ray ray,int depth) {
    if (depth <= 0) {
        return sftr_color_black;
    }
    sftr_HitRecord hit;
    if(world_hit(ray,0.0001,INFINITY,&hit)) {
        sftr_Ray scattered;
        sftr_Vector4 attenuation;

        if(ray_material_apply(&hit,&ray,&attenuation,&scattered)) {
            sftr_Color out = ray_color(scattered,depth - 1);
            return (sftr_Color){out.r * attenuation.x,out.g * attenuation.y,out.b * attenuation.z,0xFF};
        } 
        return sftr_color_black;
    }

    sftr_Vector4 unit_dir = sftr_vector_normalize(ray.dir); 
    float val =  0.5 * (unit_dir.y + 1); 

    return sftr_color_interplate((sftr_Color) {255,255,255} , (sftr_Color) {255 * 0.5,255 * 0.7,255},val); 
}

void render()  {
    canvas_clear(canvas,0x101010);

    for (int y = 0; y < canvas.h; y++) {
#if DISPLAY_LOADING
        printf("\rLoading: %%%d",(int)((float)y/canvas.h * 100));
        fflush(stdout);
#endif
        for (int x = 0; x < canvas.w; x++) {

            sftr_Vector4 pixel_center = (sftr_Vector4) {pixel_top_left.x + x * d_pixel_u.x, pixel_top_left.y + y * d_pixel_v.y, pixel_top_left.z};

            sftr_Vector4 color_vec = {0};

            // get random samples to apply anti-aliasing
            for (int i = 0; i < SAMPLE_COUNT; i++) {
                sftr_Ray ray =
                    (sftr_Ray) {
                        .orig = camera_pos,
                        .dir = (sftr_Vector4) {
                            pixel_center.x + (sftr_rand_float() - 0.5) * d_pixel_u.x - camera_pos.x ,
                            pixel_center.y + (sftr_rand_float() - 0.5) * d_pixel_v.y - camera_pos.y ,
                            pixel_center.z - camera_pos.z
                        }
                }; 
                sftr_Color tmp_color = {0};
                tmp_color =  ray_color(ray,max_depth);
                color_vec.x += tmp_color.r;
                color_vec.y += tmp_color.g;
                color_vec.z += tmp_color.b;
            }

            color_vec.x *= one_over_sample_count;
            color_vec.y *= one_over_sample_count;
            color_vec.z *= one_over_sample_count;

            // gamma corrected
            sftr_Color color = {
                .r = sftr_clamp_float(color_vec.x,0,255.f),
                .g = sftr_clamp_float(color_vec.y,0,255.f),
                .b = sftr_clamp_float(color_vec.z,0,255.f),
                .a = 255,
            };

            
            canvas_draw_pixel(canvas,x,y,sftr_ColorToHex(color));
        }
    }
    printf("\n");


    
    char str_number[50];
    sprintf(str_number,"dev-imgs/raycaster_saved/%d.ppm",iteration++); 
    canvas_to_ppm(canvas,str_number);
}
