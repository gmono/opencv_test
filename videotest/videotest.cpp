// videotest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#pragma comment(lib,"opencv_world320.lib")
#include "../opencvtest/Funs.h"
using namespace cv;
bool stop=false;
void stopfun(int state,void *userdata)
{
	stop = true;
}
int main()
{
	VideoCapture capture("./Video/video.mp4");
	if (!capture.isOpened()) return 1;
	double rate = capture.get(CV_CAP_PROP_FPS);
	Mat frame;
	namedWindow("myvideo", WINDOW_NORMAL);
	namedWindow("myvideo2", WINDOW_NORMAL);
	int delay = 1000 / rate;
	capture.set(CV_CAP_PROP_POS_FRAMES, 100);
	while (!stop)
	{
		if (!capture.read(frame))
			break;
		
		Mat tf;
		//rotate(frame, tf,ROTATE_90_CLOCKWISE);
		//tf.copyTo(frame);

		//double w1 = (double)(rand() % 100)/100;
		//double w2 = (double)(rand() % 100) / 100;
		//double w3 = (double)(rand() % 100) / 100;
		//Funs::ChangeRGB(frame, tf, w1,w2, w3);
		//tf.copyTo(frame);

		//Funs::ReduceMask(frame, tf, 10, 20);
		//tf.copyTo(frame);

		Funs::ReduceMask(frame, tf, 5, 10);

		imshow("myvideo", tf);
		imshow("myvideo2", frame);
		waitKey(delay);
	}
	
	capture.release();
    return 0;
}

