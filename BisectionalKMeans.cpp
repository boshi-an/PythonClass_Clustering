#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>
#include "Text.h"

using namespace std;

double TryBisection(int numFeatures, int numCategories, Text *texts, Cluster cluster, Cluster *tmpClusters)
{
	Text *tmpTexts = new Text[cluster.elementsCount];
	int numTmpTexts = 0;
	for (int i = 0; i < cluster.elementsCount; i++)
	{
		tmpTexts[i] = texts[cluster.elements[i]];
		numTmpTexts += 1;
	}
	if(numTmpTexts <= 1) return 1e20;
	tmpClusters[0] = cluster;
	double before = CalculateRSS(1, numFeatures, numTmpTexts, tmpTexts, tmpClusters);
	K_Means_Clustering(2, numFeatures, numTmpTexts, numCategories, tmpTexts, tmpClusters);
	double after = CalculateRSS(2, numFeatures, numTmpTexts, tmpTexts, tmpClusters);
	delete[] tmpTexts;
	return after - before;
}

void Bisectional_K_Means_Clustering(int numClusters, int numFeatures, int numTexts, int numCategories, Text *texts, Cluster *clusters)
{
	int nowClusters = 1;
	clusters[0].elementsCount = numTexts;
	for (int i = 0; i < numTexts; i++)
		clusters[0].elements[i] = i;
	memmove(clusters[0].feature, texts[0].feature, sizeof(double) * (numFeatures - 1));

	while (nowClusters < numClusters)
	{
		double bestDownHill = 1e20;
		int bestID = -1;
		Cluster tmpClusters[2], bestClusters[2];
		for (int i = 0; i < nowClusters; i++)
		{
			double downHill = TryBisection(numFeatures, numCategories, texts, clusters[i], tmpClusters);
			if (downHill < bestDownHill)
			{
				bestDownHill = downHill;
				bestID = i;
				memmove(bestClusters, tmpClusters, sizeof(Cluster) * 2);
			}
		}

		swap(clusters[bestID], clusters[nowClusters - 1]);

		int num = clusters[nowClusters - 1].elementsCount;
		int *real_id = new int[num];
		for (int i = 0; i < num; i++)
			real_id[i] = clusters[nowClusters - 1].elements[i];
		for (int i = 0; i < 2; i++)
		{
			clusters[nowClusters - 1 + i].elementsCount = bestClusters[i].elementsCount;
			for (int j = 0; j < clusters[nowClusters - 1 + i].elementsCount; j++)
				clusters[nowClusters - 1 + i].elements[j] = real_id[bestClusters[i].elements[j]];
		}
		delete[] real_id;

		nowClusters += 1;

		/*cout<<nowClusters<<":"<<endl;
		for(int i=0; i<nowClusters; i++)
		{
			for(int j=0; j<clusters[i].elementsCount; j++) cout << clusters[i].elements[j] << " ";
			cout<<endl;
		}*/
	}
}