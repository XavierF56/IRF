#include "ReferenceSystem.h"

ReferenceSystem::ReferenceSystem()
{}

ReferenceSystem::ReferenceSystem(Point bl, Point tr)
{
	widthImage = 0.088;

	bottomLeft = bl;
	topRight = tr;
	deltaX = topRight.x - bottomLeft.x;
	deltaY = bottomLeft.y - topRight.y;

	labelPos = -0.92;

	double tmpY[] = {0.091,  0.217, 0.343,  0.470,  0.595,  0.721,  0.847 };
	listY = vector<double>(tmpY, tmpY+9);

	//double tmpX[] = {0.025, -0.102, -0.154, -0.281, -0.333, -0.460, -0.512, -0.639, -0.691, -0.818};
	//double tmpX[] = {-0.102,  -0.281,  -0.460,  -0.639,  -0.818};

	/*double tmpX[] = {-0.92, -1}; //templates
	listX = vector<double>(tmpX, tmpX+2);
	cout << tmpX[0] << endl;*/

	//Rect roi(topRight.x + (int) (tmpX[1]*deltaX),topRight.y + (int) (tmp[0]*deltaY),165,165);
	//Point a cv::Mat header at it (no allocation is done)
	//Mat image_roi = img(roi);
	//imshow("name", image_roi);

	double tmpX[] = {-0.818, -0.639,-0.460,  -0.281, -0.102};
	listX = vector<double>(tmpX, tmpX+5);
}

ReferenceSystem::~ReferenceSystem(){

}

vector<Point> ReferenceSystem::getY() {
	vector<Point> ret;

	for(auto it = listY.begin() ; it != listY.end() ; it++)
	{
		ret.push_back(Point(topRight.x, topRight.y + (int) (*it*deltaY)));
	}

	return ret;
}

vector<Point> ReferenceSystem::getX() {
	vector<Point> ret;

	for(auto it = listX.begin() ; it != listX.end() ; it++)
	{
		ret.push_back(Point(topRight.x + (int) (*it*deltaX), topRight.y + (int) (0.091*deltaY)));
	}

	return ret;
}


vector<Point> ReferenceSystem::getPoints() {
	vector<Point> ret;

	for(auto ity = listY.begin() ; ity != listY.end() ; ity++)
	{
		for(auto itx = listX.begin() ; itx != listX.end() ; itx++)
		{
			ret.push_back(Point(topRight.x + (int) (*itx*deltaX), topRight.y + (int) (*ity*deltaY)));
		}
	}

	return ret;
}

Point ReferenceSystem::getLabelPosition(int row) {
	return Point(topRight.x + (int) (labelPos*deltaX), topRight.y + (int) (listY[row]*deltaY));
}


int ReferenceSystem::getWidthImage() {
	return (int) (widthImage * deltaY);
}
