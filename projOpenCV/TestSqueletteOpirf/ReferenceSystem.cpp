#include "ReferenceSystem.h"

ReferenceSystem::ReferenceSystem(Point bl, Point tr)
{
	string imgName = "w000-scans/00003.png";

	//read input image
	Mat img = imread(imgName);

	bottomLeft = bl;
	topRight = tr;
	deltaX = topRight.x - bottomLeft.x;
	deltaY = bottomLeft.y - topRight.y;

	
	cout << deltaY << endl;
	//double tmp[] = {0.091, 0.181, 0.217, 0.307, 0.343, 0.433, 0.470, 0.560, 0.595, 0.685, 0.721, 0.811, 0.847, 0.937 };
	//double tmp[] = {0.091,  0.217, 0.343,  0.470,  0.595,  0.721,  0.847 };
	double tmp[] = {0.091,  0.16, 0.217, 0.343, 0.412, 0.470,  0.595,  0.721,  0.847 }; //templates
	listY = vector<double>(tmp, tmp+9);
	cout << tmp[0] << endl;
	
	cout << deltaX << endl;
	//double tmpX[] = {0.025, -0.102, -0.154, -0.281, -0.333, -0.460, -0.512, -0.639, -0.691, -0.818};
	//double tmpX[] = {-0.102,  -0.281,  -0.460,  -0.639,  -0.818};
	double tmpX[] = {-0.92, -1}; //templates
	listX = vector<double>(tmpX, tmpX+2);
	cout << tmpX[0] << endl;

	//Rect roi(topRight.x + (int) (tmpX[1]*deltaX),topRight.y + (int) (tmp[0]*deltaY),165,165);
	//Point a cv::Mat header at it (no allocation is done)
	//Mat image_roi = img(roi);
	//imshow("name", image_roi);
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

vector<Point> ReferenceSystem::getX() {
	vector<Point> ret;

	for(auto it = listX.begin() ; it != listX.end() ; it++)
	{
		ret.push_back(Point(topRight.x + (int) (*it*deltaX), topRight.y + (int) (0.091*deltaY)));
		cout << Point(topRight.x + (int) (*it*deltaX), topRight.y + (int) (0.091*deltaY)) << endl;
	}

	return ret;
}


vector<Point> ReferenceSystem::getPoints() {
	vector<Point> ret;

	for(auto itx = listX.begin() ; itx != listX.end() ; itx++)
	{
		for(auto ity = listY.begin() ; ity != listY.end() ; ity++)
		{
			ret.push_back(Point(topRight.x + (int) (*itx*deltaX), topRight.y + (int) (*ity*deltaY)));
		}
	}

	return ret;
}

