
main: Clustering.cpp Evaluation.cpp BisectionalKMeans.cpp RandomKMeans.cpp Functions.cpp
	g++ -Wall Clustering.cpp Evaluation.cpp BisectionalKMeans.cpp RandomKMeans.cpp Functions.cpp -o main -g -std=c++11
clean:
	rm -rf ./main