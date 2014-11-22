#include "ImagesExtractor.h"
#include "ImageAnalyser.h"

ImagesExtractor::ImagesExtractor(int i)
{
	string path = "\\\\bricolo\\p12\\5info\\irfBD\\NicIcon\\all-scans\\";
	for( int i = 0; i <= 34; i++)
	{
		string scripter;
		stringstream ss1;
		ss1 << std::setfill('0') << std::setw (3) << i;
		ss1 >> scripter;
		for( int j = 0; j <= 22; j++)
		{
			string page, form;
			stringstream ss2;
			ss2 << std::setfill('0') << std::setw (2) << j;
			ss2 >> page;
			form = scripter + page;
			ImageAnalyser imgAn(path + form);
			for( int row = 1; row <= 7; row ++)
			{
				string label(imgAn.getLabel(row));
				for( int column = 1 ; column <= 5; column++)
				{
					string filename = label + "_" + scripter + "_" + page + "_" + std::to_string((long double)row) + "_" + std::to_string((long double)column);
					cout << filename << endl;
				}
			}

		}
	}

}

ImagesExtractor::~ImagesExtractor()
{
}


int main2(int argc, char* argv[])
{
	ImagesExtractor imgE(0);
}