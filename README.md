# Computational-Intelligence

Simple implementation of a multilayer perceptron with 2 hidden layers and the k-means algorithm in vanilla C, both using 2-dimensional data (points) as input. We have to generate the 2 datasets first and then:
* we can break the first dataset in a training and a test set so that we see how our mlp does in a foreing set after being trained
* use the second dataset as input for the k-means algorithm
The multilayer perceptron after being trained with the input training set of points can succesfully classify points between 5 categories. The k-means algorithm groups points according to the number of centers. 




You can try the programs on your own. Assuming a recent version of gcc as your compiler and a linux distro as your os (so your shell is bash and you have gnuplot installed), use the Makefile:


1. to train the mlp and see how it learns
    * compiling the programs (assuming a recent version of gcc as your compiler) - $ make all
    * creating the first dataset that will be used as training and test set for the multilayer perceptron - $ ./dataset1
    * visualizing the dataset you just created - $ gnuplot; gnuplot> plot "C1.dat", "C2.dat", "C3.dat", "C4.dat"
    * training and testing the mlp - $ ./mlp_classifier
    * visualizing the correct and wrong decisions of the mlp - $ gnuplot; gnuplot> plot "correct_decisions.dat", "wrong_decisions.dat" linetype 1 linewidth 6 linecolor -1
  
  
2. to try the k-means algorithm and see how it seperated the dataset into a number of groups
    * creating the second dataset that will get seperated in groups - $ ./dataset2
    * visualizing the dataset you just created - $ gnuplot; gnuplot> plot "dataset2.dat"
    * running the k-means algorithm to group the second dataset - $ ./kmeans
    * visualizing the grouped dataset according to the k-means algorithm - $ gnuplot; gnuplot> plot "kmeans_Team0.dat", "kmeans_Team1.dat", ...., "kmeans_Team(M-1).dat" , "kmeans_centroids.dat" linetype 1 linewidth 6 linecolor -1 (example for 5 centroids - gnuplot> plot "kmeans_Team0.dat", "kmeans_Team1.dat", "kmeans_Team2.dat", "kmeans_Team3.dat", "kmeans_Team4.dat", "kmeans_centroids.dat" linetype 1 linewidth 6 linecolor -1)
     
     
     
     
     
Of course you can try the whole process again!! Clean the mess of files that has been created with $ make clean and then:
* Play with the number of predefined number of nodes in the first and second hidden layers of the mlp. Change the values of H1 and H2 in #define H1 8 (which is the number of nodes in the first hidden layer, predefined as 8), and #define H2 5 (which is the number of nodes in the second hidden layer, predefined as 5) in lines 9 and 10 in the mlp_classifier.c file.
* Play with the number of predefined neighbors in k-means. Change the calue of M in #define M 5 (which is the number of groups the k-means algorithm will group the data) in line 12 in kmeans.c file).
