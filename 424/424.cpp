// 424.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>
#include<opencv.hpp>
#include<vector>
#include<cmath>
using namespace cv;
using namespace std;

 
void x(vector<vector<float>>&v, Mat&so)
{
	vector<Mat> srcMatpart(so.channels());
	split(so, srcMatpart);
	int blocksize = 16;
	Mat refMat = srcMatpart[0];
	int nx = refMat.cols / blocksize;
	int ny = refMat.rows / blocksize;
	for (int i = 0; i < nx; i++) {
		for (int j = 0; j < ny; j++)
		{
			Rect r(i * 16, j * 16, 16, 16);
			Mat current = refMat(r);
			Mat gx, gy;
			Sobel(current, gx, CV_32F, 1, 0, 1);
			Sobel(current, gy, CV_32F, 0, 1, 1);
			Mat mag, angle;
			cartToPolar(gx, gy, mag, angle, true);
			vector<float> f;
			f.resize(8);
			for (int p = 0; p < 16; p++) {
				for (int q = 0; q < 16; q++) {
					int a = angle.at<float>(p, q) / 45;
					f[angle.at<float>(p, q) / 45] += mag.at<float>(p, q);
				}
			}
			v.push_back(f);
		}
	}
}
float y(vector<vector<float>>&v1, vector<vector<float>>&v2) {
	float dis = 0;
	for (int  i = 0; i < v1.size(); i++)
	{
		for (int j = 0; j < 8; j++)
		{
			int d = (v1[i][j] - v2[i][j]);
			dis += d*d;
		}
	}
	dis = sqrt(dis);
	return dis;
}
int main()
{
	vector<vector<float>> v;
	vector<vector<float>> v1;
	vector<vector<float>> v2;
	Mat src = imread("D:\\2.jpg");
	Mat p1 = imread("D:\\1.jpg");
	Mat p2 = imread("D:\\3.jpg");
	x(v, src);
	x(v1, p1);
	x(v2, p2);
	cout << y(v, v1) << endl
	     << y(v, v2) << endl
	     << y(v1, v2) << endl;
	getchar();
}

