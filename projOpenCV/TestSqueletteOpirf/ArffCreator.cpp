#include "ArffCreator.h"

ArffCreator::ArffCreator(string filename)
{
	openArffFile(filename + ".arff");
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

int main(){
	ArffCreator ac("test");
    
	list<pair<string, string>> l;
	l.push_back(list<pair<string, string>>::value_type("sepallength", "NUMERIC"));
	l.push_back(list<pair<string, string>>::value_type("sepalwidth", "NUMERIC"));
	l.push_back(list<pair<string, string>>::value_type("petallength", "NUMERIC"));
	l.push_back(list<pair<string, string>>::value_type("petalwidth", "NUMERIC"));
	l.push_back(list<pair<string, string>>::value_type("class", "{Iris-setosa,Iris-versicolor,Iris-virginica}"));

	ac.writeHeader("iris", l);

	list<list<string>> ll;
	list<string> l1, l2, l3;

	l1.push_back("5.1");
	l1.push_back("3.5");
	l1.push_back("1.4");
	l1.push_back("0.2");
	l1.push_back("Iris-setosa");

	l2.push_back("4.9");
	l2.push_back("3.0");
	l2.push_back("1.4");
	l2.push_back("0.2");
	l2.push_back("Iris-setosa");

	l3.push_back("4.7");
	l3.push_back("3.2");
	l3.push_back("1.3");
	l3.push_back("0.2");
	l3.push_back("Iris-setosa");

	ll.push_back(l1);
	ll.push_back(l2);
	ll.push_back(l3);

	ac.writeData(ll);
}