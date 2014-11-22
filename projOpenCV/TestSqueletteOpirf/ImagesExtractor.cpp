#include "ImagesExtractor.h"
#include "ImageAnalyser.h"

ImagesExtractor::ImagesExtractor(int i)
{
	//string path = "\\\\bricolo\\p12\\5info\\irfBD\\NicIcon\\all-scans\\";
	string folder = "\\Test\\";
	for( int i = 0; i <= 0; i++)
	{
		string scripter;
		stringstream ss1;
		ss1 << std::setfill('0') << std::setw (3) << i;
		ss1 >> scripter;
		for( int j = 0; j <= 1; j++)
		{
			string page, form;
			stringstream ss2;
			ss2 << std::setfill('0') << std::setw (2) << j;
			ss2 >> page;
			form = scripter + page;
			ImageAnalyser imgAn("w" + scripter + "-scans/" + form + ".png");			
			imgAn.analyse();
			for( int row = 1; row <= 7; row ++)
			{
				string label(imgAn.getLabel(row));
				for( int column = 1 ; column <= 5; column++)
				{
					string filename = label + "_" + scripter + "_" + page + "_" + std::to_string((long double)row) + "_" + std::to_string((long double)column);
					cout << filename << endl;
					Mat img(imgAn.extract(row, column));
					imwrite(filename + ".png", img);
				}
			}

		}
	}

}

ImagesExtractor::~ImagesExtractor()
{
}


int main(int argc, char* argv[])
{
	ImagesExtractor imgE(0);
}