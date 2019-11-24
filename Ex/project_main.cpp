#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/flann.hpp"
#include "opencv2/calib3d.hpp"
#include "CMatchImg.h"

using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;

const char* keys =
"{ help h |                  | Print help message. }"
"{ input1 | Before.png          | Path to input image 1. }"
"{ input2 | After.png | Path to input image 2. }";

int main(int argc, char* argv[])
{
	CommandLineParser parser(argc, argv, keys);

	CMatchImg MatchImg;
	MatchImg.imgOri = imread(samples::findFile(parser.get<String>("input2")), IMREAD_GRAYSCALE);
	MatchImg.imgEmpty = imread(samples::findFile(parser.get<String>("input1")), IMREAD_GRAYSCALE);
	if (MatchImg.imgOri.empty() || MatchImg.imgEmpty.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		parser.printMessage();
		return -1;
	}

	MatchImg.getDiffImg();
	MatchImg.getMatchedImg();
	MatchImg.printMatchImgs();

	waitKey();

	return 0;
}