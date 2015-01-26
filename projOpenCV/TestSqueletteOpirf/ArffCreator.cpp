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
	DIR *pDIR;
	string ext = ".png";

	// Cration du ArffCreator
	list<list<string>> data;
	list<pair<string, string>> features;	
	
	// Ecriture du Header du fichier Arff
	features.push_back(list<pair<string, string>>::value_type("BBBarycenterX", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("BBBarycenteY", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("BBRatio", "NUMERIC"));
	features.push_back(list<pair<string, string>>::value_type("PixelsRatio", "NUMERIC"));	
	features.push_back(list<pair<string, string>>::value_type("class", "{accident,bomb,car,casualty,electricity,fire,fire_brigade,flood,gas,injury,paramedics,person,police,roadblock}"));

	writeHeader("Features", features);


	// Recolte des données
	struct dirent *entry;
	if (pDIR=opendir(this->folder.c_str())) {
		while(entry = readdir(pDIR)){
			if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
				string imageName = entry->d_name;

				// contains ext .png
				if (imageName.find(ext) != string::npos) {
					FeaturesExtractor tmp(this->folder + imageName, 4);
					list<string> datum;
					datum.push_back(std::to_string((long double)(tmp.getNormalizedCoGX(0))));
					datum.push_back(std::to_string((long double)(tmp.getNormalizedCoGY(0))));	
					datum.push_back(std::to_string((long double)(tmp.getRatioBB(0))));
					datum.push_back(std::to_string((long double)(tmp.getRatioColor(0))));
					datum.push_back(tmp.getClass());

					data.push_back(datum);
				} 
			}
		}
		closedir(pDIR);
	}
	
	// Ecriture des donnees dans le fichier Arff
	writeData(data);
}

int main(){
	ArffCreator ac("test2", "samples/");
	ac.extract();
	waitKey(0);
	Sleep(100000);
}