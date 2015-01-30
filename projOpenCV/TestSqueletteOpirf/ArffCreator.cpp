#include "ArffCreator.h"

ArffCreator::ArffCreator(string filename, string folder)
{
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

void ArffCreator::writeData(list<list<string>> data)
{
	fichier << "@DATA";
	for (auto i = data.begin() ; i != data.end() ; i++)
	{		
		fichier << endl;
		list<string> l = *i;
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
	int dividingFactor = 25;

	bool hu = false;
	int huMax = 8;

	DIR *pDIR;
	string ext = ".png";

	// Cration du ArffCreator
	list<list<string>> data;
	list<pair<string, string>> features;	
	
	// Ecriture du Header du fichier Arff
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
	//features.push_back(list<pair<string, string>>::value_type("IndexX", "NUMERIC"));
	//features.push_back(list<pair<string, string>>::value_type("IndexY", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("BBRatio", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("class", "{accident,bomb,car,casualty,electricity,fire,fire_brigade,flood,gas,injury,paramedics,person,police,roadblock}"));
	writeHeader("Features", features);


	// Recolte des donn�es
	struct dirent *entry;
	if (pDIR=opendir(this->folder.c_str())) {
		while(entry = readdir(pDIR)){
			if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				string imageName = entry->d_name;

				// contains ext .png
				if (imageName.find(ext) != string::npos) {
					FeaturesExtractor tmp(this->folder + imageName, dividingFactor);
                    if(tmp.getRatioColor(0) < 0.99) { // check if image is not white
						list<string> datum;
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
						//datum.push_back(std::to_string((long double)(tmp.getMaxProjectionX(0))));
						//datum.push_back(std::to_string((long double)(tmp.getMaxProjectionY(0))));
						datum.push_back(std::to_string((long double)(tmp.getRatioBB(0))));
						datum.push_back(tmp.getClass());
					
						data.push_back(datum);
					} else {
						cout << "Arghh " << imageName << endl;
					}
					//
				} 
			}
		}
		closedir(pDIR);
	}
	
	// Ecriture des donnees dans le fichier Arff
	writeData(data);
}


int main(){

	ArffCreator ac("test25basic", "Result_irf_test/");
	//ArffCreator ac("final", "samples/");
	ac.extract();
	waitKey(0);
	cout << "I am done" << endl;
	Sleep(100000);
}

