#include <math.h>

typedef struct Vector {
    long double x, y, z;
} Vector;

Vector creaete_vector(long double x, long double y, long double z){
    Vector v;
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
}

long double dot(Vector v, Vector w){
    return v.x*w.x + v.y*w.y + v.z*w.z;
}

long double square_mag(Vector v){
    return v.x*v.x + v.y*v.y + v.z*v.z;
}

long double mag(Vector v){
    return sqrt(square_mag(v));
}

Vector vec_sum(Vector v, Vector w){
    Vector result;
    result.x = v.x + w.x;
    result.y = v.y + w.y;
    result.z = v.z + w.z;
    return result;
}

Vector vec_sub(Vector v, Vector w){
    Vector result;
    result.x = v.x - w.x;
    result.y = v.y - w.y;
    result.z = v.z - w.z;
    return result;
}

Vector vec_scalar(Vector v, long double a){
    Vector result;
    result.x = v.x * a;
    result.y = v.y * a;
    result.z = v.z * a;
    return result;
}

Vector norm(Vector v){
    return vec_scalar(v, 1/mag(v));
}

Vector cross(Vector v, Vector w){
    Vector result;
    result.x = v.y*w.z - v.z*w.y;
    result.y = v.z*w.x - v.x*w.z;
    result.z = v.x*w.y - v.y*w.x;
    return result;   
}