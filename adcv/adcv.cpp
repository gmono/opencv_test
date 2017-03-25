// adcv.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#pragma comment(lib,"opencv_world320.lib")
using namespace cv;
int val = 50;
void sharpecbk(int pos, void *usdata)
{
	Mat **arr = (Mat **)usdata;
	Mat *img = arr[0];
	Mat temp;
	Mat *fmat = arr[1];
	
	float t = (1 - (float)val / 200)/48;
	for (int i = 0; i < 7; ++i)
	{
		for(int j=0;j<7;++j)
			fmat->at<float>(i, j) = t;
	}
	fmat->at<float>(1, 1) = (float)val / 10;
	fmat->at<float>(0, 0) = (float)val / 20;
	fmat->at<float>(0, 1) = -((float)val / 10);
	fmat->at<float>(1, 0) = ((float)val / 30);
	fmat->at<float>(1, 2) = -((float)val / 10);
	fmat->at<float>(2, 1) = ((float)val / 30);
	fmat->at<float>(3, 3) = (float)val / 200;
	filter2D(*img, temp, img->depth(), *fmat);
	imshow("test", temp);
}
int main()
{
	Mat fmat(7, 7, CV_32F);
	float *row=fmat.ptr<float>(0);
	row[0] = 0; row[1] = -1; row[2] = 0;
	row = fmat.ptr<float>(1);
	row[0] = -1; row[1] = 5; row[2] = -1;
	row = fmat.ptr<float>(2);
	row[0] = 0; row[1] = -1; row[2] = 0;
	Mat img = imread("../opencvtest/hua/5.jpg");
	Mat *arr[] = { &img,&fmat };
	sharpecbk(0,arr);
	createTrackbar("val", "test", &val, 150, sharpecbk, arr);
	namedWindow("test");
	waitKey(0);
    return 0;
}

