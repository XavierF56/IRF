#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

class ArffCreator
{
	private:
		ofstream fichier;

	public:
		ArffCreator(string filename);
		~ArffCreator();
		void writeHeader(string relation, list<pair<string, string>> attributs);
		void writeData(list<list<string>> data);

	private:
		void openArffFile(string filename);
		void closeArffFile();
};