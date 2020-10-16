// Clustering.cpp : Defines the entry point for the console application.
// More details in Readme.md
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>
#include "Text.h"

using namespace std;

int numFeatures = 0;

Cluster clusters[MAXNUMTEXT]; //cluster: {质心向量；类中文档数目；类中文档ID号}

char categories[MAXNUMCATEGORY][100]; //字符串数组，存放类型的名称
int numCategories = 0;				  //实际分类的数目

Text texts[MAXNUMTEXT]; //Text：{属性数组；文本的预定义类，用作聚类的质量评测；文本所属的聚类号}
int numTexts = 0;

//从数据文件中读入文档特征向量数据
bool Input(const char *fpath)
{
	FILE *file = fopen(fpath, "r"); //打开数据文件
	if (file == NULL)
	{
		fprintf(stderr, "[ERROR]: unable to open the data file\n");
		return false;
	}

	fscanf(file, "%d %d %d", &numTexts, &numFeatures, &numCategories); // 数据文件第一行：文档数，特征数，分类数

	for (int i = 0; i < numCategories; i++) // 读入类型名称: games IT sports oversea entertainment military  political
		fscanf(file, "%s", categories[i]);

	char tmp[100]; // 特征向量的分类标记（用于评测聚类结果）
	for (int i = 0; i < numTexts; i++)
	{
		for (int j = 0; j < numFeatures - 1; j++) // 读入一条特征向量
			fscanf(file, "%lf", &texts[i].feature[j]);

		fscanf(file, "%s", tmp); //读入该向量的分类标记名称

		int index = IndexOf(tmp); // 取得该标记对应的分类号
		if (-1 == index)
			return false;

		texts[i].categories = index; //设置分类属性
	}
	return true;
}

int IndexOf(char *t) //输入类名称，返回类标号
{
	if (t == NULL)
	{
		fprintf(stderr, "[WARING]: attempt to  find index of NULL in category array\n");
		return -1;
	}

	for (int i = 0; i < numCategories; i++)
	{
		if (0 == strcmp(categories[i], t))
			return i;
	}

	fprintf(stderr, "[WARING]: unable to find index of %s\n", t);
	return -1;
}

//显示所有的类的质心以及类的文档号
void DisplayClusters(int numClusters)
{
	/*for (int i = 0; i < numClusters; i++) //按每个类
	{
		printf("Cluster ID %d:\n", i);
		printf("Centroid: ");
		for (int j = 0; j < numFeatures - 1; j++) //重心向量
			printf("%lf ", clusters[i].feature[j]);

		printf("\n");
		printf("Elements ");
		for (int k = 0; k < clusters[i].elementsCount; k++) //所包含的文档编号
			printf("%d ", clusters[i].elements[k]);

		printf("\n");
	}*/
	printf("Final Purity: %.6lf\n", Purity(texts, numTexts, clusters, numClusters, numCategories));
	printf("Final F-Measure: %.6lf\n", FMeasure(texts, numTexts, clusters, numClusters, numCategories));
}

void FindBestClustering(void (*f)(int, int, int, int, Text *, Cluster *), 
						int numClusters, int numFeatures, int numTexts, int numCategories,
						Text *texts, Cluster *clusters, int round)
{
	Cluster *tmpCluster = new Cluster[numClusters];
	double bestFMeasure = -1;
	for(int i=1; i<=round; i++)
	{
		f(numClusters, numFeatures, numTexts, numCategories, texts, tmpCluster);
		double curFMeasure = FMeasure(texts, numTexts, tmpCluster, numClusters, numCategories);
		if(curFMeasure > bestFMeasure)
		{
			bestFMeasure = curFMeasure;
			memmove(clusters, tmpCluster, sizeof(Cluster) * (numClusters));
		}
	}
	delete[] tmpCluster;
}

int main()
{
	srand(time(0)); //生成伪随机数种子
	freopen("./Log.txt", "w", stderr);
	if (!Input("./data/data1.txt"))
		return -1;

	int numClusters = 7, numRound = 10, numAvgRound = 10;
	double avg = 0;
	printf("Doing K-Means Cluster:\n");
	for(int i=0; i<numAvgRound; i++)
	{
		FindBestClustering(K_Means_Clustering, numClusters, numFeatures, numTexts, numClusters, texts, clusters, numRound);
		avg += FMeasure(texts, numTexts, clusters, numClusters, numCategories);
		//DisplayClusters(numClusters);															//显示聚类结果
	}
	printf("avg accuracy: %.4lf\n", avg / numAvgRound);

	avg = 0;
	printf("Doing Bisectional K-Means Cluster:\n");
	for(int i=0; i<numAvgRound; i++)
	{
		FindBestClustering(Bisectional_K_Means_Clustering, numClusters, numFeatures, numTexts, numClusters, texts, clusters, numRound);
		avg += FMeasure(texts, numTexts, clusters, numClusters, numCategories);
		//DisplayClusters(numClusters);															//显示聚类结果
	}
	printf("avg accuracy: %.4lf\n", avg / numAvgRound);																	//显示聚类结果

	return 0;
}