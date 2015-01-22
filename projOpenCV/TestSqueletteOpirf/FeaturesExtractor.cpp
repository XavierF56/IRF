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
	//cout << source << endl;

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

	computeCoG();
	/*Point half;
	half.x = originalBox.cols/2;
	half.y = originalBox.rows/2;*/
	//circle(binaryBox, half, 5, Scalar( 200, 200, 200 ), 1, 8, 0);
	circle(binaryBox, coG, 10, Scalar( 100, 100, 100 ), 1, 8, 0);
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
	return (double)originalBox.cols / (double)originalBox.rows;
}

double FeaturesExtractor::getRatioColor() {
	//cout << "non zero " << countNonZero(binaryBox) << endl <<binaryBox.rows << endl << binaryBox.cols;
	return (double) countNonZero(binaryBox) / ((double) binaryBox.rows*binaryBox.cols);
}


void FeaturesExtractor::computeCoG() {
	float sumx=0, sumy=0;
    float num_pixel = 0;
	for(int x=0; x<binaryBox.cols; x++) {
        for(int y=0; y<binaryBox.rows; y++) {
            int val = binaryBox.at<uchar>(y,x);
            if( val == 0) {
                sumx += x;
                sumy += y;
                num_pixel++;
            }
        }
    }
    coG = Point (sumx/num_pixel, sumy/num_pixel);
}

double FeaturesExtractor::getNormalizedCoGX() {
	return (double)coG.x/(double)binaryBox.cols;
}

double FeaturesExtractor::getNormalizedCoGY() {
	return (double)coG.y/(double)binaryBox.rows;
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


