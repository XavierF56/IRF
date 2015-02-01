#include "ImagesExtractor.h"
#include "ImageAnalyser.h"
#include <fstream>
#include <windows.h>




ImagesExtractor::ImagesExtractor(int i)
{
	string path = "\\\\bricolo\\p12\\5info\\irfBD\\NicIcon\\";
	LPCWSTR Lfolder = L"Result\\";
	string folder = "Result\\";
	ofstream file;
	CreateDirectory(Lfolder, NULL);
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
			ImageAnalyser imgAn(path + "w" + scripter + "-scans/" + form + ".png");
			if(imgAn.isCorrect())
			{
				for( int row = 1; row <= 7; row ++)
				{
					string label(imgAn.getLabel(row));
					for( int column = 1 ; column <= 5; column++)
					{
						string filename = label + "_" + scripter + "_" + page + "_" + std::to_string((long double)row) + "_" + std::to_string((long double)column);
						cout << filename << endl;
						Mat img(imgAn.extract(row, column));
						imwrite(folder + filename + ".png", img);
						file.open(folder + filename + ".txt");
						file << "label " + label + "\nform " + form + "\nscripter " + scripter + "\npage " + page + "\nrow " + std::to_string((long double)row) + "\ncolumn " + std::to_string((long double)column) + "\nsize";
						file.close();
					}
				}
			}
		}
	}
}


/*
ImagesExtractor::ImagesExtractor(int i)
{
	
	string path = "\\\\bricolo\\p12\\5info\\irfBD\\NicIcon\\";
	LPCWSTR  Lfolder = L"Result\\";
	string  folder = "Result\\";
	ofstream file;
	CreateDirectory(Lfolder, NULL);
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
			ImageAnalyser imgAn(path + "w" + scripter + "-scans/" + form + ".png");
			if(imgAn.isCorrect())
			{
				for( int row = 1; row <= 7; row ++)
				{
					string label(imgAn.getLabel(row));
					for( int column = 1 ; column <= 5; column++)
					{
						string filename = label + "_" + scripter + "_" + page + "_" + std::to_string((long double)row) + "_" + std::to_string((long double)column);
						cout << filename << endl;
						Mat img(imgAn.extract(row, column));
						imwrite(folder + filename + ".png", img);
					
						file.open(folder + filename + ".txt");
						file << "label " + label + "\nform " + form + "\nscripter " + scripter + "\npage " + page + "\nrow " + std::to_string((long double)row) + "\ncolumn " + std::to_string((long double)column) + "\nsize";
						file.close();
					}
				}
			}

		}
	}
}
*/

ImagesExtractor::~ImagesExtractor()
{
}

/*
int main(int argc, char* argv[])
{
	ImagesExtractor imgE(0);

	waitKey(0);
	cout << "I am done" << endl;
	Sleep(100000);
}

*/