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
	//cout << name << endl;
	int thresh = 100;
	Mat src = imread(name);

	// Preprocessing
	cvtColor( src, src, CV_BGR2GRAY);

	medianBlur(src, src, 5);
	//Rect bou = this->find_boundingBox(src);
	//rectangle(src, bou,  Scalar(0,255,0),1, 8,0);

	//imshow(name, src);

	//find_moments(src);

	Point CoG = find_CoG(src);
	Point half;
	half.x = src.cols/2;
	half.y = src.rows/2;
	circle(src, half, 5, Scalar( 200, 200, 200 ), 1, 8, 0);
	circle(src, CoG, 10, Scalar( 100, 100, 100 ), 1, 8, 0);
	imshow(name, src);
}

Rect FeaturesExtractor::find_boundingBox(Mat src)
{
	// double for on mat
	int largest_area = 0;
	int largest_contour_index=0;
	Rect bounding_rect;
	
	int thresh = 200;
	int max_thresh = 255;
	RNG rng(12345);

	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// Detect edges using Threshold
	threshold(src, threshold_output, thresh, 255, THRESH_BINARY );
	/// Find contours
	findContours(threshold_output, contours, CV_RETR_LIST,CV_CHAIN_APPROX_NONE);
	/*cout << contours.size() << endl;
	/*
	for( size_t i = 0; i < contours.size(); i++ ) 
	{ 
		if(contours[i].size() != 0)
		{
			double a = contourArea(contours[i], false);  //  Find the area of contour
			if(a > largest_area)
			{
				largest_area=a;
				largest_contour_index=i;                //Store the index of largest contour
				//bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
			}
		}
	}

	Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
	//drawContours( src, contours,largest_contour_index, color, CV_FILLED, 8, hierarchy ); // Draw the largest contour using previously stored index.
	*/
	cout << "Rectangle : " << bounding_rect << endl;
	return bounding_rect;
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

Point FeaturesExtractor::find_CoG(Mat& picture)
{
	
	// http://opencvexamples.blogspot.com/2013/10/calculating-moments-of-image.html#.VLU9tmNZjvw
	Mat binary;
	Point center;
	threshold(picture, binary, 200, 255, THRESH_BINARY);

	double m00, m10, m01;
	CvMoments moment = moments(binary, 1);
	m00 = moment.m00;
	if (m00 == 0) {
		return center;
	}
	m10 = moment.m10;
	m01 = moment.m01;
	center.x = (int) (m10/m00);
	center.y = (int) (m01/m00); 
	
	cout << "2x: " << center.x << " y: " << center.y << endl;

	return center;
}

