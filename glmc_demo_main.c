#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <stdio.h>

#include <glmc.h>

int main(int argc, char **argv) {
    //glmc vector datatypes
    //vectors of float:
    t_vec3 position;
    t_vec4 color;

    //vectors of int
    t_ivec3 gridpos;
    t_ivec3 color_rgb;

    //set values manually
    position[0] = -10.0f;
    position[1] = 10.0f;
    position[2] = 20.0f;

    color[0] = 0.5f;
    color[1] = 0.5f;
    color[2] = 0.0f;
    color[3] = 1.0f;

    gridpos[0] = 4;
    gridpos[1] = 5;

    color_rgb[0] = 255;
    color_rgb[1] = 128;
    color_rgb[2] = 0;


    printf("position = (%f,%f,%f)\n", position[0], position[1], position[2]);
    printf("color = (%f,%f,%f,%f)\n", color[0], color[1], color[2], color[3]);
    printf("gridpos = (%d,%d)\n", gridpos[0], gridpos[1]);
    printf("color_rgb = (%d,%d,%d)\n", color_rgb[0], color_rgb[1], color_rgb[2]);
    printf("\n");


    //set values with helper  (this sets the same values as above)
    glmc_vec3_set(position, -10.0f, 10.0f, 20.0f);
    glmc_vec4_set(color, 0.5f, 0.5f, 0.0f, 1.0f);
    glmc_ivec2_set(gridpos, 4, 5);
    glmc_ivec3_set(color_rgb, 255, 128, 0);

    printf("position = (%f,%f,%f)\n", position[0], position[1], position[2]);
    printf("color = (%f,%f,%f,%f)\n", color[0], color[1], color[2], color[3]);
    printf("gridpos = (%d,%d)\n", gridpos[0], gridpos[1]);
    printf("color_rgb = (%d,%d,%d)\n", color_rgb[0], color_rgb[1], color_rgb[2]);
    printf("\n");

    //set values with initialiser
    t_vec3 ex1 = { 1.0f, 2.0f, 3.0f };
    t_vec3 ex2 = { 1.0f, 0.0f, 1.0f };
    printf("ex1 = (%f,%f,%f)\n", ex1[0], ex1[1], ex1[2]);
    printf("ex2 = (%f,%f,%f)\n", ex2[0], ex2[1], ex2[2]);
    printf("\n");

    //other helpers needed later
    t_vec3 position2 = { 1.0f, 50.0f, 100.0f };
    t_ivec3 gridpos2 = { -9, 80 };
    t_ivec3 color_rgb2 = { 128, 255, 255 };

    //assign values from 1 vector to another (arguments: dest, source)
    glmc_assign_vec3(position, position2);
    glmc_assign_ivec2(gridpos, gridpos2);

    printf("position =  (%f,%f,%f)\n", position[0], position[1], position[2]);
    printf("position2 = (%f,%f,%f)\n", position2[0], position2[1], position2[2]);
    printf("gridpos =  (%d,%d)\n", gridpos[0], gridpos[1]);
    printf("gridpos2 = (%d,%d)\n", gridpos2[0], gridpos2[1]);
    printf("\n");

    //check equality
    printf("color_rgb2 = (%d,%d,%d)\n", color_rgb2[0], color_rgb2[1], color_rgb2[2]);
    printf("\n");

    printf("position == position2 -> %d\n", glmc_equal_vec3(position, position2));
    printf("gridpos == gridpos2 -> %d\n", glmc_equal_ivec2(gridpos, gridpos2));
    printf("color_rgb == color_rgb2 -> %d\n", glmc_equal_ivec3(color_rgb, color_rgb2));
    printf("\n");

    //vec3 addition, substraction and scaler multiplication
    t_vec3 ex1plus2;
    t_vec3 ex1minus2;
    t_vec3 ex1times3;

    glmc_add_vec3(ex1, ex2, ex1plus2);
    glmc_sub_vec3(ex1, ex2, ex1minus2);
    glmc_vec3_multi(ex1, 3, ex1times3);

    printf("ex1 + ex2 = (%f,%f,%f)\n", ex1plus2[0], ex1plus2[1], ex1plus2[2]);
    printf("ex1 - ex2 = (%f,%f,%f)\n", ex1minus2[0], ex1minus2[1], ex1minus2[2]);
    printf("ex1 * 3   = (%f,%f,%f)\n", ex1times3[0], ex1times3[1], ex1times3[2]);
    printf("\n");
    
    return 0;
}
