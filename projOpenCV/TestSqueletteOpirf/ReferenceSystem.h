#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <math.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

class ReferenceSystem
{
	private:
		Point topRight, bottomLeft;
		int deltaX, deltaY, xSymbol, sizeSymbol;
		double widthImage;
		vector<double> listX, listY, listSymbol;

	public:
		ReferenceSystem(Point, Point);
		~ReferenceSystem();
		vector<Point> getPoints();
		vector<Point> getPointsSymbols();
		vector<Point> getY();
		vector<Point> getX();

		int getWidthImage();

};