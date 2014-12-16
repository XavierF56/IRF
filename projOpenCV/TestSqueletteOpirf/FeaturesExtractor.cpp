#include "FeaturesExtractor.h"



int main(){
	FeaturesExtractor ext("Accident/");
    ext.listFile();
	cout << "Accident : " << ext.getAvg() << " +/- " << ext.getDeviation() << " %" << ext.getDeviation() /ext.getAvg() << endl;

	FeaturesExtractor ext1("Gaz/");
    ext1.listFile();
	cout << "Gaz : " << ext1.getAvg() << " +/- " << ext1.getDeviation() << " %" << ext1.getDeviation() /ext1.getAvg()  << endl;

	FeaturesExtractor ext2("Fire/");
    ext2.listFile();
	cout << "Fire : " << ext2.getAvg() << " +/- " << ext2.getDeviation() << " %" << ext2.getDeviation() /ext2.getAvg()  << endl;

	cout << ( abs(ext.getDeviation() /ext.getAvg()) + abs(ext1.getDeviation() /ext1.getAvg()) + abs(ext2.getDeviation() /ext2.getAvg())) /3 << endl; 

	waitKey(0);
	Sleep(100000);
	return 0;
}

FeaturesExtractor::FeaturesExtractor(string source) {
	this->source = source;
	list = vector<double>();
}

void FeaturesExtractor::listFile() {
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

void find_moments( Mat& gray );

void FeaturesExtractor::readImage(string name) {
	//cout << name << endl;
	int thresh = 100;
	Mat src = imread(name);
	/// Convert image to gray and blur it
	cvtColor( src, src, CV_BGR2GRAY );
	blur(src, src, Size(3,3) );
	//imshow(name, src);

	find_moments(src);
}

RNG rng(12345);

void FeaturesExtractor::find_moments( Mat& gray )
{
    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
 
    /// Detect edges using canny
    Canny( gray, canny_output, 50, 150, 3 );
    /// Find contours
    //findContours(canny_output, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	Moments mom = moments( canny_output, true );
	list.push_back( mom.nu12);
}

double FeaturesExtractor::getDeviation()
{
    double avg = 0;
	for(auto it = list.begin(); it != list.end(); it++) {
		avg += *it;
		//cout << *it << endl;
	}
	avg = avg / list.size();
	double deviation = 0;
	for(auto it = list.begin(); it != list.end(); it++) {
		deviation += abs(avg - *it);
		//cout << deviation << endl;
	}
	deviation = deviation / list.size();
	return deviation;
}

double FeaturesExtractor::getAvg()
{
    double avg = 0;
	for(auto it = list.begin(); it != list.end(); it++) {
		avg += *it;
	}
	avg = avg / list.size();
	return avg;
}

