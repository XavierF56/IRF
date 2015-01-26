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
//#include <math.h>


using namespace std;
using namespace cv;

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

	public:
		void extract();

		FeaturesExtractor(string, int);

		double getNormalizedCoGX(int);
		double getNormalizedCoGY(int);
		double getRatioBB(int);
		double getRatioColor(int);
		vector<double> getHuMoments(int);
		string getClass();
		double getMaxProjectionX(int);
		double getMaxProjectionY(int);

	private:
		void calculateMoments(string);
		Rect find_boundingBox();
		void find_moments( Mat& gray );
		void computeCoG();
		void divide();
};