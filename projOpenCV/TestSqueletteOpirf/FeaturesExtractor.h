#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <regex>
#include"dirent.h"
#include <windows.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"


using namespace std;
using namespace cv;

class FeaturesExtractor
{
	private:
		string source;

	public:
		void extract();

		FeaturesExtractor(string);


		void find_moments( Mat& gray );

		double getDeviation();
		double getAvg();

	private:
		string extractClass(string name);
		list<string> readImage(string name);
		void calculateMoments(string);
		Rect find_boundingBox(Mat img);
};