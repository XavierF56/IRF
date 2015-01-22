#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "dirent.h"
#include "FeaturesExtractor.h"

using namespace std;

class ArffCreator
{
	private:
		ofstream fichier;
		string folder;

	public:
		ArffCreator(string filename, string folder);
		~ArffCreator();
		void extract();

	private:
		void openArffFile(string filename);
		void writeHeader(string relation, list<pair<string, string>> attributs);
		void writeData(list<list<string>> data);
		void closeArffFile();
};