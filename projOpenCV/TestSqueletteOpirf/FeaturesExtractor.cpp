#include "FeaturesExtractor.h"



int main(){
	FeaturesExtractor ext("samples/");
    ext.extract();

	waitKey(0);
	Sleep(100000);
	return 0;
}

FeaturesExtractor::FeaturesExtractor(string source) {
	this->source = source;
	list = vector<double>();
}

void FeaturesExtractor::extract() {
	DIR *pDIR;
	string ext = ".png";

	struct dirent *entry;
	if (pDIR=opendir(this->source.c_str())) {
		while(entry = readdir(pDIR)){
			if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				string imageName = entry->d_name;

				// contains ext .png
				if (imageName.find(ext) != string::npos) {
					readImage(this->source + imageName);
				} 
			}
		}
		closedir(pDIR);
	}
}

void FeaturesExtractor::readImage(string name) {
	Mat src = imread(name);

	// Preprocessing
	cvtColor( src, src, CV_BGR2GRAY);
	medianBlur(src, src, 5);
	Rect bb = this->find_boundingBox(src);

	//rectangle(src, bb,  Scalar(0,255,0),1, 8,0);
	src = src(bb);

	// From main image

	// dividing in sub image
	int colM = src.cols / 2

	imshow(name, src);

	//find_moments(src);
}


list<Rect> FeaturesExtractor::getRectDiv(Mat) {
	list<Rect> res;


}


Rect FeaturesExtractor::find_boundingBox(Mat src)
{
	threshold(src, src, 230, 255, CV_THRESH_BINARY);

	unsigned char *input = (unsigned char*)(src.data);

	bool stop;
	int topY, bottomY, leftX, rightX;
	
	stop = false;
	for(int i=0; i<src.rows&&!stop; ++i) {
		for(int j=0; j<src.cols&&!stop; ++j) {
			if(input[src.step*j+i]==0) {
				leftX = i;
				stop = true;
			}
		}
	}

	stop = false;
	for(int i=0; i<src.cols&&!stop; ++i) {
		for(int j=0; j<src.rows&&!stop; ++j) {
			if(input[src.step*i+j]==0) {
				topY = i;
				stop = true;
			}
		}
	}

	stop = false;
	for(int i=src.cols-1;i>=0&&!stop; --i) {
		for(int j=src.rows-1; j>=0&&!stop; --j) {
			if(input[src.step*i+j]==0) {
				bottomY = i;
				stop = true;
			}
		}
	}

	stop = false;
	for(int i=src.rows-1;i>=0&&!stop; --i) {
		for(int j=src.cols-1; j>=0&&!stop; --j) {
			if(input[src.step*j+i]==0) {
				rightX = i;
				stop = true;
			}
		}
	}

	return cv::Rect(leftX, topY, (rightX-leftX), (bottomY-topY));
}

void find_moments( Mat& gray );


RNG rng(12345);

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
	list.push_back(mom.m00);
}

