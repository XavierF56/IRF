#include "ExtractionVerifier.h"


int main()
{
	ExtractionVerifier ver("Verif\\Result\\");
	ver.verifyAll("Verif\\ToCheck\\");
	//waitKey(0);
	//Sleep(100000);
}


ExtractionVerifier::ExtractionVerifier(string source)
{
	this->source = source;
}

void ExtractionVerifier::verifyAll(string destination)
{
	DIR *pDIR;
	string ext = ".png";

	struct dirent *entry;
	if (pDIR=opendir(this->source.c_str())) {
		while(entry = readdir(pDIR)){
			if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				string imageName = entry->d_name;

				// contains ext .png
				if (imageName.find(ext) != string::npos)
				{
					Mat src = imread(this->source + imageName);
					if (verifyImage(src)){
						imwrite(destination + imageName, src);
					}
				} 
			}
		}
		closedir(pDIR);
	}
}

bool ExtractionVerifier::verifyImage(Mat src)
{
	imshow("test", src);
	Mat binary(src.rows, src.cols, CV_THRESH_BINARY);
	cvtColor(src, src, CV_BGR2GRAY);
	threshold(src, binary, 230, 255, CV_THRESH_BINARY);
	//imshow("testbinary", binary);
	bool stop;

	unsigned char *input = (unsigned char*)(binary.data);
	
	stop = false;
	for(int i=1; i<binary.rows&&!stop; ) {
		if(input[i] = 0 || input[i*binary.step] == 0 /*|| input[src.step*(src.step-1) + i] == 0*/ || input[++i*binary.step - 1] == 0)
		{
			return true;
		}
	}
	return false;
}