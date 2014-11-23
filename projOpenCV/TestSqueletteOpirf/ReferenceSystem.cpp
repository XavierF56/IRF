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

	labelPos = -1;
	labelWidth = 0.080;
	labelHeight = 0.068;

	double tmpY[] = {0.091,  0.217, 0.343,  0.470,  0.595,  0.721,  0.847 };
	listY = vector<double>(tmpY, tmpY+7);

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

Rect ReferenceSystem::getLabel(int row) {
	return Rect(topRight.x + (int) (labelPos*deltaX), topRight.y + (int) (listY[row]*deltaY), labelWidth*deltaX, labelHeight*deltaY);
}


int ReferenceSystem::getWidthImage() {
	return (int) (widthImage * deltaY);
}
