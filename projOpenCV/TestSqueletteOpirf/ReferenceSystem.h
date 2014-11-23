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
		double labelPos;
		double labelWidth;
		double labelHeight;

	public:
		ReferenceSystem();
		ReferenceSystem(Point, Point);
		~ReferenceSystem();
		vector<Rect> getImages();

		Rect getLabel(int row);

		int getWidthImage();
};