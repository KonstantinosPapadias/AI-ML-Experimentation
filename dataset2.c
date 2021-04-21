#include "datasets.h"
#include <stdio.h>

#define NUMBER_OF_POINTS 150


Point make_new_S2_point(float lower_x1, float upper_x1, float lower_x2, float upper_x2);


int main(void){
    Point new_points[6];
    FILE *new_file = fopen("dataset2.dat", "w");
    FILE *s2_file = fopen("S2.txt","w");
    srand(time(NULL));

    printf("generating points for set 2...\n");
    for(int i=0; i<NUMBER_OF_POINTS; i++){
        new_points[0] = make_new_S2_point(0.75, 1.25, 0.75, 1.25);
        new_points[1] = make_new_S2_point(0, 0.5, 0, 0.5);
        new_points[2] = make_new_S2_point(0, 0.5, 1.5, 2);
        new_points[3] = make_new_S2_point(1.5, 2, 0, 0.5);
        new_points[4] = make_new_S2_point(1.5, 2, 1.5, 2);
        new_points[5] = make_new_S2_point(0, 2, 0, 2);

        for(int i=0; i<6; i++){
            fprintf(s2_file, "%f\t%f\n", new_points[i].x1, new_points[i].x2);
            fprintf(new_file, "%f\t%f\n", new_points[i].x1, new_points[i].x2);
        }
    }
    printf("done!\n");

    return 0;
}


Point make_new_S2_point(float lower_x1, float upper_x1, float lower_x2, float upper_x2){
    Point new_point;

    new_point.x1 = choose_random_float_in_range(lower_x1, upper_x1); 
    new_point.x2 = choose_random_float_in_range(lower_x2, upper_x2);

    return new_point;
}
