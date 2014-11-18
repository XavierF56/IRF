#include "ReferenceSystem.h"

ReferenceSystem::ReferenceSystem(Point bl, Point tr)
{
	bottomLeft = bl;
	topRight = tr;
	deltaX = topRight.x - bottomLeft.x;
	deltaY = bottomLeft.y - topRight.y;

	cout << deltaY << endl;
	double tmp[] = {0.091, 0.181, 0.217, 0.307, 0.343, 0.433, 0.470, 0.560, 0.595, 0.685, 0.721, 0.811, 0.847, 0.937 };
	listY = vector<double>(tmp, tmp+14);
}

ReferenceSystem::~ReferenceSystem(){

}

vector<Point> ReferenceSystem::getY() {
	vector<Point> ret;

	for(auto it = listY.begin() ; it != listY.end() ; it++)
	{
		ret.push_back(Point(topRight.x, topRight.y + (int) (*it*deltaY)));
		//cout << Point(topRight.x, topRight.y + (int) (*it*deltaY)) << endl;
	}

	return ret;
}

