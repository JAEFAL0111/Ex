#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <time.h>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/flann.hpp"
#include "opencv2/calib3d.hpp"
#include "CMatchImg.h"

using namespace cv;
using namespace std;

int main()
{
	//////////
	time_t curr_time;					//long과 동일한개념?
	struct tm* curr_tm;					//구조체 선언?
	curr_time = time(NULL);				//Null을 인자로 계산
	curr_tm = localtime(&curr_time);	//tm구조체에 저장
	int ampm = 12;						//오전오후 구분변수
	//////////
	//시간출력
	string mytext = to_string(curr_tm->tm_year - 100); //기존년에서 2000년을 뺀다
	mytext += ".";									   //(0019년만 표시하려고)
	mytext += to_string(curr_tm->tm_mon + 1);
	mytext += ".";
	mytext += to_string(curr_tm->tm_mday);
	mytext += " ";
	if (curr_tm->tm_hour < ampm+1) //오후 12시가 넘지 않으면(13시 전까지)
	{
		mytext += "AM";
		mytext += to_string(curr_tm->tm_hour);
	}
	else //오후 12시가 넘으면
	{
		mytext += "PM";
		mytext += to_string(curr_tm->tm_hour - ampm);
	}
		mytext += ":";
	mytext += to_string(curr_tm->tm_min);
	//////////
	//이미지 불러오기
	Mat image;
	image = imread("Banana1.jpg", IMREAD_COLOR);
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	namedWindow("Original", WINDOW_AUTOSIZE);
	//텍스트출력(이미지, 텍스트, 좌표, 폰트, 글자크기, 색)
	putText(image, mytext, Point(30, 200), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 2);
	//////////
	imshow("Original", image);

	waitKey(0);
}


