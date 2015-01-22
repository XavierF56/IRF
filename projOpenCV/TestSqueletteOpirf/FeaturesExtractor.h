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

class FeaturesExtractor {
	private:
		Mat originalImg;
		Mat greyscaleImg;
		Mat binaryImg;

		Mat originalBox;
		Mat greyscaleBox;
		Mat binaryBox;
		string name;

		Point coG;

	public:
		void extract();

		FeaturesExtractor(string);

		double getNormalizedCoGX();
		double getNormalizedCoGY();
		double getRatioBB();
		double getRatioColor();
		string getClass();
		list<Rect> getRectDiv(Mat);

	private:
		void calculateMoments(string);
		Rect find_boundingBox();
		void find_moments( Mat& gray );
		void computeCoG();
};