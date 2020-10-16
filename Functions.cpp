#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>
#include "Text.h"

using namespace std;

//计算两个特征向量（两篇文档或者一篇文档和一个类的质心）之间的平方欧氏距离
double GetSquaredEuclideanDist(double features1[], double features2[], int numFeatures)
{
	double dist = 0;

	for (int i = 0; i < numFeatures - 1; i++)
	{
		dist += (features1[i] - features2[i]) * (features1[i] - features2[i]);
	}
	return dist;
}

//计算两个文档（或者一篇文档和一个类的质心）之间的欧氏距离
double getEuclideanDist(double features1[], double features2[], int numFeatures)
{
	return sqrt(GetSquaredEuclideanDist(features1, features2, numFeatures));
}

//计算Residual Sum of Square: the squared distance of each vector
//from its centroid summed over all vectors.
double CalculateRSS(int numClusters, int numFeatures, int numTexts, Text *texts, Cluster *clusters)
{
	double *RSS = new double[numClusters]; // 每个类计算一个RSS值
	double totalRSS = 0.0;

	memset(RSS, 0, sizeof(double) * numClusters); //初始化RSS值数组

	for (int i = 0; i < numClusters; i++)
		for (int j = 0; j < clusters[i].elementsCount; j++)
			RSS[i] += GetSquaredEuclideanDist(texts[clusters[i].elements[j]].feature, clusters[i].feature, numFeatures);

	for (int i = 0; i < numClusters; i++) //平方距离总和
		totalRSS += RSS[i];

	delete[] RSS;
	return totalRSS;
}