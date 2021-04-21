#include "datasets.h"
#include "neural_network.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#define B 1
#define d 2
#define H1 8
#define H2 5
#define NUMBER_OF_TRAINING_POINTS 3000
#define NUMBER_OF_TEST_POINTS 3000
#define C1 1
#define C2 2
#define C3 3
#define C4 4
#define NUMBER_OF_CATEGORIES 4
#define ACTIVATION_FUNCTION_1(x) tanh(x)
#define ACTIVATION_FUNCTION_2(x) 1 / (1 + exp(-x))
#define ACTIVATION_FUNCTION_OUT(x) 1 / (1 + exp(-x))


void open_files();
void load_dataset();
void close_files();
void print_dataset();
void set_targets();
void forward_pass(Point point, float *y);
void initialize_weights();
void print_weights();
void calculate_neuron_input(Neuron *neuron, int number_of_neuron_inputs, Neuron previous_layer_neuron);
void backprop(Point point, float *target);
void print_partial_derivatives();
float calculate_point_error(float *target);
void reset_partial_derivatives();
void update_weights(float n);
void train_neural_network();
void test_neural_network();


FILE *training_set_file;
FILE *test_set_file;
FILE *correct_decisions_file;
FILE *wrong_decisions_file;
Point training_set[NUMBER_OF_TRAINING_POINTS];
Point test_set[NUMBER_OF_TEST_POINTS];
float training_targets[NUMBER_OF_TRAINING_POINTS][NUMBER_OF_CATEGORIES] = {0};
float test_targets[NUMBER_OF_TEST_POINTS][NUMBER_OF_CATEGORIES] = {0};
// + 1 gia thn eisodo ths polwshs
Neuron entry_layer_neurons[d + 1];
Neuron hidden_layer1_neurons[H1 + 1];
Neuron hidden_layer2_neurons[H2 + 1]; 
Neuron output_layer_neurons[NUMBER_OF_CATEGORIES + 1];
// + 1 gia thn polwsh kathe neurwna
float hidden_layer1_weights[H1 + 1][d + 1];
float hidden_layer2_weights[H2 + 1][H1 + 1];
float output_layer_weights[NUMBER_OF_CATEGORIES + 1][H2 + 1];
float hidden_layer1_partial_derivatives[H1 + 1][d + 1];
float hidden_layer2_partial_derivatives[H2 + 1][H1 + 1];
float output_layer_partial_derivatives[NUMBER_OF_CATEGORIES + 1][H2 + 1];


int main(void){
    
    srand(time(NULL));

    open_files();
    load_dataset();
    set_targets();
    initialize_weights();
    train_neural_network();
    test_neural_network();
    close_files(); 

    return 0;
}


void open_files(){
    
    if((training_set_file = fopen("training_set.txt", "r")) == NULL){
        printf("Couldn't open 'training_set.txt, propably doesn't exist yet\n");
        printf("Remember to make the dataset first!\n");
        exit(1);
    }
    if((test_set_file = fopen("test_set.txt", "r")) == NULL){
        printf("couldn't open 'test_set.txt, propably doesn't exist yet\n");
        printf("Remember to make the dataset first!\n");
        exit(1);
    }
    
    if((correct_decisions_file = fopen("correct_decisions.dat", "w")) == NULL){
        printf("Couldn't open 'correct_decisions.dat'\n");
        exit(1);
    }
    if((wrong_decisions_file = fopen("wrong_decisions.dat", "w")) == NULL){
        printf("Couldn't open 'wrong_decisions.dat'\n");
        exit(1);
    }
}


void close_files(){

    fclose(training_set_file);
    fclose(test_set_file);

    fclose(correct_decisions_file);
    fclose(wrong_decisions_file);
}


void load_dataset(){
    Point new_point;

    for(int i=0; i<NUMBER_OF_TRAINING_POINTS; i++){
        if(fscanf(training_set_file, "%f", &new_point.x1) == EOF) break;
        if(fscanf(training_set_file, "%f", &new_point.x2) == EOF) break;
        if(fscanf(training_set_file, "%d", &new_point.category) == EOF) break;
        training_set[i] = new_point;
    }

    for(int i=0; i<NUMBER_OF_TEST_POINTS; i++){
        if(fscanf(test_set_file, "%f", &new_point.x1) == EOF) break;
        if(fscanf(test_set_file, "%f", &new_point.x2) == EOF) break;
        if(fscanf(test_set_file, "%d", &new_point.category) == EOF) break;
        test_set[i] = new_point;
    }
}


