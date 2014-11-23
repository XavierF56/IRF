#include "ImageAnalyser.h"



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

	
	this->preprocess();

	// get points
	ref = ReferenceSystem(crossBottom, crossTop);
	points = ref.getPoints();
	widthImage = ref.getWidthImage();

	//this->getTemplate();

	
	printPoints();
	displayMin(img, "img");
}

ImageAnalyser::~ImageAnalyser(){

}



void ImageAnalyser::preprocess(){
	// transforme image
	this->getTopCross();
	this->getBottomCross();
	this->rotate();
	
	
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
	// calculate image ratio, the correct should be sqrt(2.0)
	double goodRatio = sqrt(2.0);
	double imgRatio = ((double)crossBottom.y - (double)crossTop.y)/((double)crossTop.x - (double)crossBottom.x);

	// calculate angles from ratio
	double goodAngle = atan(goodRatio) * 180 / PI;
	double imgAngle = atan(imgRatio) * 180 / PI;
	double correctionAngle = goodAngle - imgAngle;

	//cout << "Ratio : " << imgRatio << endl << "good : " << goodAngle << endl << "bad : " << imgAngle << endl;
	//cout << "Top : " <<  crossTop << endl << "Bottom : " << crossBottom << endl;

	// create transformation
    cv::Mat r = cv::getRotationMatrix2D(crossBottom, correctionAngle, 1);
	Mat dst;

	// apply transformation on greyscaled image
    cv::warpAffine(img, dst, r, cv::Size(img.cols, img.rows));
	img = dst;

	// apply transformation on original image
	cv::warpAffine(original, dst, r, cv::Size(img.cols, img.rows));
	original = dst;

	// Racalculate topCross after rotation
	this->getTopCross();


	//imgRatio = ((double)crossBottom.y - (double)crossTop.y)/((double)crossTop.x - (double)crossBottom.x);
	//cout << "Ratio : " << imgRatio << endl;
}

void ImageAnalyser::getTemplate()  {
	labels = vector<string>(5);

	string tmp[] = {"accident", "bomb", "car", "casualty", "electricity", "fire", "fire_brigade", "flood", "gas", "injury", "paramedics", "person", "police", "roadblock"};
	temps = vector<string>(tmp, tmp+14);


	Mat result;
	ReferenceSystem ref(crossBottom, crossTop);
	double maxval;
	string bestMatch;
	double bestVal;
	for (int i = 0; i < 14; i++) {
		Mat temp = imread("templates/" + temps[i] + ".png");
		if (temp.data == NULL){
			cerr << "Image not found: " << temps[i] << endl;
			exit(0);
		}
		cvtColor(temp, temp, CV_RGB2GRAY);
	

		Rect roi((int) (ref.getX()[1].x), (int) (ref.getY()[0].y),165,165);
		//Point a cv::Mat header at it (no allocation is done)
		Mat image_roi = img(roi);

		//Mat subMat(img, Rect(0, 3*img.rows/4, img.cols/4, img.rows/4));

		cv::matchTemplate(image_roi, temp, result, CV_TM_CCORR_NORMED);
		minMaxLoc(result, NULL, &maxval, NULL, NULL);
		if(maxval > bestVal) {
			bestVal = maxval;
			bestMatch = temps[i];
		}
	}
	cout << bestMatch << endl;
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

int ImageAnalyser::getWidth() {
	return widthImage;
}

void ImageAnalyser::printPoints() { // test method
	for(auto it = points.begin() ; it != points.end() ; it++)
	{
		circle(img, *it, 5, 10, 10);
		cout << *it << endl;
	}
	circle(img, crossBottom, 10, 2, 10);
	circle(img, crossTop, 10, 2, 10);

	for(int j = 0 ; j < 7 ; j++)
	{
		rectangle(img, ref.getLabel(j), 10, 2, 10);
	}
}

void ImageAnalyser::displayMin(Mat input, string name) {
	Size tailleReduite(input.cols / reduction, input.rows / reduction);
	Mat res = Mat(tailleReduite, CV_8UC3); //cree une image à 3 canaux de profondeur 8 bits chacuns
	resize(input, res, tailleReduite);
	imshow(name, res);
}

int main(int argc, char* argv[])
{
	ImageAnalyser img("w000-scans/00001.png");
	img.extract(2, 2);

	waitKey(0);

	return 0;
}

