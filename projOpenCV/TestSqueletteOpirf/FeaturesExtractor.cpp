#include "FeaturesExtractor.h"


int main2(){
	//FeaturesExtractor ext("samples/accident_000_00_1_1.png");
	// Recolte des données
	struct dirent *entry;
	DIR *pDIR;
	string ext = ".png";
	if (pDIR=opendir("samples/")) {
		while(entry = readdir(pDIR)){
			if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				string imageName = entry->d_name;

				// contains ext .png
				if (imageName.find(ext) != string::npos) {
					FeaturesExtractor ext("samples/" + imageName);
				} 
			}
		}
		closedir(pDIR);
	}

	waitKey(0);
	Sleep(100000);
	return 0;
}

FeaturesExtractor::FeaturesExtractor(string source) {
	cout << source << endl;

	this->name = source;
	this->originalImg = imread(name);
	this->binaryImg = Mat(originalImg.rows, originalImg.cols, CV_BGR2GRAY);
	this->greyscaleImg = Mat(originalImg.rows, originalImg.cols, CV_THRESH_BINARY);

	cvtColor(this->originalImg, this->greyscaleImg, CV_BGR2GRAY);
	medianBlur(this->greyscaleImg, this->greyscaleImg, 5);
	threshold(this->greyscaleImg, this->binaryImg, 230, 255, CV_THRESH_BINARY);

	/*imshow("color", originalImg);
	imshow("greyscale", greyscaleImg);
	imshow("binar", binaryImg);*/
	Rect bb = this->find_boundingBox();


	this->greyscaleBox = this->greyscaleImg(bb);
	this->binaryBox = this->binaryImg(bb);
	this->originalBox = this->originalImg(bb);

	/*imshow("color", greyscaleBox);
	imshow("greyscale", binaryBox);
	imshow("binar", originalBox);*/

	Point CoG = getCoG();
	Point half;
	half.x = originalBox.cols/2;
	half.y = originalBox.rows/2;
	//circle(binaryBox, half, 5, Scalar( 200, 200, 200 ), 1, 8, 0);
	circle(binaryBox, CoG, 10, Scalar( 100, 100, 100 ), 1, 8, 0);
	imshow(name, binaryBox);
}

Rect FeaturesExtractor::find_boundingBox() {
	unsigned char *input = (unsigned char*)(binaryImg.data);

	bool stop;
	int topY, bottomY, leftX, rightX;
	
	stop = false;
	for(int i=0; i<binaryImg.rows&&!stop; ++i) {
		for(int j=0; j<binaryImg.cols&&!stop; ++j) {
			if(input[binaryImg.step*j+i]==0) {
				leftX = i;
				stop = true;
			}
		}
	}

	stop = false;
	for(int i=0; i<binaryImg.cols&&!stop; ++i) {
		for(int j=0; j<binaryImg.rows&&!stop; ++j) {
			if(input[binaryImg.step*i+j]==0) {
				topY = i;
				stop = true;
			}
		}
	}

	stop = false;
	for(int i=binaryImg.cols-1;i>=0&&!stop; --i) {
		for(int j=binaryImg.rows-1; j>=0&&!stop; --j) {
			if(input[binaryImg.step*i+j]==0) {
				bottomY = i;
				stop = true;
			}
		}
	}

	stop = false;
	for(int i=binaryImg.rows-1;i>=0&&!stop; --i) {
		for(int j=binaryImg.cols-1; j>=0&&!stop; --j) {
			if(input[binaryImg.step*j+i]==0) {
				rightX = i;
				stop = true;
			}
		}
	}

	return cv::Rect(leftX, topY, (rightX-leftX), (bottomY-topY));
}


string FeaturesExtractor::getClass()
{
	const std::string s = this->name;
    std::regex rgx(".*/([a-z]+).*");
    std::smatch match;

	if (std::regex_search(s.begin(), s.end(), match, rgx))
        return match[1];
}


list<Rect> FeaturesExtractor::getRectDiv(Mat) {
	list<Rect> res;
}


double FeaturesExtractor::getRatioBB() {
	return originalBox.cols / originalBox.rows;
}

double FeaturesExtractor::getRatioColor() {
	return countNonZero(binaryBox) / binaryBox.rows*binaryBox.cols;
}


Point FeaturesExtractor::getCoG() {
	Point center;
	Moments m = moments(binaryBox, false);

	if (m.m00 == 0)
		return center;

	center.x =  (int)(m.m10/m.m00);
	center.y = (int)(m.m01/m.m00); 

	cout << "2x: " << ((double)(m.m10/m.m00)) / (double)binaryBox.cols << " y: " << ((double)(m.m01/m.m00)) / (double)binaryBox.rows << endl;

	return center;
}


void FeaturesExtractor::find_moments(Mat& gray)
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
 
    /// Detect edges using canny
    Canny( gray, canny_output, 50, 150, 3 );
    /// Find contours
    //findContours(canny_output, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	Moments mom = moments( canny_output, true );
}


