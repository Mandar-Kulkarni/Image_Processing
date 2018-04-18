


// Include required headers
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char* argv)
{
	Mat image = imread("bird.jpg");
	IplImage *img = cvLoadImage("bird.jpg");
	//IplImage *img(image);

	IplImage *gray;
	IplImage *laplace;
	IplImage *temp;
	IplImage *sobelX, *X, *Y, *cannyImage;

	gray = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	cvCvtColor(img, gray, CV_RGB2GRAY);

	// Allocate image data
	temp = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_16S, 1);
	laplace = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	sobelX = cvCreateImage(cvSize(img->width, img->height), 8, 1);
	cannyImage = cvCreateImage(cvSize(img->width, img->height), 8, 1);

	// Create Temp Images
	X = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_16S, 1);
	Y = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_16S, 1);

	// Sobel and laplace conversions
	cvSobel(gray, X, 1, 1, 5);
	cvLaplace(gray, temp, 3);
	cvCanny(gray, cannyImage, 70, 5);

	// Convert to absolute values
	cvConvertScaleAbs(temp, laplace, 1, 0);
	cvConvertScaleAbs(X, sobelX, 1, 0);

	// Display original Image
	cvNamedWindow("Original Image", WINDOW_AUTOSIZE);
	cvShowImage("Original Image", img);
	
	// Display Laplace Image
	cvNamedWindow("Laplace edge map", WINDOW_AUTOSIZE);
	cvShowImage("Laplace edge map", laplace);


	// Display Image with Sobel Filter
	cvNamedWindow("Sobel edge detection", WINDOW_AUTOSIZE);
	cvShowImage("Sobel edge detection", sobelX);


	// Display Image with Canny edge detector
	cvNamedWindow("Canny edge detection", WINDOW_AUTOSIZE);
	cvShowImage("Canny edge detection", cannyImage);


	cvWaitKey(0);

	cvDestroyAllWindows();
	return 0;
}