void set_targets(){
    int category;

    for(int i=0; i<NUMBER_OF_TRAINING_POINTS; i++){
        category = training_set[i].category;
        switch (category){
            case C1:
                training_targets[i][0] = 1;
                break;
            case C2:
                training_targets[i][1] = 1;
                break;
            case C3: 
                training_targets[i][2] = 1;
                break;
            case C4:
                training_targets[i][3] = 1;
                break;
        }
    }
    for(int i=0; i<NUMBER_OF_TEST_POINTS; i++){
        category = test_set[i].category;
        switch (category){
            case C1:
                test_targets[i][0] = 1;
                break;
            case C2:
                test_targets[i][1] = 1;
                break;
            case C3: 
                test_targets[i][2] = 1;
                break;
            case C4:
                test_targets[i][3] = 1;
                break;
        }
    }
}


void initialize_weights(){

    for(int i=0; i<H1+1; i++){
        hidden_layer1_neurons[i].weights = hidden_layer1_weights[i];
        hidden_layer1_neurons[i].partial_derivatives = hidden_layer1_partial_derivatives[i];
        for(int j=0; j<d+1; j++){
            hidden_layer1_neurons[i].weights[j] = choose_random_float_in_range(-1, 1);
        }
    }

    for(int i=0; i<H2+1; i++){
        hidden_layer2_neurons[i].weights = hidden_layer2_weights[i];
        hidden_layer2_neurons[i].partial_derivatives = hidden_layer2_partial_derivatives[i];
        for(int j=0; j<H1+1; j++){
            hidden_layer2_neurons[i].weights[j] = choose_random_float_in_range(-1, 1);
        }
    }

    for(int i=0; i<NUMBER_OF_CATEGORIES+1; i++){
        output_layer_neurons[i].weights = output_layer_weights[i];
        output_layer_neurons[i].partial_derivatives = output_layer_partial_derivatives[i];
        for(int j=0; j<H2+1; j++){
            output_layer_neurons[i].weights[j] = choose_random_float_in_range(-1, 1);
        }
    }
}


void forward_pass(Point point, float *y){

    // entry layer
    entry_layer_neurons[0].output = 1;
    entry_layer_neurons[1].output = point.x1;
    entry_layer_neurons[2].output = point.x2; 

    // 1st hidden layer
    hidden_layer1_neurons[0].output = 1;
    for(int i=1; i<H1+1; i++){
        hidden_layer1_neurons[i].input = 0;
        for(int j=0; j<d+1; j++){
            hidden_layer1_neurons[i].input += hidden_layer1_neurons[i].weights[j] * entry_layer_neurons[j].output;
        }
        hidden_layer1_neurons[i].output = ACTIVATION_FUNCTION_1(hidden_layer1_neurons[i].input);
    }

    // 2nd hidden layer
    hidden_layer2_neurons[0].output = 1;
    for(int i=1; i<H2+1; i++){
        hidden_layer2_neurons[i].input = 0;
        for(int j=0; j<H1+1; j++){
            hidden_layer2_neurons[i].input += hidden_layer2_neurons[i].weights[j] * hidden_layer1_neurons[j].output;
        }
        hidden_layer2_neurons[i].output = ACTIVATION_FUNCTION_2(hidden_layer2_neurons[i].input);
    }

    // output layer
    for(int i=1; i<NUMBER_OF_CATEGORIES+1; i++){
        output_layer_neurons[i].input = 0;
        for(int j=0; j<H2+1; j++){
            output_layer_neurons[i].input += output_layer_neurons[i].weights[j] * hidden_layer2_neurons[j].output;
        }
        output_layer_neurons[i].output = ACTIVATION_FUNCTION_OUT(output_layer_neurons[i].input);
        y[i-1] = output_layer_neurons[i].output;
    }
}


