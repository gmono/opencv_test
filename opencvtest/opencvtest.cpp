// opencvtest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Funs.h"
#pragma comment(lib,"opencv_world320.lib")
using namespace cv;
///噪点测试 如果提供一个参数代表原位处理 否则以result为返回
//p代表噪点比例 0-1
namespace CallBack
{
	int salt_val;
	void salt_cbk(int pos,void *userdata)
	{
		Mat **arr = (Mat **)(userdata);
		Funs::Salt((double)salt_val / 100, *arr[0], arr[1]);
		imshow("salt", *arr[1]);
	}
	//
	int w1=10, w2=10, w3=10;
	void grey_cbk(int pos, void *userdata)
	{
		Mat **arr = (Mat **)userdata;
		Funs::Grey(*arr[0], *arr[1], (double)w1/10/3, (double)w2/10/3, (double)w3/10/3);
		imshow("result", *arr[1]);
	}
	//
	int cw1=100, cw2=100, cw3=100;
	void change_cbk(int pos, void *userdata)
	{
		Mat **arr = (Mat **)userdata;
		Funs::ChangeRGB(*arr[0], *arr[1], (double)cw1 / 100, (double)cw2 / 100, (double)cw3 / 100);
		imshow("rgb", *arr[1]);
	}
	//
	int rw1 = 100, rw2 = 100, rw3 = 100,mr=0,mc=0;
	void mix_cbk(int pos, void *userdata)
	{
		Mat **arr = (Mat **)userdata;
		Mat &source = *arr[0];
		double rd = source.rows / 100;
		double cd = source.cols / 100;
		Funs::user_Add(*arr[0], *arr[1], *arr[2],mr*rd,mc*cd,(double)rw1 / 100, (double)rw2 / 100, (double)rw3 / 100);
		imshow("mix", *arr[2]);
	}
}
int main()
{
	Mat pic = imread("hua/1.jpg");
	namedWindow("helloworld", WINDOW_NORMAL);
	imshow("helloworld", pic);

	Mat rgb;
	Funs::ChangeRGB(pic, rgb);
	namedWindow("rgb", WINDOW_NORMAL);
	//创建数据
	Mat *rgb_arr[2] = { &pic,&rgb };
	createTrackbar("rgb_1", "rgb", &CallBack::cw1, 100, &CallBack::change_cbk, (void *)rgb_arr);
	createTrackbar("rgb_2", "rgb", &CallBack::cw2, 100, &CallBack::change_cbk, (void *)rgb_arr);
	createTrackbar("rgb_3", "rgb", &CallBack::cw3, 100, &CallBack::change_cbk, (void *)rgb_arr);
	imshow("rgb", rgb);

	Mat result;
	Funs::Grey(pic, result);
	namedWindow("result", WINDOW_NORMAL);
	//注册拖条
	//创建数据
	Mat *grey_arr[2] = { &pic,&result };
	createTrackbar("grey_1", "result", &CallBack::w1, 10, &CallBack::grey_cbk, (void *)grey_arr);
	createTrackbar("grey_2", "result", &CallBack::w2, 10, &CallBack::grey_cbk, (void *)grey_arr);
	createTrackbar("grey_3", "result", &CallBack::w3, 10, &CallBack::grey_cbk, (void *)grey_arr);
	imshow("result", result);
	Mat salt;
	Funs::Salt(0.05, result, &salt);
	namedWindow("salt", WINDOW_NORMAL);
	imshow("salt",salt);
	//创建salt比例调节
	//创建source 和dest的指针数组
	Mat *arr[2] = { &result,&salt };
	createTrackbar("salt_track", "salt", &CallBack::salt_val, 100,&CallBack::salt_cbk,arr);
	//图片叠加
	Mat pic2 = imread("hua/7.jpg");
	Mat dp;
	Mat *rarr[3] = { &pic,&pic2,&dp };
	
	namedWindow("mix", WINDOW_NORMAL);
	namedWindow("mix_bar", WINDOW_NORMAL);
	createTrackbar("mix_1", "mix_bar", &CallBack::rw1, 100, &CallBack::mix_cbk, (void *)rarr);
	createTrackbar("mix_2", "mix_bar", &CallBack::rw2, 100, &CallBack::mix_cbk, (void *)rarr);
	createTrackbar("mix_3", "mix_bar", &CallBack::rw3, 100, &CallBack::mix_cbk, (void *)rarr);
	createTrackbar("mix_r", "mix_bar", &CallBack::mr, 100, &CallBack::mix_cbk, (void *)rarr);
	createTrackbar("mix_c", "mix_bar", &CallBack::mc, 100, &CallBack::mix_cbk, (void *)rarr);
	Funs::user_Add(pic, pic2, dp, 0, 0, 1, 1, 1);
	imshow("mix", dp);
	Mat pic4 = imread("hua/4.jpg");
	imshow("mix_bar", pic4);
	//马赛克
	namedWindow("mask", WINDOW_NORMAL);
	Mat mpic;
	Funs::Mask(pic4, mpic, 10, 10);
	imshow("mask", mpic);
	//均值马赛克
	namedWindow("maskex", WINDOW_NORMAL);
	Mat mpic_ex;
	Funs::Mask(pic4, mpic_ex, 10, 10);
	imshow("maskex", mpic_ex);
	//去马赛克
	namedWindow("reduce_mask", WINDOW_NORMAL);
	Mat remask;
	Funs::ReduceMask(mpic_ex, remask, 10, 10);
	imshow("reduce_mask", remask);
	waitKey(0);
    return 0;
}

