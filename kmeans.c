#include "datasets.h"
#include "neural_network.h"
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <assert.h> 



#define NUMBER_OF_POINTS 900
#define M 5 // M is 2,3,4,5,6,7,10

FILE *s2_file;
FILE *cat0;
FILE *cat1;
FILE *cat2;
FILE *cat3;
FILE *cat4;
FILE *cat5;
FILE *cat6;
FILE *cat7;
FILE *cat8;
FILE *cat9;
FILE *cent;

int number_of_cluster_points[M] = {0};
int point_categories[NUMBER_OF_POINTS];
Point k_points[NUMBER_OF_POINTS]; // k_points.x1 , k_points.x2
Point centroids[M];
Point centroids_sums[M];
float total_error = 0;

void open_files(){
    
    if((s2_file = fopen("S2.txt", "r")) == NULL){
        printf("Couldn't open 'S2.txt, propably doesn't exist yet\n");
        printf("Remember to make the dataset first!\n");
        exit(1);
    }

    cat0 = fopen("kmeans_Team0.dat", "w");
    cat1 = fopen("kmeans_Team1.dat", "w");
    cat2 = fopen("kmeans_Team2.dat", "w");
    cat3 = fopen("kmeans_Team3.dat", "w");
    cat4 = fopen("kmeans_Team4.dat", "w");
    cat5 = fopen("kmeans_Team5.dat", "w");
    cat6 = fopen("kmeans_Team6.dat", "w");
    cat7 = fopen("kmeans_Team7.dat", "w");
    cat8 = fopen("kmeans_Team8.dat", "w");
    cat9 = fopen("kmeans_Team9.dat", "w");
    cent = fopen("kmeans_centroids.dat", "w");
}


void close_files(){

    fclose(s2_file);
    fclose(cat0);
    fclose(cat1);
    fclose(cat2);
    fclose(cat3);
    fclose(cat4);
    fclose(cat5);
    fclose(cat6);
    fclose(cat7);
    fclose(cat8);
    fclose(cat9);
    fclose(cent);
}


void load_dataset(){

    Point new_point;

    for(int i=0; i<NUMBER_OF_POINTS; i++){
        if(fscanf(s2_file, "%f", &new_point.x1) == EOF) break;
        if(fscanf(s2_file, "%f", &new_point.x2) == EOF) break;
        k_points[i] = new_point;
    }

}


float distance(Point point1, Point point2){

    return (powf(point1.x1 - point2.x1, 2) + powf(point1.x2 - point2.x2, 2));
    // Euclidean distance

}


void print_number_of_cluster_points(){
    for(int i=0; i<M; i++){
        printf("number_of_cluster_points[%d] - %d\n", i, number_of_cluster_points[i]);
    }
}

void print_k_points(){
    Point point;

    for(int i=0; i<NUMBER_OF_POINTS; i++){
        point = k_points[i];
        printf("(%0.2f,%0.2f)\n", point.x1, point.x2);
    }
}

int choose_point_cluster(float *dist, int length){
    float min_distance = dist[0], cluster = 0;
    
    for(int i=1; i<length; i++){
        if(dist[i] < min_distance){
            min_distance = dist[i];
            cluster = i;
        } 
    }

    return cluster;
}



int centroids_changed(Point *old_centroids, Point *new_centroids){
    float dif = 0;

    for(int i=0; i<M; i++){
        dif = distance(old_centroids[i], new_centroids[i]);
        if(dif > 0.0)
            return 1;
    }

    return 0;
}

void kmeans(){
    int i, j, index, t = 0;
    float dist[M];
    Point *clusters[M];
    Point old_centroids[M];
    Point new_point;

    for(i=0; i<M; i++){
        index = rand() % NUMBER_OF_POINTS;
        centroids[i] = k_points[index];
        //printf("centroids[%d] - (%0.2f,%0.2f)\n", i, centroids[i].x1, centroids[i].x2);
        number_of_cluster_points[i] = 1;
    } 

    while(1){
        for(i=0; i<NUMBER_OF_POINTS; i++){
            new_point = k_points[i];
            for(j=0; j<M; j++){
                dist[j] = distance(k_points[i], centroids[j]);
            }
            point_categories[i] = choose_point_cluster(dist, M);
            
            number_of_cluster_points[point_categories[i]]++;
            centroids_sums[point_categories[i]].x1 += new_point.x1;
            centroids_sums[point_categories[i]].x2 += new_point.x2;
            centroids[point_categories[i]].x1 = (float) centroids_sums[point_categories[i]].x1 / (float) number_of_cluster_points[point_categories[i]];
            centroids[point_categories[i]].x2 = (float) centroids_sums[point_categories[i]].x2 / (float) number_of_cluster_points[point_categories[i]];
        }

        if(!centroids_changed(old_centroids, centroids) || t> 10000){
            break;
        }

        // save old centroids
        for(i=0; i<M; i++){
            Point new_point;

            new_point.x1 = centroids[i].x1;
            new_point.x2 = centroids[i].x2;
            old_centroids[i] = new_point;
            //printf("centroids[%d] - (%0.2f,%0.2f)\n", i, new_point.x1, new_point.x2);
        }
        //printf("\n\n");

        t++;
        //printf("(t %d)\n", t);   
    }
}

float calculate_error(){
    float total_error = 0;
    for(int i=0; i<NUMBER_OF_POINTS; i++){
        total_error += distance(k_points[i], centroids[point_categories[i]]);
    }
    return total_error;
}


void write_files(){
    for (int i = 0; i < NUMBER_OF_POINTS; i++) {
        switch(point_categories[i]){
            case 0:
                fprintf(cat0, "%f\t%f\n", k_points[i].x1, k_points[i].x2);
                break;
            case 1:
                fprintf(cat1, "%f\t%f\n", k_points[i].x1, k_points[i].x2);
                break;
            case 2:
                fprintf(cat2, "%f\t%f\n", k_points[i].x1, k_points[i].x2);
                break;
            case 3:
                fprintf(cat3, "%f\t%f\n", k_points[i].x1, k_points[i].x2);
                break;
            case 4:
                fprintf(cat4, "%f\t%f\n", k_points[i].x1, k_points[i].x2);
                break;
            case 5:
                fprintf(cat5, "%f\t%f\n", k_points[i].x1, k_points[i].x2);
                break;
            case 6:
                fprintf(cat6, "%f\t%f\n", k_points[i].x1, k_points[i].x2);
                break;
            case 7:
                fprintf(cat7, "%f\t%f\n", k_points[i].x1, k_points[i].x2);
                break;
            case 8:
                fprintf(cat8, "%f\t%f\n", k_points[i].x1, k_points[i].x2);
                break;
            case 9:
                fprintf(cat9, "%f\t%f\n", k_points[i].x1, k_points[i].x2);
                break;
        }
    }

    for(int i=0; i<M; i++){
        //printf("(%0.2f,%0.2f)\n", centroids[i].x1, centroids[i].x2);
        fprintf(cent, "%f\t%f\n", centroids[i].x1, centroids[i].x2);
    }
}

int main(void){

    
    srand(time(NULL));
    open_files();
    load_dataset();
    printf("Running kmeans on the dataset...\n");
    kmeans();
    printf("done!\n");
    printf("\nTotal error -> %f\n", calculate_error());
    write_files();
    close_files();

    return 0;
}