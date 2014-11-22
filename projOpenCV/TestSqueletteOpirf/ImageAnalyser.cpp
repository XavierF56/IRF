#include "ImageAnalyser.h"
#include "ReferenceSystem.h"

ImageAnalyser::ImageAnalyser(string imageName){
	string crossName = "cross.png";

	//read input image
	original = imread(imageName);
	img = original;

	if (img.data == NULL){
		cerr << "Image not found: " << imageName << endl;
		exit(0);
	}
	cvtColor(original, img, CV_RGB2GRAY);

	cross = imread(crossName);
	if (img.data == NULL){
		cerr << "Image not found: " << crossName << endl;
		exit(0);
	}

	cvtColor(cross, cross, CV_RGB2GRAY);

	labels = vector<string>(5);
	labels.push_back("hello");
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

	ReferenceSystem ref(crossBottom, crossTop);
	points = ref.getPoints();
	widthImage = ref.getWidthImage();

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
	// calculating ratio, a corretc image should have sqrt(2.0)
	double goodRatio = sqrt(2.0);
	double imgRatio = ((double)crossBottom.y - (double)crossTop.y)/((double)crossTop.x - (double)crossBottom.x);

	double goodAngle = atan(goodRatio) * 180 / PI;
	double imgAngle = atan(imgRatio) * 180 / PI;

	//cout << "Ratio : " << imgRatio << endl << "good : " << goodAngle << endl << "bad : " << imgAngle << endl;
	//cout << "Top : " <<  crossTop << endl << "Bottom : " << crossBottom << endl;

    cv::Mat r = cv::getRotationMatrix2D(crossBottom, goodAngle-imgAngle, 1);
	Mat dst;

	// Rotating greyscaled image
    cv::warpAffine(img, dst, r, cv::Size(img.cols, img.rows));
	img = dst;

	// Rotating original image
	cv::warpAffine(original, dst, r, cv::Size(img.cols, img.rows));
	original = dst;

	// Racalculate topCross after rotation
	this->getTopCross();


	//imgRatio = ((double)crossBottom.y - (double)crossTop.y)/((double)crossTop.x - (double)crossBottom.x);
	//cout << "Ratio : " << imgRatio << endl;
}

void ImageAnalyser::printPoints() {
	for(auto it = points.begin() ; it != points.end() ; it++)
	{
		circle(img, *it, 5, 10, 10);
		cout << *it << endl;
	}
}

Mat ImageAnalyser::extract(int row, int column) {
	int index = (row-1)*5 + column -1;
	Point imagePoint = points[index];

	Mat image_roi = original(Rect(imagePoint.x, imagePoint.y, widthImage, widthImage));
	imshow("name", image_roi);

	return image_roi;
}


string ImageAnalyser::getLabel(int row) {
	return "blabla";
	return labels[row-1];
}



int main2(int argc, char* argv[])
{
	ImageAnalyser img("w000-scans/00001.png");
	img.analyse();
	img.extract(2, 2);

	waitKey(0);

	return 0;
}

