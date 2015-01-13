#include "FeaturesExtractor.h"
#include "ArffCreator.h"



int main(){
	FeaturesExtractor ext("samples/");
    ext.extract();

	waitKey(0);
	Sleep(100000);
	return 0;
}

FeaturesExtractor::FeaturesExtractor(string source) {
	this->source = source;
}

void FeaturesExtractor::extract() {
	DIR *pDIR;
	string ext = ".png";

	// Cration du ArffCreator
	ArffCreator ac("test");
	list<list<string>> data;
	list<pair<string, string>> features;	
	
	// Ecriture du Header du fichier Arff
	features.push_back(list<pair<string, string>>::value_type("BBBarycenterX", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("BBBarycenteY", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("BBRatio", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("PixelsRatio", "NUMERIC"));	
	features.push_back(list<pair<string, string>>::value_type("class", "{accident,bomb,car,casualty,electricity,fire,fire_brigade,flood,gas,injury,paramedics,person,police,roadblock}"));

	ac.writeHeader("Features", features);


	// Recolte des données
	struct dirent *entry;
	if (pDIR=opendir(this->source.c_str())) {
		while(entry = readdir(pDIR)){
			if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				string imageName = entry->d_name;

				// contains ext .png
				if (imageName.find(ext) != string::npos) {
					data.push_back(readImage(this->source + imageName));
				} 
			}
		}
		closedir(pDIR);
	}
	
	// Ecriture des donnees dans le fichier Arff
	ac.writeData(data);
}

list<string> FeaturesExtractor::readImage(string name) {	
	cout << name << endl;
	int thresh = 100;
	Mat src = imread(name);

	// Preprocessing
	cvtColor( src, src, CV_BGR2GRAY);
	medianBlur(src, src, 5);

	// does not work
	Rect bb = this->find_boundingBox(src);

	//rectangle(src, bb,  Scalar(0,255,0),1, 8,0);
	src = src(bb);
	imshow(name, src);


	// From main image



	list<string> data;

	// Calcul du barycentre
	Point barycenter(0,0);

	// Calcul du ratio BB
	double BBRatio = getRatioBB(src);
	cout << BBRatio << endl;


	// Calcul du ratio blacks pixels / total pixels
	int pixelsRatio = getRatioColor(src);
	cout << pixelsRatio << endl;


	// Extraction de la classe via le nom
	string classe = name;

	data.push_back(std::to_string((long double)(barycenter.x)));
	data.push_back(std::to_string((long double)(barycenter.y)));	
	data.push_back(std::to_string((long double)(BBRatio)));
	data.push_back(std::to_string((long double)(pixelsRatio)));
	data.push_back(classe);


	return data;
}


list<Rect> FeaturesExtractor::getRectDiv(Mat) {
	list<Rect> res;
}


double FeaturesExtractor::getRatioBB(Mat src) {
	return src.cols / src.rows;
}
double FeaturesExtractor::getRatioColor(Mat src) {
	//threshold(src, src, 230, 255, CV_THRESH_BINARY);
	//imshow("name", src);
	//cout << countNonZero(src) << endl;
	return countNonZero(src) / src.rows*src.cols;
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
}

