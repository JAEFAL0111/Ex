#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <thread>
#include <Windows.h>
#include <process.h>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/flann.hpp"
#include "opencv2/calib3d.hpp"
/*
#define THREAD_COUNT 4
#define SEMAPHORE_START 2
#define SEMAPHORE_MAX 2

HANDLE g_semaphore = NULL;
int number = 0;

unsigned int __stdcall threadFun(void* lpVoid);


using std::thread;
using namespace std;
using namespace cv;

unsigned int __stdcall threadFun(void* lpVoid)
{
	std::cout << "Enter : " << __FUNCTION__ << std::endl;

	for (int i = 0; i < 10; i++)
	{
		WaitForSingleObject(g_semaphore, INFINITE);
		
		std::cout << number << __FUNCTION__ << std::endl;

		number++;

		ReleaseSemaphore(g_semaphore, 1, NULL);
	}

	std::cout << "End : " << __FUNCTION__ << std::endl;

	return 0;
}


int main(int, char**)
{
	int index = 0;
	char buf[256];
	Mat frameMat;

	HANDLE hThread[THREAD_COUNT];
	g_semaphore = CreateSemaphore(NULL, SEMAPHORE_START, SEMAPHORE_MAX, NULL);

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, threadFun, NULL, 0, NULL);
	}

	WaitForMultipleObjects(THREAD_COUNT, hThread, true, INFINITE);

	for (int i = 0; i < THREAD_COUNT; i++)
	{
		CloseHandle(hThread[i]);
	}
	CloseHandle(g_semaphore);

	return 0;
}
*/

using std::thread;
using std::vector;
using namespace std;
using namespace cv;


Mat frameMat;
VideoCapture videoCapture(0);


void func2() {


	int index = 0;
	char buf[256];

	while (1)
	{

		getchar();

		videoCapture.read(frameMat);

		//이미지로 저장  
		sprintf(buf, "c:/temp/img_%06d.jpg", index);
		cout << buf << endl;
		imwrite(buf, frameMat);

		index++;
		if (index == 99) index = 0;

	}
}

void func1() {



	if (!videoCapture.isOpened())
	{
		printf("첫번째 카메라를 열수 없습니다. \n");
	}


	namedWindow("웹캠 영상", WINDOW_AUTOSIZE);

	//waitKey();
	while (1)
	{

		//웹캡으로부터 한 프레임을 읽어옴  
		videoCapture.read(frameMat);

		//화면에 보여줌
		imshow("웹캠 영상", frameMat);

		if (waitKey(25) == 27) break; //ESC키 누르면 종료  
	}
}
int main(int, char**)
{

	thread t1(func1);
	thread t2(func2);

	t1.join();
	t2.join();

	return 0;
}

