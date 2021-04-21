#include "datasets.h"
#include <stdio.h>
#include <math.h>


#define NUMBER_OF_TEST_POINTS 3000
#define NUMBER_OF_TRAINING_POINTS 3000
#define C1 1
#define C2 2
#define C3 3
#define C4 4

void open_files();
void close_files();
void generate_test_set_points();
void generate_training_set_points();
Point make_new_S1_point();
void choose_point_category(Point *new_point);
int is_inside_square(Point new_point, float x1_min, float x1_max, float x2_min, float x2_max);
void add_noise(Point *new_point);


int noisy_points_count = 0;
FILE *C1_file;
FILE *C2_file;
FILE *C3_file;
FILE *C4_file;
FILE *training_set_file;
FILE *test_set_file;


int main(void){

    srand(time(NULL));
    open_files();
    generate_test_set_points();
    generate_training_set_points();
    close_files();

    return 0;
}


void open_files(){

    if((C1_file = fopen("C1.dat", "w")) == NULL){
        printf("couldn't open 'C1.dat'\n");
        exit(1);
    }
    if((C2_file = fopen("C2.dat", "w")) == NULL){
        printf("couldn't open 'C2.dat'\n");
        exit(1);
    }
    if((C3_file = fopen("C3.dat", "w")) == NULL){
        printf("couldn't open 'C3.dat'\n");
        exit(1);
    }
    if((C4_file = fopen("C4.dat", "w")) == NULL){
        printf("couldn't open 'C4.dat'\n");
        exit(1);
    }
    if((training_set_file = fopen("training_set.txt", "w")) == NULL){
        printf("couldn't open 'training_set.txt'\n");
        exit(1);
    }
    if((test_set_file = fopen("test_set.txt", "w")) == NULL){
        printf("couldn't open 'test_set.txt'\n");
        exit(1);
    }
}


void close_files(){

    fclose(C1_file);
    fclose(C2_file);
    fclose(C3_file);
    fclose(C4_file);
    fclose(training_set_file);
    fclose(test_set_file);
}


void generate_test_set_points(){

    printf("generating test set...\n");
    for(int i=0; i<NUMBER_OF_TEST_POINTS; i++){
        Point new_point = make_new_S1_point();
        fprintf(test_set_file, "%f\t%f\t%d\n", new_point.x1, new_point.x2, new_point.category);
        if(new_point.category == C1) fprintf(C1_file, "%f\t%f\n", new_point.x1, new_point.x2);
        else if(new_point.category == C2) fprintf(C2_file, "%f\t%f\n", new_point.x1, new_point.x2);
        else if(new_point.category == C3) fprintf(C3_file, "%f\t%f\n", new_point.x1, new_point.x2);
        else if(new_point.category == C4) fprintf(C4_file, "%f\t%f\n", new_point.x1, new_point.x2);
    }
    printf("done!\n");
}


void generate_training_set_points(){

    printf("generating training set...\n");
    for(int i=0; i<NUMBER_OF_TRAINING_POINTS; i++){
        Point new_point = make_new_S1_point();
        if(new_point.category != C4){
            add_noise(&new_point);
        }
        fprintf(training_set_file, "%f\t%f\t%d\n", new_point.x1, new_point.x2, new_point.category);
        if(new_point.category == C1) fprintf(C1_file, "%f\t%f\n", new_point.x1, new_point.x2);
        else if(new_point.category == C2) fprintf(C2_file, "%f\t%f\n", new_point.x1, new_point.x2);
        else if(new_point.category == C3) fprintf(C3_file, "%f\t%f\n", new_point.x1, new_point.x2);
        else if(new_point.category == C4) fprintf(C4_file, "%f\t%f\n", new_point.x1, new_point.x2);
    }
    printf("done!\n");
    printf("the noise percentage of the training set is %f%%\n", (float) noisy_points_count / (float) NUMBER_OF_TRAINING_POINTS * 100);
}


Point make_new_S1_point(){
    Point new_point;

    new_point.x1 = choose_random_float_in_range(-1, 1);
    new_point.x2 = choose_random_float_in_range(-1, 1);
    choose_point_category(&new_point);

    return new_point;
}


void choose_point_category(Point *new_point){

    if(pow(new_point->x1, 2) + pow(new_point->x2, 2) < 0.25) new_point->category = C1;
    else if(is_inside_square(*new_point, -1, -0.4, -1, -0.4)) new_point->category = C2;
    else if(is_inside_square(*new_point, 0.4, 1, 0.4, 1)) new_point->category = C2;
    else if(is_inside_square(*new_point, -1, -0.4, 0.4, 1)) new_point->category = C3;
    else if(is_inside_square(*new_point, 0.4, 1, -1, -0.4)) new_point->category = C3;
    else new_point->category = C4;
}


int is_inside_square(Point new_point, float x1_min, float x1_max, float x2_min, float x2_max){

    if(new_point.x1 < x1_min || new_point.x1 > x1_max || new_point.x2 < x2_min || new_point.x2 > x2_max) return 0;
    else return 1;
}


void add_noise(Point *new_point){
    float probability = choose_random_float_in_range(0, 1);

    if(probability < 0.1) {
        new_point->category = C4;  
        noisy_points_count++;
    }
}