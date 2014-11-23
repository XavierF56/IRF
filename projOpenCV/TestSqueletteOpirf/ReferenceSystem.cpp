#include "ReferenceSystem.h"

ReferenceSystem::ReferenceSystem()
{}

ReferenceSystem::ReferenceSystem(Point bl, Point tr)
{
	bottomLeft = bl;
	topRight = tr;
	deltaX = topRight.x - bottomLeft.x;
	deltaY = bottomLeft.y - topRight.y;

	// labels
	labelPos = -1.01;
	labelWidth = 0.1;
	labelHeight = 0.068;

	// Images
	widthImage = 0.088;

	double tmpY[] = {0.091,  0.217, 0.343,  0.470,  0.595,  0.721,  0.847 };
	listY = vector<double>(tmpY, tmpY+7);

	double tmpX[] = {-0.818, -0.639,-0.460,  -0.281, -0.102};
	listX = vector<double>(tmpX, tmpX+5);
}

ReferenceSystem::~ReferenceSystem(){

}

vector<Rect> ReferenceSystem::getImages() {
	vector<Rect> ret;

	for(auto ity = listY.begin() ; ity != listY.end() ; ity++)
	{
		for(auto itx = listX.begin() ; itx != listX.end() ; itx++)
		{
			ret.push_back(Rect(topRight.x + (int) (*itx*deltaX), topRight.y + (int) (*ity*deltaY), widthImage*deltaY, widthImage*deltaY));
		}
	}

	return ret;
}

Rect ReferenceSystem::getLabel(int row) {
	return Rect(topRight.x + (int) (labelPos*deltaX), topRight.y + (int) (listY[row]*deltaY), 250, 250);
}


int ReferenceSystem::getWidthImage() {
	return (int) (widthImage * deltaY);
}
