#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "dirent.h"
#include "FeaturesExtractor.h"

using namespace std;


/*
 * ArffCreator class
 * Build .arff files with features coming from images of the database
 * Features are computed in FeaturesExtractor
 */
class ArffCreator
{
	private:
		ofstream fichier;
		string folder;

		int dividingFactor;
		bool hu;
		int huMax;

		
		void openArffFile(string filename);
		void writeHeader(string relation, list<pair<string, string>> attributs);
		void writeData(vector<vector<string>> data);
		void closeArffFile();

	public:
		ArffCreator(string filename, string folder, int div, bool hu, int huMax);
		~ArffCreator();
		void extract();
};