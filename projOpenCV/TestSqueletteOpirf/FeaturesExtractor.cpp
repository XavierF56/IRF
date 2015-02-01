#include "FeaturesExtractor.h"

/*
int main(){
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
}*/

FeaturesExtractor::FeaturesExtractor(string source, int div) {
	this->div = div;
	this->name = source;
	this->originalImg = imread(name);
	this->binaryImg = Mat(originalImg.rows, originalImg.cols, CV_BGR2GRAY);
	this->greyscaleImg = Mat(originalImg.rows, originalImg.cols, CV_THRESH_BINARY);

	// Preprocessing
	cvtColor(this->originalImg, this->greyscaleImg, CV_BGR2GRAY);
	medianBlur(this->greyscaleImg, this->greyscaleImg, 5);
	GaussianBlur(this->greyscaleImg, this->greyscaleImg, Size(3, 3), 0, 0, BORDER_DEFAULT);
	threshold(this->greyscaleImg, this->binaryImg, 230, 255, CV_THRESH_BINARY);

	// Calculation of BoudingBox
	Rect bb = this->find_boundingBox();

	// Division of imags
	// main image and sub-images are added to a vector
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

	// computation of center of gravity on all (sub) images
	computeCoG();
}


/*
 * Divide image in sub images according to a diving factor
 * Main image and sub-images are added to a vector
 */
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

/*
 * Calculate and return the bounding box of the original image
 */
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

/*
 * Return the class name
 */
string FeaturesExtractor::getClass()
{
	const std::string s = this->name;
    std::regex rgx(".*/([a-z]+(_[a-z]+)?).*");
    std::smatch match;

	if (std::regex_search(s.begin(), s.end(), match, rgx))
        return match[1];
}

/*
 * Return the bounding box ratio
 * @param index of he sub image
 */
double FeaturesExtractor::getRatioBB(int index) {
	return (double)originalBox[index].cols / (double)originalBox[index].rows;
}

/*
 * Return the color ratio
 * @param index of he sub image
 */
double FeaturesExtractor::getRatioColor(int index) {
	return (double) countNonZero(binaryBox[index]) / ((double) binaryBox[index].rows*binaryBox[index].cols);
}

/*
 * Return the Hu Moments
 * @param index of he sub image
 */
vector<double> FeaturesExtractor::getHuMoments(int index) {
	std::vector<double> ret;
	double hu[7];
	cv::HuMoments(cv::moments(binaryBox[index]), hu);
	
	for(int i=0;i<7;++i) {
		ret.push_back(std::powf(10, 4*(i+1)) * hu[i]);
	}
	return ret;
}

/*
 * Compute center of gravity
 */
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
	}
}

/*
 * Return nomalized center of gravity: x coordinate
 * @param index of he sub image
 */
double FeaturesExtractor::getNormalizedCoGX(int index) {
	double ret = (double)coG[index].x/(double)binaryBox[index].cols;
	if (ret < 0 || ret > 1) {
		return 0.5;
	} else {
		return ret;
	}
	
}

/*
 * Return nomalized center of gravity: y coordinate
 * @param index of he sub image
 */
double FeaturesExtractor::getNormalizedCoGY(int index) {
	double ret = (double)coG[index].y/(double)binaryBox[index].rows;
	if (ret < 0 || ret > 1) {
		return 0.5;
	} else {
		return ret;
	}
}

/*
 * Return max histogram projection: x coordinate
 * @param index of he sub image
 */
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
	return (double)indexMax/(double)(binaryBox[index].cols);
}

/*
 * Return max histogram projection: y coordinate
 * @param index of he sub image
 */
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
	return (double)indexMax/(double)(binaryBox[index].rows);
}