void backprop(Point point, float *target){
    Neuron *neuron;
    float output[NUMBER_OF_CATEGORIES];
    
    forward_pass(point, output);

    // output layer
    for(int i=1; i<NUMBER_OF_CATEGORIES+1; i++){
        neuron = &output_layer_neurons[i];
        neuron->error = neuron->output * (1 - neuron->output) * (output[i-1] - target[i-1]);
        for(int j=1; j<H2+1; j++){
            neuron->partial_derivatives[j] += neuron->error * hidden_layer2_neurons[j].output;
        }
    }

    // layer 2
    for(int i=0; i<H2+1; i++){
        neuron = &hidden_layer2_neurons[i];
        neuron->error = 0;
        for(int j=0; j<NUMBER_OF_CATEGORIES+1; j++){
            neuron->error += neuron->output * (1 - neuron->output) * output_layer_neurons[j].weights[i] * output_layer_neurons[j].error;
            //neuron->error += output_layer_neurons[j].weights[i] * output_layer_neurons[j].error;
        }
        for(int k=0; k<H1+1; k++){
            if(k==0){
                neuron->partial_derivatives[k] = neuron->error;
                continue;
            } 
            neuron->partial_derivatives[k] += neuron->error * hidden_layer1_neurons[k].output;
        }
    }

    // layer 1
    for(int i=0; i<H1+1; i++){
        neuron = &hidden_layer1_neurons[i];
        neuron->error = 0;
        for(int j=0; j<H2+1; j++){
            neuron->error += (1 - pow(neuron->output, 2)) * hidden_layer2_neurons[j].weights[i] * hidden_layer2_neurons[j].error;
        }
        for(int k=0; k<d+1; k++){
            if(k==0){
                neuron->partial_derivatives[k] = neuron->error;
                continue;
            }
            neuron->partial_derivatives[k] += neuron->error * entry_layer_neurons[k].output;
        }
    }
}


void train_neural_network(){
    float n = 0.1;
    float threshold = 0.1;
    int epoch = 0;
    int minimum_epochs_number = 500;
    float last_total_error;
    float current_total_error;

    initialize_weights();

    while(1){
        reset_partial_derivatives();

        current_total_error = 0;
        for(int i=0; i<NUMBER_OF_TRAINING_POINTS; i++){
            backprop(training_set[i], training_targets[i]);
            current_total_error += calculate_point_error(training_targets[i]);
            if(i%B == 0){
                update_weights(n);
                reset_partial_derivatives();
            }
        }
    
        if(epoch >= minimum_epochs_number && fabs(last_total_error - current_total_error) < threshold){
            break;
        }
        epoch++;
        last_total_error = current_total_error;
        printf("(epoch %d)\ttotal training error -> %f\n", epoch, current_total_error);
    }
}


void test_neural_network(){
    float output[NUMBER_OF_CATEGORIES];
    int number_of_correct_decisions = 0;
    float percentage;

    for(int i=0; i<NUMBER_OF_TEST_POINTS; i++){
        int output_category = 0;
        int actual_category = 0;
        float max = output[0];
        float max_pos = 0;

        forward_pass(test_set[i], output);
        for(int j=0; j<NUMBER_OF_CATEGORIES; j++){
            if(test_targets[i][j] == 1){
                actual_category = j + 1;
            }
            if(output[j] > max){
                max = output[j];
                max_pos = j;
            }
        }
        output_category = max_pos + 1;
        if(output_category == actual_category){
            number_of_correct_decisions++;
            fprintf(correct_decisions_file, "%f\t%f\n", test_set[i].x1, test_set[i].x2);
        }else{
            fprintf(wrong_decisions_file, "%f\t%f\n", test_set[i].x1, test_set[i].x2);
        }
            
    }
    percentage = (float) number_of_correct_decisions / (float) NUMBER_OF_TEST_POINTS;
    printf("\nGeneralization ability of the neural network -> %0.2f%%\n", percentage * 100);
}


float calculate_point_error(float *target){
    float total_error = 0;

    for(int i=1; i<NUMBER_OF_CATEGORIES+1; i++){
        total_error += fabs(output_layer_neurons[i].output - target[i-1]);
    }

    return total_error;
}


