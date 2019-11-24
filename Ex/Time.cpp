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
	time_t curr_time;					//long�� �����Ѱ���?
	struct tm* curr_tm;					//����ü ����?
	curr_time = time(NULL);				//Null�� ���ڷ� ���
	curr_tm = localtime(&curr_time);	//tm����ü�� ����
	int ampm = 12;						//�������� ���к���
	//////////
	//�ð����
	string mytext = to_string(curr_tm->tm_year - 100); //�����⿡�� 2000���� ����
	mytext += ".";									   //(0019�⸸ ǥ���Ϸ���)
	mytext += to_string(curr_tm->tm_mon + 1);
	mytext += ".";
	mytext += to_string(curr_tm->tm_mday);
	mytext += " ";
	if (curr_tm->tm_hour < ampm+1) //���� 12�ð� ���� ������(13�� ������)
	{
		mytext += "AM";
		mytext += to_string(curr_tm->tm_hour);
	}
	else //���� 12�ð� ������
	{
		mytext += "PM";
		mytext += to_string(curr_tm->tm_hour - ampm);
	}
		mytext += ":";
	mytext += to_string(curr_tm->tm_min);
	//////////
	//�̹��� �ҷ�����
	Mat image;
	image = imread("Banana1.jpg", IMREAD_COLOR);
	if (image.empty())
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	namedWindow("Original", WINDOW_AUTOSIZE);
	//�ؽ�Ʈ���(�̹���, �ؽ�Ʈ, ��ǥ, ��Ʈ, ����ũ��, ��)
	putText(image, mytext, Point(30, 200), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 2);
	//////////
	imshow("Original", image);

	waitKey(0);
}


