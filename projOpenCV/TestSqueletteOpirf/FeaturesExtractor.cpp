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

	list<string> data;

	// Calcul du barycentre
	Point barycenter(0,0);

	// Calcul du ratio BB
	int BBRatio = 1;


	// Calcul du ratio blacks pixels / total pixels
	int pixelsRatio = 1;

	// Extraction de la classe via le nom
	string classe = name;

	data.push_back(std::to_string((long double)(barycenter.x)));
	data.push_back(std::to_string((long double)(barycenter.y)));	
	data.push_back(std::to_string((long double)(BBRatio)));
	data.push_back(std::to_string((long double)(pixelsRatio)));
	data.push_back(classe);

	return data;
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
}

