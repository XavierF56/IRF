#include <stdio.h>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include"dirent.h"

using namespace std;
using namespace cv;

class ExtractionVerifier
{
	private:
		string source;

	public:
		ExtractionVerifier(string);

		void verifyAll(string);
		bool verifyImage(Mat src);
};