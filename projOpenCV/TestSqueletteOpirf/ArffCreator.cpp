#include "ArffCreator.h"

ArffCreator::ArffCreator(string filename, string folder, int div, bool hu, int huMax)
{
	this->dividingFactor = div;
	this->hu = hu;
	this->huMax = huMax;
	openArffFile(filename + ".arff");
	this->folder = folder;
}

ArffCreator::~ArffCreator()
{
}

void ArffCreator::writeHeader(string relation, list<pair<string, string>> attributs)
{
	fichier << "@RELATION " << relation << endl << endl;
	for(auto i = attributs.begin(); i != attributs.end(); i++)
	{
		fichier << "@ATTRIBUTE ";
		fichier << i->first << " ";
		fichier << i->second << endl;
	}
	fichier << endl << endl;
}

void ArffCreator::writeData(vector<vector<string>> data)
{
	fichier << "@DATA";

	for (auto i = data.begin() ; i != data.end() ; i++)
	{		
		fichier << endl;
		vector<string> l = *i;
		for (auto j = l.begin() ; j != l.end() ; j++)
		{
			if (j != l.begin())
				fichier << ",";
			fichier << *j;
		}
	}

	closeArffFile();
}

void ArffCreator::openArffFile(string filename)
{
	fichier = ofstream(filename, ios::out | ios::trunc);
}

void ArffCreator::closeArffFile()
{
	fichier.close();
}

void ArffCreator::extract() {
	DIR *pDIR;
	string ext = ".png";

	// Data container inialization
	vector<vector<string>> data;
	list<pair<string, string>> features;	
	
	// Header creation
	for (int index = 0; index < dividingFactor + 1; index++) {
		stringstream sstm;
		sstm << "BBBarycenterX" << index;
		string	result = sstm.str();
		features.push_back(list<pair<string, string>>::value_type(result, "NUMERIC"));
		stringstream sstm1;
		sstm1 << "BBBarycenterY" << index;
		result = sstm1.str();
		features.push_back(list<pair<string, string>>::value_type(result, "NUMERIC"));
		stringstream sstm3;
		sstm3 << "PixelsRatio" << index;
		result = sstm3.str();
		features.push_back(list<pair<string, string>>::value_type(result, "NUMERIC"));
		if (hu) {
			for (int k = 0; k < huMax; k++) {
				stringstream tmp;
				tmp << "HuMoment" << k << index;
				string tmpSt = tmp.str();
				features.push_back(list<pair<string, string>>::value_type(tmpSt, "NUMERIC"));
			}
		}
	}
	features.push_back(list<pair<string, string>>::value_type("BBRatio", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("class", "{accident,bomb,car,casualty,electricity,fire,fire_brigade,flood,gas,injury,paramedics,person,police,roadblock}"));
	writeHeader("Features", features);


	// Images pocessing
	struct dirent *entry;
	if (pDIR=opendir(this->folder.c_str())) {
		while(entry = readdir(pDIR)){
			if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				string imageName = entry->d_name;

				// contains ext .png
				if (imageName.find(ext) != string::npos) {
					FeaturesExtractor tmp(this->folder + imageName, dividingFactor);
                    if(tmp.getRatioColor(0) < 0.99) { // check if image is not white
						vector<string> datum;
						for (int index = 0; index < dividingFactor + 1; index++) {
							datum.push_back(std::to_string((long double)(tmp.getNormalizedCoGX(index))));
							datum.push_back(std::to_string((long double)(tmp.getNormalizedCoGY(index))));
							datum.push_back(std::to_string((long double)(tmp.getRatioColor(index))));
							if (hu) {
								for (int k = 0; k < huMax; k++) {
									datum.push_back(std::to_string((long double)(tmp.getHuMoments(index))[k]));
								}
							}
						}
						datum.push_back(std::to_string((long double)(tmp.getRatioBB(0))));
						datum.push_back(tmp.getClass());
						//cout << imageName << endl;
						data.push_back(datum);
					} else {
						cout << "Arghh " << imageName << endl;
					}
				} 
			}
		}
		closedir(pDIR);
	}

	writeData(data);
}


int main(){
	ArffCreator ac("train4", "Result/", 4, false, 3);
	ac.extract();
	cout << "I am done" << endl;
	waitKey(0);
	//Sleep(100000);
}

