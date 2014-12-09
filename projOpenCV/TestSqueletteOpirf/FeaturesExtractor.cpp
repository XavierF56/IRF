#include "FeaturesExtractor.h"


int main(){
	FeaturesExtractor ext("Source/");
    ext.listFile();

	Sleep(100000);
    return 0;
}

FeaturesExtractor::FeaturesExtractor(string source) {
	this->source = source;
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

void FeaturesExtractor::readImage(string name) {
	cout << name << endl;

	Mat original = imread(name);
}