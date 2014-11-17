#include "ImageAnalyser.h"

ImageAnalyser::ImageAnalyser(int i){
	string imgName = "00000.png";
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
	Mat res = Mat(tailleReduite, CV_8UC3); //cree une image � 3 canaux de profondeur 8 bits chacuns
	resize(input, res, tailleReduite);
	imshow(name, res);
}

void ImageAnalyser::analyse(){
	
	
	cout << img.cols << " " << img.rows << endl;
	
	this->getTopCross();
	this->getBottomCross();

	displayMin(img, "img");
}

void ImageAnalyser::getTopCross()  {
	Mat result;

	// Extract bottom left corner
	Mat subMat(img, Rect(3*img.cols/4, 0, img.cols/4, img.rows/4));

	// Matching cross
	cv::matchTemplate(subMat, cross, result, CV_TM_CCORR_NORMED);
	minMaxLoc(result, NULL, NULL, NULL, &crossBottom);

	// Adjusting with half of image size 
	crossBottom.x += 62;
	crossBottom.y += 58;

	circle(subMat, crossBottom, 10, 2, 10);
}

void ImageAnalyser::getBottomCross()  {
	Mat result;

	// Extract bottom left corner
	Mat subMat(img, Rect(0, 3*img.rows/4, img.cols/4, img.rows/4));
	
	// Matching cross
	cv::matchTemplate(subMat, cross, result, CV_TM_CCORR_NORMED);
	minMaxLoc(result, NULL, NULL, NULL, &crossBottom);

	// Adjusting with half of image size 
	crossBottom.x += 62;
	crossBottom.y += 58;

	circle(subMat, crossBottom, 10, 2, 10);
}

int main(int argc, char* argv[])
{
	ImageAnalyser img(1);
	img.analyse();

	waitKey(0);

	return 0;
}