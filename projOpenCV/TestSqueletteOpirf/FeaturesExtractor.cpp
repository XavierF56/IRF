#include "FeaturesExtractor.h"


int main2(){
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
					FeaturesExtractor ext("samples/" + imageName, 16);
				} 
			}
		}
		closedir(pDIR);
	}

	waitKey(0);
	Sleep(100000);
	return 0;
}

FeaturesExtractor::FeaturesExtractor(string source, int div) {
	//cout << source << endl;

	this->div = div;

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


	this->originalBox = vector<Mat>(div + 1);
	this->binaryBox = vector<Mat>(div + 1);
	this->greyscaleBox = vector<Mat>(div + 1);

	try {
	this->greyscaleBox[0] = this->greyscaleImg(bb);
	this->binaryBox[0] = this->binaryImg(bb);
	this->originalBox[0] = this->originalImg(bb);
	} catch(...) {
		cout << "wrong " << name << endl; 
	}
		

	divide();
	computeCoG();

	/*Point half;
	half.x = originalBox.cols/2;
	half.y = originalBox.rows/2;*/
	//circle(binaryBox, half, 5, Scalar( 200, 200, 200 ), 1, 8, 0);
	//circle(binaryBox[0], coG, 10, Scalar( 100, 100, 100 ), 1, 8, 0);
	//imshow(name, binaryBox);
}

void FeaturesExtractor::divide() {
	int r = sqrt((double) div);
	int c = r;

	int height = originalBox[0].rows / r;
	int  width = originalBox[0].cols / c;

	int index = 1;

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			Rect div(i*width, j*height, width, height);
			this->greyscaleBox[index] = this->greyscaleBox[0](div);
			this->binaryBox[index] = this->binaryBox[0](div);
			this->originalBox[index] = this->originalBox[0](div);

			index++;
		}
	}
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
    std::regex rgx(".*/([a-z]+(_[a-z]+)?).*");
    std::smatch match;

	if (std::regex_search(s.begin(), s.end(), match, rgx))
        return match[1];
}





double FeaturesExtractor::getRatioBB(int index) {
	return (double)originalBox[index].cols / (double)originalBox[index].rows;
}

double FeaturesExtractor::getRatioColor(int index) {
	return (double) countNonZero(binaryBox[index]) / ((double) binaryBox[index].rows*binaryBox[index].cols);
}

vector<double> FeaturesExtractor::getHuMoments(int index) {
	std::vector<double> ret;
	double hu[7];
	cv::HuMoments(cv::moments(binaryBox[index]), hu);
	
	for(int i=0;i<7;++i) {
		ret.push_back(hu[i]);
	}
	return ret;
}


void FeaturesExtractor::computeCoG() {
	this->coG = vector<Point>(div + 1);
	for (int index = 0; index < div + 1; index++) {
		float sumx=0, sumy=0;
		float num_pixel = 0;
		for(int x=0; x<binaryBox[index].cols; x++) {
			for(int y=0; y<binaryBox[index].rows; y++) {
				int val = binaryBox[index].at<uchar>(y,x);
				if(val == 0) {
					sumx += x;
					sumy += y;
					num_pixel++;
				}
			}
		}
		coG[index] = Point (sumx/num_pixel, sumy/num_pixel);
		/*circle(binaryBox[index], coG[index], 5, Scalar( 200, 200, 200 ), 1, 8, 0);
		imshow(index +" " + name, binaryBox[index]);*/
	}
}

double FeaturesExtractor::getNormalizedCoGX(int index) {
	return (double)coG[index].x/(double)binaryBox[index].cols;
}

double FeaturesExtractor::getNormalizedCoGY(int index) {
	return (double)coG[index].y/(double)binaryBox[index].rows;
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

double FeaturesExtractor::getMaxProjectionX(int index)
{
	int valMax = 0;
	int indexMax;
	for(int x=0; x<binaryBox[index].cols; x++) {
		int sumx = 0;
		for(int y=0; y<binaryBox[index].rows; y++) {
			int val = binaryBox[index].at<uchar>(y,x);
			if(val == 0) {
				sumx++;
			}
		}
		if(sumx >= valMax)
		{
			valMax = sumx;
			indexMax = x;
		}
	}
	cout << "IndexX " << indexMax << " / " << binaryBox[index].cols << endl;
	return (double)indexMax/(double)(binaryBox[index].cols);
}

double FeaturesExtractor::getMaxProjectionY(int index)
{
	int sumy = 0;
	int valMax = 0;
	int indexMax;
	for(int y=0; y<binaryBox[index].rows; y++) {
		int sumy = 0;
		for(int x=0; x<binaryBox[index].cols; x++) {			
			int val = binaryBox[index].at<uchar>(y,x);
			if(val == 0) {
				sumy++;
			}
		}
		if(sumy >= valMax)
		{
			valMax = sumy;
			indexMax = sumy;
		}
	}	
	cout << "IndexY " << indexMax << " / " << binaryBox[index].rows << endl;
	return (double)indexMax/(double)(binaryBox[index].rows);
}

