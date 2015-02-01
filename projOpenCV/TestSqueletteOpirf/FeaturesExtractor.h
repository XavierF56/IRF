#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <regex>
#include "dirent.h"
#include <windows.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"

using namespace std;
using namespace cv;


/*
 * FeaturesExtractor class
 * Compute features on one image
 */
class FeaturesExtractor {
	private:
		Mat originalImg;
		Mat greyscaleImg;
		Mat binaryImg;

		vector<Mat> originalBox;
		vector<Mat> greyscaleBox;
		vector<Mat> binaryBox;

		string name;

		vector<Point> coG;

		int div;

		Rect find_boundingBox();
		void computeCoG();
		void divide();

	public:
		FeaturesExtractor(string, int);

		double getNormalizedCoGX(int);
		double getNormalizedCoGY(int);
		double getRatioBB(int);
		double getRatioColor(int);
		double getMaxProjectionX(int);
		double getMaxProjectionY(int);
		vector<double> getHuMoments(int);
		string getClass();
};