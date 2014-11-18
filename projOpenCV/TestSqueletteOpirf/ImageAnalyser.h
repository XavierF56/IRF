#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <math.h>
#include <iostream>

using namespace std;
using namespace cv;

class ImageAnalyser
{
	private:
		Mat img, cross;
		Point crossTop, crossBottom;
		static const int reduction = 2;

	public:
		ImageAnalyser(int);
		~ImageAnalyser();
		void analyse();
		void extract(int pointX, int pointY, int width, int height);

	private:
		void getTopCross();
		void getBottomCross();
		static void displayMin(Mat, string);
		void rotate();
		void printPoints();
};