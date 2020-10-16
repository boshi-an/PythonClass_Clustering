#pragma once

# define MAXNUMFEATURE 20
# define MAXNUMTEXT 200
# define MAXNUMCATEGORY 20
# define MAXNUMCLUSTER 200

typedef struct
{
	double  feature[MAXNUMFEATURE];  //属性数组
	int  categories;	             //文本的预定义类，用作聚类的质量评测
	//int	 cluster;		             //文本所属的聚类号
}Text;

typedef struct
{
	double feature[MAXNUMFEATURE];	//质心向量
	int elementsCount;			    //类中文档数目
	int elements[MAXNUMTEXT];	    //类中文档ID号
}Cluster;

double GetSquaredEuclideanDist(double features1[], double features2[], int numFeatures);
double getEuclideanDist(double features1[], double features2[], int numFeatures);

void selectRandomSeeds(int n, int k, int seeds[]);
void updateClusterCentroid(int numClusters, int numFeatures, Text *texts, Cluster *clusters);
double CalculateRSS(int numClusters, int numFeatures, int numTexts, Text *texts, Cluster *clusters);
bool converge(int numClusters, int numFeatures, int numTexts, Text *texts, Cluster *clusters, double &prev_RSS, int iteration);
void reassignment(int numClusters, int numFeatures, int numTexts, Text *texts, Cluster *clusters);
void K_Means_Clustering(int numClusters, int numFeatures, int numTexts, int numCategories, Text *texts, Cluster *clusters);

double TryBisection(int numFeatures, int numTexts, int numCategories, Text *texts, Cluster cluster);
void Bisectional_K_Means_Clustering(int numClusters, int numFeatures, int numTexts, int numCategories, Text *texts, Cluster *clusters);

void K_Means_Clustering();
void DisplayClusters();

int IndexOf(char *);  //输入类名称，输出类标号
double Purity(Text *text, int text_num, Cluster *cluster, int cluster_num, int label_num);
double FMeasure(Text *text, int text_num, Cluster *cluster, int cluster_num, int label_num);