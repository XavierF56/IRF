#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include <math.h>
#include <iostream>
#include <iomanip> 

using namespace std;
using namespace cv;

/*
 * ImagesExtractor class
 * Build the database of images
 */
class ImagesExtractor
{
	public:
		ImagesExtractor(int i);
		~ImagesExtractor();
};