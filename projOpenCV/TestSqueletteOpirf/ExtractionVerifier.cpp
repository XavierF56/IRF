#include "ExtractionVerifier.h"

int main()
{
	ExtractionVerifier ver("Verif\\ResultsExtraction\\");
	ver.verifyAll("Verif\\ToCheck\\");

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
					if (verifyImage(src))
						imwrite(destination + imageName, src);
				} 
			}
		}
		closedir(pDIR);
	}
}

bool ExtractionVerifier::verifyImage(Mat src)
{
	threshold(src, src, 230, 255, CV_THRESH_BINARY);

	bool stop;

	unsigned char *input = (unsigned char*)(src.data);
	
	stop = false;
	for(int i=1; i<src.rows&&!stop; ) {
		if(input[i] = 0 || input[i*src.step] == 0 /*|| input[src.step*(src.step-1) + i] == 0*/ || input[++i*src.step - 1] == 0)
		{
			return true;
		}
	}
	return false;
}