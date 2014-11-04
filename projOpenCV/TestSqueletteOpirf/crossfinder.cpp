#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"



#include <iostream>
using namespace std;
using namespace cv;
using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
	//charge et affiche l'image 
	string imName = "00000.png";
	Mat im = imread(imName);
	if (im.data == NULL){
		cerr << "Image not found: " << imName << endl;
		exit(0);
	}

	//applique une reduction de taille d'un facteur 5
	//ici modifier pour ne reduire qu'a l'affichage 
	//comme demande dans l'enonce
	int reduction = 5;
	Size tailleReduite(im.cols / reduction, im.rows / reduction);
	Mat img = Mat(tailleReduite, CV_8UC3); //cree une image à 3 canaux de profondeur 8 bits chacuns
	resize(im, img, tailleReduite);
	

	threshold(img, img, 250.0, 255.0, THRESH_BINARY);

	string outputName = "crosses_detect.png";
	Mat templ = imread("cross.png", 1);
	Size tailleReduite2(templ.cols / reduction, templ.rows / reduction);
	Mat cross = Mat(tailleReduite2, CV_8UC3); //cree une image à 3 canaux de profondeur 8 bits chacuns
	resize(templ, cross, tailleReduite2);

	threshold(cross, cross, 250.0, 255.0, THRESH_BINARY);

	int resultCols = img.cols - cross.cols + 1;
	int resultRows = img.rows - cross.rows + 1;
	Mat result(resultCols, resultRows, CV_32FC1);

	matchTemplate(img, cross, result, CV_TM_CCOEFF);
	normalize(result, result, 0, 255.0, NORM_MINMAX, CV_8UC1, Mat());

	Mat resultMask;
	threshold(result, resultMask, 250.0, 255.0, THRESH_BINARY);

	Mat temp = resultMask.clone();
	vector< vector<Point> > contours;
	findContours(temp, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(cross.cols / 2, cross.rows / 2));

	vector< vector<Point> >::iterator i;
	/*
	i = contours.begin();
	for (i = contours.begin(); i != contours.end(); i++)
	{
		Moments m = moments(*i, false);
		Point2f centroid(m.m10 / m.m00, m.m01 / m.m00);
		circle(img, centroid, 3, Scalar(0, 255, 0), 3);
	}
	*/

	imshow("img", img);
	imshow("results", result);
	imshow("resultMask", resultMask);

	imwrite(outputName, img);

	waitKey(0);

	return 0;
}