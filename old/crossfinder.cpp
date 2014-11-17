#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <math.h>


#include <iostream>
using namespace std;
using namespace cv;
Mat resize(Mat input, int reduction) {
	Size tailleReduite(input.cols / reduction, input.rows / reduction);
	Mat res = Mat(tailleReduite, CV_8UC3); //cree une image à 3 canaux de profondeur 8 bits chacuns
	resize(input, res, tailleReduite);
	return res;
}

Point cross_top;
Point cross_bottom;
Mat img;
Mat cross;

Point getTopCross()  {
	Point res;

	// Extract bottom left corner
	Mat subMat(img, Rect(0, 3*img.rows/4, img.cols/4, img.rows/4));
	imshow("submat", subMat);

	//

	return res;
}

int main2(int argc, char* argv[])
{
	//read input image
	string imName = "00000.png";
	img = imread(imName);
	if (img.data == NULL){
		cerr << "Image not found: " << imName << endl;
		exit(0);
	}
	
	cout << img.cols << " " << img.rows << endl;

	int reduction = 5;
	
	//img = resize(img, reduction);

	

	//threshold(img, img, 250.0, 255.0, THRESH_BINARY);

	string outputName = "crosses_detect.png";
	Mat cross = imread("cross.png", 1);
	//cross = resize(cross, reduction);

	//threshold(cross, cross, 250.0, 255.0, THRESH_BINARY);

	//int resultCols = img.cols - cross.cols + 1;
	//int resultRows = img.rows - cross.rows + 1;

	int resultCols = img.cols;
	int resultRows = img.rows;
	Mat result(resultCols, resultRows, CV_32FC1);

	matchTemplate(img, cross, result, CV_TM_CCOEFF);
	normalize(result, result, 0, 255.0, NORM_MINMAX, CV_8UC1, Mat());

	Mat resultMask;
	threshold(result, resultMask, 250.0, 255.0, THRESH_BINARY);

	Mat temp = resultMask.clone();
	vector< vector<Point> > contours;
	findContours(temp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(cross.cols / 2, cross.rows / 2));

	vector< vector<Point> >::iterator i;
	/*
	i = contours.begin();
	for (i = contours.begin(); i != contours.end(); i++)
	{
		Moments m = moments(*i, false);
		Point2f centroid(m.m10 / m.m00, m.m01 / m.m00);
		circle(img, centroid, 3, Scalar(0, 255, 0), 3);
	}
	*/

	img = resize(img, reduction);
	result = resize(result, reduction);
	resultMask = resize(resultMask, reduction);


	imshow("img", img);
	//imshow("results", result);
	//imshow("resultMask", resultMask);

	getTopCross(img);



	imwrite(outputName, img);

	waitKey(0);

	return 0;
}