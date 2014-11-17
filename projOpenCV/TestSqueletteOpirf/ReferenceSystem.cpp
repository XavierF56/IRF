#include "ReferenceSystem.h"

ReferenceSystem::ReferenceSystem(Point bl, Point tr)
{
	bottomLeft = bl;
	topRight = tr;
	deltaX = topRight.x - bottomLeft.x;
	deltaY = bottomLeft.y - topRight.y;
	int tmp[] = {10, 20, 30};
	listX = vector<int>(tmp, tmp+3);
}

ReferenceSystem::~ReferenceSystem(){

}

vector<Point> ReferenceSystem::getY() {
	vector<Point> ret();

	for(auto it = listY.begin() ; it != listY.end() ; it++)
	{

	}
}

