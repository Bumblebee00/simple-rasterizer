#include <stdio.h>
#include <stdlib.h>

// Include the stb_image_write header file
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "geometric_vectors.h"

#define WIDTH 200
#define HEIGHT 200

long double l = 1;
long double f = 1;

typedef struct Camera{
    Vector
        C, // camera position
        D, // camera looking direction
        U, // camera up direction
        R, // camera right direction
        C1;// top left corner of screen
    long double l, f; // they need to be substituted with angle aperture

} Camera;

typedef struct Color{
    unsigned int r, g, b; // 0-255
} Color;

Vector i = {1.,0.,0.};
Vector j = {0.,1.,0.};
Vector k = {0.,0.,1.};

Color red = {255,0,0};
Color green = {0,255,0};
Color blue = {0,0,255};
Color orange = {255,165,0};
Color lilla = {128,0,128};
Color white = {255,255,255};

// creates a camera from a C and D (assumes U is towards poositive y)
Camera create_camera(Vector C, Vector D);

// draws in the image img (array of rgb values) the point P, seen from camera cam
void draw_point(unsigned char *img, Vector P, Camera cam);

// draws in the image img (array of rgb values) the line from A to B, seen from camera cam
void draw_line(unsigned char *img, Vector A, Vector B, Camera cam);
long double line_thickness = 3;

// draws in the image img (array of rgb values) the triangle ABC, seen from camera cam
void draw_triangle(unsigned char *img, Vector A, Vector B, Vector C, Camera cam, Color c);

int main(int argc, char **argv){

    // create a random image
    unsigned char *img;

    // draws 150 frames
    for (int frame=0;frame<150; frame++){
        img = calloc(WIDTH * HEIGHT * 3, sizeof(unsigned char));
        // rotating camera
        Vector C = vec_sum(vec_scalar(k,4.), vec_sum(vec_scalar(i,5.*sin(frame/10.)), vec_scalar(j,5.*cos(frame/10.))));
        Vector D = vec_scalar(C, -f/mag(C)); // camera looks at the origin
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

        draw_triangle(img, creaete_vector(1.,-1.,1.), creaete_vector(-1.,1.,1.), creaete_vector(1.,1.,1.), cam, orange);
        draw_triangle(img, creaete_vector(1.,-1.,1.), creaete_vector(-1.,1.,1.), creaete_vector(-1.,-1.,1.), cam, lilla);

        // save frame
        char name[30];
        sprintf(name, "%d.png", frame);
        if (stbi_write_png(name, WIDTH, HEIGHT, 3, img, WIDTH * 3) != 0) {
        } else { printf("Failed to save the image\n"); }

        free(img);
    }

    return 0;
}


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

// utility function called when a pixel has to be manipulated
void manipulate_pixel(unsigned char *img, int x, int y, Color c){
    int index = (y*WIDTH + x)*3;
    // for (int i=0; i<3; i++){
    //     if (img[index+i] <= 127){ img[index+i]+=127; }
    //     else{ img[index+i]+=128; }
    // }
    img[index] = c.r;
    img[index+1] = c.g;
    img[index+2] = c.b;
}

// returns the 2d vector fo the point projected on camera plane
Vector project_point(Vector P, Camera cam){
    Vector v = vec_sub(P, cam.C);
    Vector S = vec_sum(cam.C,vec_scalar(v, square_mag(cam.D)/dot(cam.D,v))); // point on screen
    long double x_coord = dot(cam.R, vec_sub(S, cam.C1))*l*WIDTH;
    long double y_coord = dot(cam.U, vec_sub(S, cam.C1))*(-l)*HEIGHT;

    Vector S2d = creaete_vector(x_coord, y_coord, 0.);
    return S2d;
}

// draws in the image img (array of rgb values) the point P, seen from camera cam
void draw_point(unsigned char *img, Vector P, Camera cam){
    Vector S2d = project_point(P, cam);
    
    manipulate_pixel(img, roundl(S2d.y), roundl(S2d.y), white);
}

void draw_line(unsigned char *img, Vector A, Vector B, Camera cam){
    Vector A_2d = project_point(A, cam);
    Vector B_2d = project_point(B, cam);
    Vector BA_2d = vec_sub(A_2d,B_2d);

    Vector P_2d = creaete_vector(0.,0.,0.);
    Vector BP_2d;
    Vector AP_2d;
    long double dist = 0;
    for (int i=0; i<HEIGHT; i++){
        for (int j=0; j<WIDTH; j++){
            P_2d.x = j+0.5; P_2d.y = i+0.5;

            // calculate distance between point and segment on 2d plane of the image
            AP_2d = vec_sub(P_2d,A_2d);
            BP_2d = vec_sub(P_2d,B_2d);
            if(dot(AP_2d, BA_2d) > 0){
                dist = mag(AP_2d);
            } else if (dot(BP_2d, BA_2d) < 0){
                dist = mag(BP_2d);
            } else {
                dist = mag(vec_sub(BP_2d, vec_scalar(BA_2d, dot(BA_2d, BP_2d)/square_mag(BA_2d))));
            }

            if(dist<line_thickness){
                manipulate_pixel(img, j, i, white);
            }
        }
    }
}

void draw_triangle(unsigned char *img, Vector A, Vector B, Vector C, Camera cam, Color color){
    Vector A_2d = project_point(A, cam);
    Vector B_2d = project_point(B, cam);
    Vector C_2d = project_point(C, cam);

    Vector AB_2d = vec_sub(B_2d, A_2d);
    Vector AC_2d = vec_sub(C_2d, A_2d);

    Vector P_2d = creaete_vector(0.,0.,0.);
    Vector AP_2d;
    long double det, b, c;

    for (int i=0; i<HEIGHT; i++){
        for (int j=0; j<WIDTH; j++){
            P_2d.x = j+0.5; P_2d.y = i+0.5;
            AP_2d = vec_sub(P_2d,A_2d);

            // calculate if point in the triangle
            det = AB_2d.x*AC_2d.y - AB_2d.y*AC_2d.x;
            b = (AP_2d.x*AC_2d.y - AP_2d.y*AC_2d.x)/det;
            c = (AB_2d.x*AP_2d.y - AB_2d.y*AP_2d.x)/det;

            if(b>=0 && c>=0 && c<1-b){ manipulate_pixel(img, j, i, color);}
        }
    }
}