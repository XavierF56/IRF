#include "ReferenceSystem.h"

ReferenceSystem::ReferenceSystem(Point bl, Point tr)
{
	bottomLeft = bl;
	topRight = tr;
	deltaX = topRight.x - bottomLeft.x;
	deltaY = bottomLeft.y - topRight.y;

	
	cout << deltaY << endl;
	double tmp[] = {0.091, 0.182, 0.217, 0.308, 0.343, 0.434, 0.470, 0.561, 0.596, 0.687 };
	listY = vector<double>(tmp, tmp+10);
	
	cout << deltaX << endl;
	double tmpX[] = {0.0255, -0.105, -0.155, -0.285, -0.335, -0.464, -0.512, -0.641, -0.692, -0.821};
	listX = vector<double>(tmpX, tmpX+10);


}

ReferenceSystem::~ReferenceSystem(){

}

vector<Point> ReferenceSystem::getY() {
	vector<Point> ret;

	for(auto it = listY.begin() ; it != listY.end() ; it++)
	{
		ret.push_back(Point(topRight.x, topRight.y + (int) (*it*deltaY)));
		cout << Point(topRight.x, topRight.y + (int) (*it*deltaY)) << endl;
	}

	return ret;
}

vector<Point> ReferenceSystem::getX() {
	vector<Point> ret;

	for(auto it = listX.begin() ; it != listX.end() ; it++)
	{
		ret.push_back(Point(topRight.x + (int) (*it*deltaX), topRight.y + (int) (0.091*deltaY)));
		cout << Point(topRight.x + (int) (*it*deltaX), topRight.y + (int) (0.091*deltaY)) << endl;
	}

	return ret;
}


