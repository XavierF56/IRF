#include "ReferenceSystem.h"

ReferenceSystem::ReferenceSystem(Point bl, Point tr)
{
	bottomLeft = bl;
	topRight = tr;
	deltaX = topRight.x - bottomLeft.x;
	deltaY = bottomLeft.y - topRight.y;

	cout << deltaY << endl;
	float tmp[] = {0.2f , 0.3f, 0.4f};
	listY = vector<float>(tmp, tmp+3);
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

