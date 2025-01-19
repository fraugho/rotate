#ifndef VEC3_H
#define VEC3_H

#include <stdio.h>
#include <math.h>

// Define the vec3 struct
typedef struct {
    double ele[3];
} vec3;

// Function to initialize a vec3
void vec3_init(vec3* v, double x, double y, double z) {
    v->ele[0] = x;
    v->ele[1] = y;
    v->ele[2] = z;
}

// Function to get x, y, z
double vec3_get_x(const vec3* v) { return v->ele[0]; }
double vec3_get_y(const vec3* v) { return v->ele[1]; }
double vec3_get_z(const vec3* v) { return v->ele[2]; }

// Function to calculate length squared
double vec3_length_squared(const vec3* v) {
    return v->ele[0] * v->ele[0] + v->ele[1] * v->ele[1] + v->ele[2] * v->ele[2];
}

// Function to calculate the magnitude
double vec3_get_magnitude(const vec3* v) {
    return sqrt(vec3_length_squared(v));
}

// Function to negate a vector
vec3 vec3_negate(const vec3* v) {
    vec3 result;
    vec3_init(&result, -v->ele[0], -v->ele[1], -v->ele[2]);
    return result;
}

// Arithmetic operations
vec3 vec3_add(const vec3 a, const vec3 b) {
    vec3 result;
    vec3_init(&result, a.ele[0] + b.ele[0], a.ele[1] + b.ele[1], a.ele[2] + b.ele[2]);
    return result;
}

vec3 vec3_sub(const vec3 a, const vec3 b) {
    vec3 result;
    vec3_init(&result, a.ele[0] - b.ele[0], a.ele[1] - b.ele[1], a.ele[2] - b.ele[2]);
    return result;
}

vec3 vec3_mul(const vec3 a, const vec3 b) {
    vec3 result;
    vec3_init(&result, a.ele[0] * b.ele[0], a.ele[1] * b.ele[1], a.ele[2] * b.ele[2]);
    return result;
}

vec3 vec3_scalar_mul(const vec3 v, double scalar) {
    vec3 result;
    vec3_init(&result, v.ele[0] * scalar, v.ele[1] * scalar, v.ele[2] * scalar);
    return result;
}

vec3 vec3_scalar_div(const vec3 v, double divisor) {
    return vec3_scalar_mul(v, 1.0 / divisor);
}

// Dot product
double vec3_dot(const vec3* a, const vec3* b) {
    return a->ele[0] * b->ele[0] + a->ele[1] * b->ele[1] + a->ele[2] * b->ele[2];
}

// Cross product
vec3 vec3_cross(const vec3* a, const vec3* b) {
    vec3 result;
    vec3_init(&result, 
        a->ele[1] * b->ele[2] - a->ele[2] * b->ele[1],
        a->ele[2] * b->ele[0] - a->ele[0] * b->ele[2],
        a->ele[0] * b->ele[1] - a->ele[1] * b->ele[0]
    );
    return result;
}

// Normalize the vector
vec3 vec3_unit_vector(const vec3* v) {
    return vec3_scalar_div(*v, vec3_get_magnitude(v));
}

// Print a vec3
void vec3_print(const vec3* v) {
    printf("%lf %lf %lf\n", v->ele[0], v->ele[1], v->ele[2]);
}

#endif
