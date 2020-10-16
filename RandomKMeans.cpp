#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>
#include "Text.h"

#define RSS_THRESHOLD 1e-2	 // RSS停止阈值
#define MAX_ITERATION_NUM 50 // 最大迭代次数

using namespace std;

//从n篇文档(文档号：0~n-1)中随机选择k个(不重复的数)作为初始聚类种子
void selectRandomSeeds(int n, int k, int seeds[])
{
	int i;
	int index = n;

	int *tempArr = new int[n]; //用来存放n篇文档的标号，动态申请避免占用栈空间
	for (i = 0; i < n; i++)
		tempArr[i] = i;

	//srand(1);
	fprintf(stderr, "Random Seeds: ");

	for (i = 0; i < k; i++)
	{
		int r = rand() % index;	 //生成 0到index-1之间的一个随机数
		seeds[i] = tempArr[r];	 //返回所对应的文档标号
		fprintf(stderr, "%d ", seeds[i]); //完成一个种子，打印之

		tempArr[r] = tempArr[index - 1]; //最后的那个元素搬移填空
		index--;						 //当前数组长度减一
	}
	fprintf(stderr, "\n");

	delete[] tempArr; //释放空间
}

//更新各个类的质心
void updateClusterCentroid(int numClusters, int numFeatures, Text *texts, Cluster *clusters)
{
	int i, j, k;
	for (i = 0; i < numClusters; i++) //每个类跟新
	{
		if (clusters[i].elementsCount == 0) //没有元素就pass
			continue;

		memset(clusters[i].feature, 0, sizeof(double) * (numFeatures - 1)); //清空原质心向量

		for (j = 0; j < clusters[i].elementsCount; j++) //对类中的所有样本的向量加总 平均
			for (k = 0; k < numFeatures - 1; k++)		//按每个特征依次进行加总
				clusters[i].feature[k] += texts[clusters[i].elements[j]].feature[k];

		for (k = 0; k < numFeatures - 1; k++)
			clusters[i].feature[k] /= clusters[i].elementsCount; //平均
	}
}

//判断迭代中止条件：RSS值变化幅度小于阈值RSS_THRESHOLD，或者迭代次数大于最大允许次数
bool converge(int numClusters, int numFeatures, int numTexts, Text *texts, Cluster *clusters, double &prev_RSS, int iteration)
{
	bool flag = false;

	double cur_RSS = CalculateRSS(numClusters, numFeatures, numTexts, texts, clusters);
	fprintf(stderr, "\tRSS: %lf\n", cur_RSS);

	if (abs(cur_RSS - prev_RSS) < RSS_THRESHOLD || iteration > MAX_ITERATION_NUM)
		flag = true;

	prev_RSS = cur_RSS;
	return flag;
}

//对每个文本进行类的赋值，将它分到质心距离它最近的类中
void reassignment(int numClusters, int numFeatures, int numTexts, Text *texts, Cluster *clusters)
{
	int i, j;

	for (j = 0; j < numClusters; j++)
		clusters[j].elementsCount = 0;

	for (i = 0; i < numTexts; i++) //为每个样本找归属类
	{
		int belong = -1;
		double minDist = double(INT_MAX); //初始设定距离为INT_MAX

		for (j = 0; j < numClusters; j++) //计算跟所有的类之间的距离，选定距离最小的那个类
		{
			double dist = getEuclideanDist(texts[i].feature, clusters[j].feature, numFeatures); // 计算欧氏距离
			if (dist < minDist)													   //距离较小？
			{
				minDist = dist;		  //记录距离
				belong = j; //归入该类
			}
		}
		//在分到的类中添加新加入的文本ID号
		int count = clusters[belong].elementsCount; //取得该类目前的文档数
		clusters[belong].elements[count] = i;		  //在对应位置记录新加入的文档ID
		clusters[belong].elementsCount++;			  //文档数++
	}
}

void K_Means_Clustering(int numClusters, int numFeatures, int numTexts, int numCategories, Text *texts, Cluster *clusters)
{
	double prev_RSS = 1e20;
	int iteration = 1;

	int i, seeds[MAXNUMCLUSTER];

	selectRandomSeeds(numTexts, numClusters, seeds); //随机选K篇文档作为类的质心
	for (i = 0; i < numClusters; i++)
		memcpy(clusters[i].feature, texts[seeds[i]].feature, sizeof(double) * (numFeatures - 1));
	reassignment(numClusters, numFeatures, numTexts, texts, clusters); //对每个文本进行类的赋值
	while (!converge(numClusters, numFeatures, numTexts, texts, clusters, prev_RSS, iteration))
	{
		reassignment(numClusters, numFeatures, numTexts, texts, clusters); //对每个文本进行类的赋值
		updateClusterCentroid(numClusters, numFeatures, texts, clusters); //更新各个类的质心
		iteration++;
	}
}