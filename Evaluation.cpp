//Evaluation.cpp : Algorithms that evaluats whether a clustering is effective
//FMeasure() : Calculates the F-Measure of a clustering
//Purity() : Calculates the purity of a clustering
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <climits>
#include <vector>
#include "Text.h"

using namespace std;

double Match(int n, const vector<vector<double> > &mat)
{
	vector<bool> vis(n+1, false);
	vector<double> slk(n+1, 0);
	vector<int> mch(n+1, 0);
	vector<int> pre(n+1, 0);
	vector<double> lbx(n+1, 0);
	vector<double> lby(n+1, 0);

	for (int i = 1; i <= n; i++)
		lbx[i] = 0, lby[i] = 0, mch[i] = 0;
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			lbx[i] = max(lbx[i], mat[i][j]);
	for (int i = 1; i <= n; i++)
	{
		double d;
		int py, p, x;
		for (int j = 1; j <= n; j++)
			slk[j] = +1e20, vis[j] = 0;
		for (mch[py = 0] = i; mch[py]; py = p)
		{
			vis[py] = 1;
			d = +1e20;
			x = mch[py];
			for (int y = 1; y <= n; y++)
			{
				if (!vis[y])
				{
					if (lbx[x] + lby[y] - mat[x][y] < slk[y])
						slk[y] = lbx[x] + lby[y] - mat[x][y], pre[y] = py;
					if (slk[y] < d)
						d = slk[y], p = y;
				}
			}
			for (int y = 0; y <= n; y++)
				if (vis[y])
					lbx[mch[y]] -= d, lby[y] += d;
				else
					slk[y] -= d;
		}
		for (; py; py = pre[py])
			mch[py] = mch[pre[py]];
	}

	double res = 0;

	for (int i = 1; i <= n; i++)
		res += mat[mch[i]][i];


	return res;
}

double FMeasure(Text *text, int text_num, Cluster *cluster, int cluster_num, int label_num)
{
	int N = max(cluster_num, label_num);
	vector<int> SizeLabel(text_num, 0);
	vector<double> line(N+1, 0);
	vector<vector<double> > mat;
	for(int i=0; i<N+1; i++) mat.push_back(line);

	for (int i = 0; i < label_num; i++)
		SizeLabel[i] = 0;
	for (int i = 0; i < text_num; i++)
		SizeLabel[text[i].categories]++;

	auto P_ri = [=](int cluster_id, int label_id) {
		if (cluster_id >= cluster_num || label_id >= label_num)
			return 0.0;
		double P_and_C = 0;
		double P = SizeLabel[label_id], C = cluster[cluster_id].elementsCount;
		for (int i = 0; i < cluster[cluster_id].elementsCount; i++)
			if (text[cluster[cluster_id].elements[i]].categories == label_id)
				P_and_C++;
		double acc = P_and_C / (C + 1e-10);
		double rec = P_and_C / (P + 1e-10);
		return 2 * acc * rec / (acc + rec + 1e-10);
	};

	double TotalNum = 0;
	for (int i = 0; i < cluster_num; i++)
		TotalNum += cluster[i].elementsCount;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (i < cluster_num && j < label_num)
				mat[i + 1][j + 1] = P_ri(i, j) * cluster[i].elementsCount / (TotalNum + 1e-10);
			else
				mat[i + 1][j + 1] = 0;

	double res = Match(N, mat);


	return res;
}

double Purity(Text *text, int text_num, Cluster *cluster, int cluster_num, int label_num)
{
	int N = max(cluster_num, label_num);
	vector<vector<double> > mat;
	vector<double> line(N+1, 0);
	for(int i=0; i<N+1; i++) mat.push_back(line);

	auto P_ri = [=](int cluster_id, int label_id) {
		if (cluster_id >= cluster_num || label_id >= label_num)
			return 0.0;
		double up = 0, dn = cluster[cluster_id].elementsCount;
		for (int i = 0; i < cluster[cluster_id].elementsCount; i++)
			if (text[cluster[cluster_id].elements[i]].categories == label_id)
				up += 1.0;
		return up / (dn + 1e-10);
	};

	double TotalNum = 0;
	for (int i = 0; i < cluster_num; i++)
		TotalNum += cluster[i].elementsCount;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (i < cluster_num && j < label_num)
				mat[i + 1][j + 1] = P_ri(i, j) * cluster[i].elementsCount / (TotalNum + 1e-10);
			else
				mat[i + 1][j + 1] = 0;

	double res = Match(N, mat);

	return res;
}