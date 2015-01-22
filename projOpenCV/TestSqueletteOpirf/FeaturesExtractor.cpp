#include "FeaturesExtractor.h"
#include "ArffCreator.h"



int main(){
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
/*
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
*/
/*
list<string> FeaturesExtractor::readImage(string name) {



	/*Point CoG = find_CoG(src);
	Point half;
	half.x = src.cols/2;
	half.y = src.rows/2;
	circle(src, half, 5, Scalar( 200, 200, 200 ), 1, 8, 0);
	circle(src, CoG, 10, Scalar( 100, 100, 100 ), 1, 8, 0);
	imshow(name, src);*/

	// does not work
	/*Rect bb = this->find_boundingBox(src);

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
	string classe = extractClass(name);

	data.push_back(std::to_string((long double)(barycenter.x)));
	data.push_back(std::to_string((long double)(barycenter.y)));	
	data.push_back(std::to_string((long double)(BBRatio)));
	data.push_back(std::to_string((long double)(pixelsRatio)));
	data.push_back(classe);


	return data;
}

*/

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


double FeaturesExtractor::getRatioBB(Mat src) {
	return src.cols / src.rows;
}

double FeaturesExtractor::getRatioColor(Mat src) {
	//threshold(src, src, 230, 255, CV_THRESH_BINARY);
	//imshow("name", src);
	//cout << countNonZero(src) << endl;
	return countNonZero(src) / src.rows*src.cols;
}


Point FeaturesExtractor::getCoG()
{
	
	// http://opencvexamples.blogspot.com/2013/10/calculating-moments-of-image.html#.VLU9tmNZjvw
	Point center;


	double m00, m10, m01;
	CvMoments moment = moments(binaryImg, 1);
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


