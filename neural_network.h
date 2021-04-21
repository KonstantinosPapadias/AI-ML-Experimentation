typedef struct{
    float input;
    float output;
    float error;
    float *weights;
    float *partial_derivatives;
} Neuron;
