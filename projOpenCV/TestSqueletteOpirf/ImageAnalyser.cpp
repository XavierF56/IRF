#include "ImageAnalyser.h"
#include "ReferenceSystem.h"

ImageAnalyser::ImageAnalyser(int i){
	//string imgName = "w000-scans/00000.png";
	string imgName = "hello.png";
	string crossName = "cross.png";

	//read input image
	img = imread(imgName);
	if (img.data == NULL){
		cerr << "Image not found: " << imgName << endl;
		exit(0);
	}
	cvtColor(img, img, CV_RGB2GRAY);

	cross = imread(crossName);
	if (img.data == NULL){
		cerr << "Image not found: " << crossName << endl;
		exit(0);
	}

	cvtColor(cross, cross, CV_RGB2GRAY);

	

	//threshold(img, img, 250.0, 255.0, THRESH_BINARY);
	//threshold(cross, cross, 250.0, 255.0, THRESH_BINARY);
}

ImageAnalyser::~ImageAnalyser(){

}

void ImageAnalyser::displayMin(Mat input, string name) {
	Size tailleReduite(input.cols / reduction, input.rows / reduction);
	Mat res = Mat(tailleReduite, CV_8UC3); //cree une image à 3 canaux de profondeur 8 bits chacuns
	resize(input, res, tailleReduite);
	imshow(name, res);


}

void ImageAnalyser::analyse(){
	
	
	this->getTopCross();
	this->getBottomCross();
	this->rotate();
	

	circle(img, crossBottom, 10, 2, 10);

	circle(img, crossTop, 10, 2, 10);

	printPoints();

	displayMin(img, "img");
}

void ImageAnalyser::getTopCross()  {
	Mat result;

	// Extract bottom left corner
	Mat subMat(img, Rect(3*img.cols/4, 0, img.cols/4, img.rows/4));

	// Matching cross
	cv::matchTemplate(subMat, cross, result, CV_TM_CCORR_NORMED);
	minMaxLoc(result, NULL, NULL, NULL, &crossTop);

	// Adjusting with half of image size 
	crossTop.x += 3*img.cols/4 + cross.cols/2;
	crossTop.y += cross.rows/2;
}

void ImageAnalyser::getBottomCross()  {
	Mat result;

	// Extract bottom left corner
	Mat subMat(img, Rect(0, 3*img.rows/4, img.cols/4, img.rows/4));
	
	// Matching cross
	cv::matchTemplate(subMat, cross, result, CV_TM_CCORR_NORMED);
	minMaxLoc(result, NULL, NULL, NULL, &crossBottom);

	// Adjusting with half of image size 
	crossBottom.x += cross.cols/2;
	crossBottom.y += 3*img.rows/4 + cross.rows/2;
}

void ImageAnalyser::rotate() {
	//todo create max min points from minMaxLoc and use it to rotate
	// recalculate top cross for new repere

	double goodRatio = sqrt(2.0);
	double imgRatio = ((double)crossBottom.y - (double)crossTop.y)/((double)crossTop.x - (double)crossBottom.x);

	double goodAngle = atan(goodRatio) * 180 / PI;
	double imgAngle = atan(imgRatio) * 180 / PI;

	//cout << "Ratio : " << imgRatio << endl << "good : " << goodAngle << endl << "bad : " << imgAngle << endl;
	//cout << "Top : " <<  crossTop << endl << "Bottom : " << crossBottom << endl;

    cv::Mat r = cv::getRotationMatrix2D(crossBottom, goodAngle-imgAngle, 1);

	Mat dst;

    cv::warpAffine(img, dst, r, cv::Size(img.cols, img.rows));

	//displayMin(dst, "dst");
	
	// Replace old image with rotated 
	img = dst;

	// Racalculate topCross after rotation
	this->getTopCross();


	imgRatio = ((double)crossBottom.y - (double)crossTop.y)/((double)crossTop.x - (double)crossBottom.x);
	cout << "Ratio : " << imgRatio << endl;
}

void ImageAnalyser::printPoints() {
	ReferenceSystem ref(crossBottom, crossTop);
	vector<Point> vec = ref.getY();
	for(auto it = vec.begin() ; it != vec.end() ; it++)
	{
		circle(img, *it, 2, 200, 10);
	}

	vector<Point> vec1 = ref.getX();
	for(auto it = vec1.begin() ; it != vec1.end() ; it++)
	{
		circle(img, *it, 2, 200, 10);
	}
}

void ImageAnalyser::extract(int pointX, int pointY, int width, int height) {
	/*int pointX = 628;
	int pointY = 783;
	int width = 235;
	int height = 235;*/

	//Make a rectangle
	Rect roi(pointX,pointY,width,height);
	//Point a cv::Mat header at it (no allocation is done)
	Mat image_roi = img(roi);
	imshow("name", image_roi);
	string filename = "C:/Users/rjahn/Desktop/test/00001.png";
	imwrite(filename, image_roi);

}



int main(int argc, char* argv[])
{
	ImageAnalyser img(1);
	img.analyse();
	img.extract(628, 783, 235, 235);

	waitKey(0);

	return 0;
}