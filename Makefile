CC=gcc

all: dataset1 mlp_classifier dataset2 kmeans

dataset1: 
	$(CC) -o dataset1 dataset1.c -lm

mlp_classifier:
	$(CC) -o mlp_classifier mlp_classifier.c -lm

dataset2:
	$(CC) -o dataset2 dataset2.c

kmeans:
	$(CC) -o kmeans kmeans.c -lm

clean:
	@rm -f dataset1 mlp_classifier dataset2 kmeans ./*.dat ./*.txt 
