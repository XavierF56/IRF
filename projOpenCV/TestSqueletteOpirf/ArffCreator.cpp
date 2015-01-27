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
	int dividingFactor = 16;

	DIR *pDIR;
	string ext = ".png";

	// Cration du ArffCreator
	list<list<string>> data;
	list<pair<string, string>> features;	
	
	// Ecriture du Header du fichier Arff
	for (int index = 0; index < dividingFactor + 1; index++) {
		stringstream sstm;
		/*sstm << "BBBarycenterX" << index;
		string	result = sstm.str();
		features.push_back(list<pair<string, string>>::value_type(result, "NUMERIC"));
		stringstream sstm1;
		sstm1 << "BBBarycenterY" << index;
		result = sstm1.str();
		features.push_back(list<pair<string, string>>::value_type(result, "NUMERIC"));*/
		stringstream sstm3;
		sstm3 << "PixelsRatio" << index;
		string result = sstm3.str();
		features.push_back(list<pair<string, string>>::value_type(result, "NUMERIC"));
		stringstream sstm4;
		sstm4 << "HuMoment0" << index;
		result = sstm4.str();
		features.push_back(list<pair<string, string>>::value_type(result, "NUMERIC"));
		/*stringstream sstm5;
		sstm5 << "HuMoment1" << index;
		result = sstm5.str();
		features.push_back(list<pair<string, string>>::value_type(result, "NUMERIC"));
		stringstream sstm6;
		sstm6 << "HuMoment2" << index;
		result = sstm6.str();
		features.push_back(list<pair<string, string>>::value_type(result, "NUMERIC"));*/
	}
	features.push_back(list<pair<string, string>>::value_type("BarycenterX", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("BarycenterY", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("BBRatio", "NUMERIC"));
	//features.push_back(list<pair<string, string>>::value_type("IndexX", "NUMERIC"));
	//features.push_back(list<pair<string, string>>::value_type("IndexY", "NUMERIC"));
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
							datum.push_back(std::to_string((long double)(tmp.getRatioColor(index))));
							datum.push_back(std::to_string((long double)(tmp.getHuMoments(index))[0]));
							//datum.push_back(std::to_string((long double)(tmp.getHuMoments(index))[1]));
							//datum.push_back(std::to_string((long double)(tmp.getHuMoments(index))[2]));
						}
						datum.push_back(std::to_string((long double)(tmp.getNormalizedCoGX(0))));
						datum.push_back(std::to_string((long double)(tmp.getNormalizedCoGY(0))));
						datum.push_back(std::to_string((long double)(tmp.getRatioBB(0))));
						//datum.push_back(std::to_string((long double)(tmp.getMaxProjectionX(0))));
						//datum.push_back(std::to_string((long double)(tmp.getMaxProjectionY(0))));
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

	ArffCreator ac("99999", "Result_irf/");
	//ArffCreator ac("final", "samples/");
	ac.extract();
	waitKey(0);
	cout << "I am done" << endl;
	Sleep(100000);
}

