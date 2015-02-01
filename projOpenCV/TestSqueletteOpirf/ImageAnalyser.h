#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <math.h>
#include <iostream>
#define PI 3.1415926
#include "ReferenceSystem.h"

using namespace std;
using namespace cv;


/*
 * ImageAnalyser class
 * Extract "Imagette" from one form
 */
class ImageAnalyser
{
	private:
		Mat img, cross, original;
		Point crossTop, crossBottom;
		
		static const int reduction = 3;
		static const int rowN = 7;
		static const int columnN = 5;
		vector<string> temps;
		
		vector<Rect> images;
		vector<string> labels;
		int widthImage;

		ReferenceSystem ref;

		bool correct;


	public:
		ImageAnalyser(string imageName);
		~ImageAnalyser();
		Mat extract(int row, int column);
		string getLabel(int row);
		bool isCorrect();

	private:
		bool getTemplate();
		void preprocess();
		void getTopCross();
		void getBottomCross();
		void rotate();
		void printPoints();
		static void displayMin(Mat, string);
};