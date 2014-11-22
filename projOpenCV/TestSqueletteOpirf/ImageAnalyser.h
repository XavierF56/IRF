#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <math.h>
#include <iostream>
#define PI 3.1415926

using namespace std;
using namespace cv;

class ImageAnalyser
{
	private:
		Mat img, cross;
		Point crossTop, crossBottom, crossTopMax, crossBottomMax, crossTopMin, crossBottomMin;
		vector<Point> points;
		vector<string> labels;
		static const int reduction = 3;
		int widthImage;

	public:
		ImageAnalyser(int);
		~ImageAnalyser();
		void analyse();
		void extract(int row, int column);
		string getLabel(int row);

	private:
		void getTopCross();
		void getBottomCross();
		static void displayMin(Mat, string);
		void rotate();
		void printPoints();
};