void reset_partial_derivatives(){

    for(int i=0; i<H1+1; i++){
        for(int j=0; j<d+1; j++){
            hidden_layer1_neurons[i].partial_derivatives[j] = 0;
        }
    }

    for(int i=0; i<H2+1; i++){
        for(int j=0; j<H1+1; j++){
            hidden_layer2_neurons[i].partial_derivatives[j] = 0;
        }
    }

    for(int i=0; i<NUMBER_OF_CATEGORIES+1; i++){
        for(int j=0; j<H2+1; j++){
            output_layer_neurons[i].partial_derivatives[j] = 0;
        }
    }
}


void update_weights(float n){
       
    // layer 1
    for(int i=0; i<H1+1; i++){
        for(int j=0; j<d+1; j++){
            hidden_layer1_neurons[i].weights[j] -= n * hidden_layer1_neurons[i].partial_derivatives[j];
        }
    }

    // layer 2
    for(int i=0; i<H2+1; i++){
        for(int j=0; j<H1+1; j++){
            hidden_layer2_neurons[i].weights[j] -= n * hidden_layer2_neurons[i].partial_derivatives[j];
        }
    }

    // output layer
    for(int i=0; i<NUMBER_OF_CATEGORIES+1; i++){
        for(int j=0; j<H2+1; j++){
            output_layer_neurons[i].weights[j] -= n * output_layer_neurons[i].partial_derivatives[j];
        }
    }
}


void print_dataset(){
    Point point;

    for(int i=0; i<NUMBER_OF_TRAINING_POINTS; i++){
        point = training_set[i];
        printf("(training set)\t");
        printf("(%0.2f,%0.2f),\t\tcategory -> C%d\t\t", point.x1, point.x2, point.category);
        printf("target -> (%0.0f,%0.0f,%0.0f,%0.0f)\n", training_targets[i][0], training_targets[i][1], training_targets[i][2], training_targets[i][3]);
    }
    
    for(int i=0; i<NUMBER_OF_TEST_POINTS; i++){
        point = test_set[i];
        printf("(test set)\t");
        printf("(%0.2f,%0.2f),\t\tcategory -> C%d\t\t", point.x1, point.x2, point.category);
        printf("target -> (%0.0f,%0.0f,%0.0f,%0.0f)\n", test_targets[i][0], test_targets[i][1], test_targets[i][2], test_targets[i][3]);
    }
    printf("\n");
}


void print_weights(){

    printf("weights - first hidden layer\n");
    for(int i =0; i<H1+1; i++){
        for(int j=0; j<d+1; j++){
            printf("w%d%d = %0.2f\n", i, j, hidden_layer1_neurons[i].weights[j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("weights - second hidden layer\n");
    for(int i =0; i<H2+1; i++){
        for(int j=0; j<H1+1; j++){
            printf("w%d%d = %0.2f\n", i, j, hidden_layer2_neurons[i].weights[j]);
        }
        printf("\n");
    }
    printf("\n");

    printf("weights - output layer\n");
    for(int i =0; i<NUMBER_OF_CATEGORIES+1; i++){
        for(int j=0; j<H2+1; j++){
            printf("w%d%d = %0.2f\n", i, j, output_layer_neurons[i].weights[j]);
        }
        printf("\n");
    }
    printf("\n");
}


void print_partial_derivatives(){
    
    for(int i=0; i<NUMBER_OF_CATEGORIES+1; i++){
        for(int j=0; j<H2+1; j++){
            printf("(output layer) partial_derivative[%d][%d] -> %f\n", i, j, output_layer_neurons[i].partial_derivatives[j]);
        }
        printf("\n");
    }
    printf("\n");

    for(int i=0; i<H2+1; i++){
        for(int j=0; j<H1+1; j++){
            printf("(layer 2) partial_derivative[%d][%d] -> %f\n", i, j, hidden_layer2_neurons[i].partial_derivatives[j]);
        }
        printf("\n");
    }
    printf("\n");

    for(int i=0; i<H1+1; i++){
        for(int j=0; j<d+1; j++){
            printf("(layer 1) partial_derivative[%d][%d] -> %f\n", i, j, hidden_layer1_neurons[i].partial_derivatives[j]);
        }
        printf("\n");
    }
    printf("\n");
}