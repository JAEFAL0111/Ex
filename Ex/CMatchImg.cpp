#define _CRT_SECURE_NO_WARNINGS
#include "CMatchImg.h"

class Object
{
public:
	Object() {};
	~Object() {};

	int area = 0;
	int center_x = 0;
	int center_y = 0;
	int left = 0;
	int top = 0;
	int width = 0;
	int height = 0;
};

int CMatchImg::getDiffImg()
{
	int width = imgEmpty.cols;
	int height = imgEmpty.rows;

	cv::Mat diffImg = cv::Mat(height, width, CV_8UC3, cv::Scalar(0));

	diffImg = imgOri - imgEmpty;
	cv::GaussianBlur(diffImg, diffImg, cv::Size(gaussianMaskSize, gaussianMaskSize), 0);
	cv::threshold(diffImg, diffImg, thresholdValue, 255, cv::THRESH_BINARY);

	cv::Mat mask = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3), cv::Point(1, 1));
	cv::erode(diffImg, diffImg, mask, cv::Point(-1, -1), erodeValue);
	cv::dilate(diffImg, diffImg, mask, cv::Point(-1, -1), dilateValue);
	//cv::Canny(diffImg, diffImg, 50, 150);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(diffImg.clone(), contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	cv::drawContours(diffImg, contours, -1, cv::Scalar(255, 255, 255), drawContoursLineThickness);

	cv::Mat labels, stats, centroids;
	int nlabels = cv::connectedComponentsWithStats(diffImg, labels, stats, centroids);
	std::vector<Object> objects(nlabels);
	for (int i = 0; i < nlabels; i++)
	{

		if (i < 1)    continue;

		int area = stats.at<int>(i, cv::CC_STAT_AREA);
		int center_x = centroids.at<double>(i, 0);
		int center_y = centroids.at<double>(i, 1);
		int left = stats.at<int>(i, cv::CC_STAT_LEFT);
		int top = stats.at<int>(i, cv::CC_STAT_TOP);
		int width = stats.at<int>(i, cv::CC_STAT_WIDTH);
		int height = stats.at<int>(i, cv::CC_STAT_HEIGHT);

		if (area > NoisFilterSize)
		{
			//cv::rectangle(drawImg, cv::Point(left, top), cv::Point(left + width, top + height), cv::Scalar(0, 0, 255), 1);
			//cv::circle(drawImg, cv::Point(center_x, center_y), 5, (255, 0, 0), 1);
			//cv::putText(drawImg, std::to_string(i), cv::Point(left + 20, top + 20), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0), 2);

			objects[i].area = area;
			objects[i].center_x = center_x;
			objects[i].center_y = center_y;
			objects[i].left = left;
			objects[i].top = top;
			objects[i].width = width;
			objects[i].height = height;

			cv::Rect roi;
			roi.x = left;
			roi.y = top;
			roi.width = width;
			roi.height = height;

			subImgs.push_back(imgOri(roi));

			//std::string strPath = "C:\\";
			//strPath += std::to_string(i);
			//strPath += ".jpg";
			//cv::imwrite(strPath, subImg);
		}
	}
	return 1;
}

int CMatchImg::getMatchedImg()
{
	int i;
	Mat imgMatch;
	for (i = 0; i < subImgs.size(); i++)
	{
		imgMatchs.push_back(imgMatch);
		findMatchedImg(subImgs[i], imgOri, imgMatchs[i]);
	}
	return i;
}

void CMatchImg::printMatchImgs()
{
	char tempbuff[WINDOW_BUFF_SIZE];
	for (int i = 0; i < subImgs.size(); i++)
	{
		sprintf(tempbuff, "인식결과%d", i);
		namedWindow(tempbuff);
		imshow(tempbuff, imgMatchs[i]);
	}
}

int CMatchImg::findMatchedImg(Mat& imgModel, Mat& imgScene, Mat& imgMatched)
{
	if (imgModel.empty() || imgScene.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
		return -1;
	}
	//-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
	Ptr<SURF> detector = SURF::create(minHessian);
	std::vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;
	detector->detectAndCompute(imgModel, noArray(), keypoints1, descriptors1);
	detector->detectAndCompute(imgScene, noArray(), keypoints2, descriptors2);
	//-- Step 2: Matching descriptor vectors with a FLANN based matcher
	// Since SURF is a floating-point descriptor NORM_L2 is used
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	std::vector< std::vector<DMatch> > knn_matches;
	matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);
	//-- Filter matches using the Lowe's ratio test
	std::vector<DMatch> good_matches;
	for (size_t i = 0; i < knn_matches.size(); i++)
	{
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
		{
			good_matches.push_back(knn_matches[i][0]);
		}
	}
	//-- Draw matches
	drawMatches(imgModel, keypoints1, imgScene, keypoints2, good_matches, imgMatched, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	drawRectsMatchImg(imgModel, imgMatched, good_matches, keypoints1, keypoints2);

	return 1;
}

int CMatchImg::drawRectsMatchImg(Mat& imgModel, Mat& imgMatched, std::vector<DMatch>& good_matches, std::vector<KeyPoint>& keypoints1, std::vector<KeyPoint>& keypoints2)
{
	std::vector<Point2f> model_pt;
	std::vector<Point2f> scene_pt;
	for (int i = 0; i < good_matches.size(); i++)
	{
		model_pt.push_back(keypoints1[good_matches[i].queryIdx].pt);
		scene_pt.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}

	if (model_pt.size() == 0)
	{
		cout << "Could not find match point!\n" << endl;
		return -1;
	}

	std::vector<Point2f> model_corner(4);
	model_corner[0] = Point(0, 0);
	model_corner[1] = Point(imgModel.cols, 0);
	model_corner[2] = Point(imgModel.cols, imgModel.rows);
	model_corner[3] = Point(0, imgModel.rows);

	std::vector<Point2f> scene_corner(4);
	Mat H = findHomography(model_pt, scene_pt, cv::RANSAC);
	perspectiveTransform(model_corner, scene_corner, H);

	Point2f p(imgModel.cols, 0);
	line(imgMatched, scene_corner[0] + p, scene_corner[1] + p, Scalar(0, 0, 255), 3);
	line(imgMatched, scene_corner[1] + p, scene_corner[2] + p, Scalar(0, 0, 255), 3);
	line(imgMatched, scene_corner[2] + p, scene_corner[3] + p, Scalar(0, 0, 255), 3);
	line(imgMatched, scene_corner[3] + p, scene_corner[0] + p, Scalar(0, 0, 255), 3);

	return 1;
}
