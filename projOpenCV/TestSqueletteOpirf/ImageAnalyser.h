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
		Mat img, cross, original;
		Point crossTop, crossBottom, crossTopMax, crossBottomMax, crossTopMin, crossBottomMin;
		vector<Point> points;
		static const int reduction = 3;
		vector<string> temps;
		vector<string> labels;
		int widthImage;

	public:
		ImageAnalyser(string imageName);
		~ImageAnalyser();
		void analyse();
		Mat extract(int row, int column);
		string getLabel(int row);

	private:
		void getTopCross();
		void getBottomCross();
		void getTemplate();
		static void displayMin(Mat, string);
		void rotate();
		void printPoints();
};