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

int line_thickness = 5;
void draw_line(unsigned char *img, Vector A, Vector B, Camera cam){
    Vector ac = vec_sub(A, cam.C);
    Vector Sa = vec_sum(cam.C,vec_scalar(ac, square_mag(cam.D)/dot(cam.D,ac)));
    long double x_coord_a = dot(cam.R, vec_sub(Sa, cam.C1))*l*WIDTH;
    long double y_coord_a = dot(cam.U, vec_sub(Sa, cam.C1))*(-l)*HEIGHT;
    Vector A_2d = creaete_vector(x_coord_a, y_coord_a, 0.);
    
    Vector bc = vec_sub(B, cam.C);
    Vector Sb = vec_sum(cam.C,vec_scalar(bc, square_mag(cam.D)/dot(cam.D,bc)));
    long double x_coord_b = dot(cam.R, vec_sub(Sb, cam.C1))*l*WIDTH;
    long double y_coord_b = dot(cam.U, vec_sub(Sb, cam.C1))*(-l)*HEIGHT;
    Vector B_2d = creaete_vector(x_coord_b, y_coord_b, 0.);
    
    Vector BA_2d = vec_sub(A_2d,B_2d);

    int index=0;
    Vector P_2d = creaete_vector(0.,0.,0.);
    Vector BP_2d;
    Vector AP_2d;
    long double dist = 0;
    for (int i=0; i<HEIGHT; i++){
        for (int j=0; j<WIDTH; j++){
            P_2d.x = j+0.5; P_2d.y = i+0.5;

            AP_2d = vec_sub(P_2d,A_2d);
            BP_2d = vec_sub(P_2d,B_2d);

            if(dot(AP_2d, BA_2d) > 0){
                dist = mag(AP_2d);
            } else if (dot(BP_2d, BA_2d) < 0){
                dist = mag(BP_2d);
            } else {
                dist = mag(vec_sub(BP_2d, vec_scalar(BA_2d, dot(BA_2d, BP_2d)/square_mag(BA_2d))));
            }

            if(roundl(dist)<line_thickness){
                img[index] = 255;
                img[index+1] = 255;
                img[index+2] = 255;
            }
            index += 3;
        }
    }
}

int main(int argc, char **argv){
    unsigned char *img;

    Vector i = creaete_vector(1.,0.,0.);
    Vector j = creaete_vector(0.,1.,0.);
    Vector k = creaete_vector(0.,0.,1.);


    // // randomize image
    // int index = 0;
    // for (int i=0; i<HEIGHT; i++){
    //     for (int j=0; j<WIDTH; j++){
    //         int c = rand() % 255;
    //         img[index] = c;
    //         img[index+1] = c;
    //         img[index+2] = c;
    //         index += 3;
    //     }
    // }

    for (int frame=0;frame<150; frame++){
        img = calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));

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

        free(img);
    }

    return 0;
}