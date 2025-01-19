#include <stdio.h>
#include <stdlib.h>

// Include the stb_image_write header file
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "geometric_vectors.h"

#define WIDTH 100
#define HEIGHT 100

long double l = 1;
long double f = 1;

void print_vector(Vector v){
    printf("(%Lf, %Lf, %Lf)",v.x,v.y,v.z);
}

typedef struct Camera{
    Vector
        C, // camera position
        D, // camera looking direction
        U, // camera up direction
        R, // camera right direction
        C1;// top left corner of screen
    long double l, f; // they need to be substituted with angle aperture

} Camera;

// creates a camera from a C and D (assumes U is towards poositive y)
Camera create_camera(Vector C, Vector D){
    Camera cam;

    cam.l = 1; cam.f = 1;

    cam.C = C; cam.D = D;
    long double N = sqrt(D.x*D.x + D.y*D.y);
    cam.U = norm(creaete_vector(-D.x*D.z/N, -D.y*D.z/N, N));
    cam.R = norm(cross(D, cam.U));
    cam.C1 = vec_sum(C, vec_sum(D, vec_scalar(vec_sub(cam.U, cam.R), l/2))); // C+D+(U-R)(l/2)

    return cam;
}

// draws in the image img (array of rgb values) the point P, seen from camera cam
void draw_point(unsigned char *img, Vector P, Camera cam){
    Vector v = vec_sub(P, cam.C);
    Vector S = vec_sum(cam.C,vec_scalar(v, square_mag(cam.D)/dot(cam.D,v))); // point on screen
    int x_coord = roundl(dot(cam.R, vec_sub(S, cam.C1))*l*WIDTH);
    int y_coord = roundl(dot(cam.U, vec_sub(S, cam.C1))*(-l)*HEIGHT);
    
    int index = (y_coord*WIDTH + x_coord)*3;
    for (int i=0; i<3; i++){
        if (img[index+i] <= 127){ img[index+i]+=127; }
        else{ img[index+i]+=128; }
    }
}

// brutally inefficient way of drawing a line
void draw_line(unsigned char *img, Vector A, Vector B, Camera cam){
    int n_points = mag(vec_sub(A,B))*WIDTH;

    for (int i=0; i<n_points; i++){
        draw_point(img, vec_sum(A, vec_scalar(vec_sub(B,A), (long double)i/n_points)), cam);
    }
}

int main(int argc, char **argv){
    unsigned char *img;

    Vector i = creaete_vector(1.,0.,0.);
    Vector j = creaete_vector(0.,1.,0.);
    Vector k = creaete_vector(0.,0.,1.);

    img = calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));

    int index = 0;
    for (int i=0; i<HEIGHT; i++){
        for (int j=0; j<WIDTH; j++){
            int c = rand() % 255;
            img[index] = c;
            img[index+1] = c;
            img[index+2] = c;
            index += 3;
        }
    }

    for (int frame=0;frame<200; frame++){

        Vector C = vec_sum(vec_scalar(k,4.), vec_sum(vec_scalar(i,5.*sin(frame/15.)), vec_scalar(j,5.*cos(frame/15.))));
        Vector D = vec_scalar(C, -f/mag(C)); // camera looking direction
        Camera cam = create_camera(C, D);
        
        // draw cube
        draw_line(img, creaete_vector(1.,1.,1.), creaete_vector(1.,-1.,1.), cam);
        draw_line(img, creaete_vector(1.,1.,1.), creaete_vector(-1.,1.,1.), cam);
        draw_line(img, creaete_vector(-1.,1.,1.), creaete_vector(-1.,-1.,1.), cam);
        draw_line(img, creaete_vector(1.,-1.,1.), creaete_vector(-1.,-1.,1.), cam);
        
        draw_line(img, creaete_vector(1.,1.,-1.), creaete_vector(1.,1.,1.), cam);
        draw_line(img, creaete_vector(1.,-1.,-1.), creaete_vector(1.,-1.,1.), cam);
        draw_line(img, creaete_vector(-1.,1.,-1.), creaete_vector(-1.,1.,1.), cam);
        draw_line(img, creaete_vector(-1.,-1.,-1.), creaete_vector(-1.,-1.,1.), cam);

        draw_line(img, creaete_vector(1.,1.,-1.), creaete_vector(1.,-1.,-1.), cam);
        draw_line(img, creaete_vector(1.,1.,-1.), creaete_vector(-1.,1.,-1.), cam);
        draw_line(img, creaete_vector(-1.,1.,-1.), creaete_vector(-1.,-1.,-1.), cam);
        draw_line(img, creaete_vector(1.,-1.,-1.), creaete_vector(-1.,-1.,-1.), cam);

        char name[30];
        sprintf(name, "%d.png", frame);
    
        if (stbi_write_png(name, WIDTH, HEIGHT, 3, img, WIDTH * 3) != 0) {
        } else { printf("Failed to save the image\n"); }
    }

    return 0;
}