#pragma once
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/flann.hpp"
#include "opencv2/calib3d.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;

#define WINDOW_BUFF_SIZE 100

class CMatchImg
{
public:
	CMatchImg() {};
	~CMatchImg() {};

	int getDiffImg();
	int getMatchedImg();
	void printMatchImgs();

	int findMatchedImg(Mat& imgModel, Mat& imgScene, Mat& imgMatched);
	int drawRectsMatchImg(Mat& imgModel, Mat& imgMatched, std::vector<DMatch>& good_matches,
		std::vector<KeyPoint>& keypoints1, std::vector<KeyPoint>& keypoints2);

	Mat imgEmpty;
	Mat imgOri;
	Mat imgMatched;

	std::vector<cv::Mat> subImgs;
	std::vector<cv::Mat> imgMatchs;
	std::vector<DMatch> good_matches;
	std::vector<KeyPoint> keypoints1;
	std::vector<KeyPoint> keypoints2;

	int gaussianMaskSize = 5;	// 홀수만 넣으세요
	int thresholdValue = 30;
	int erodeValue = 7;
	int dilateValue = 10;
	int NoisFilterSize = 1500;

	int drawContoursLineThickness = 10;

	int minHessian = 400;
	const float ratio_thresh = 0.7f; //-- Filter matches using the Lowe's ratio test

private:
};