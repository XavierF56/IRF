#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include <string>
#include<fstream>
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
		vector<double> list;


	public:
		void listFile();
		FeaturesExtractor(string);
		void find_moments( Mat& gray );

		double getDeviation();
		double getAvg();

	private:
		void readImage(string name);
		void calculateMoments(string);
};