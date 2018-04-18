

// Include libraries
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>


using namespace cv;
using namespace std;

int main(int argc, char* argv)
{
	Mat image_original = imread("zebras.jpg");
	Mat image;
	Mat image_padded;
	Mat image_dct;
	cvtColor(image_original, image, CV_RGB2GRAY);

	if (image.empty())return -1;

	// Add padding to the image for faster dft
	int x = getOptimalDFTSize(image.rows);
	int y = getOptimalDFTSize(image.cols);
	copyMakeBorder(image, image_padded, 0, x-image.rows, 0, y-image.cols, BORDER_CONSTANT, Scalar::all(0));


	// Split it Mat in complex and imaginary planes 
	Mat planes[] = {Mat_<float>(image_padded), Mat::zeros(image_padded.size(), CV_32F)};
	Mat complexPlane;
	merge(planes, 2, complexPlane);

	// Perform dft and save the values in the same matrix
	dft(complexPlane, complexPlane);

	// Split the complex dft and calculate the magnitude
	split(complexPlane, planes);
	magnitude(planes[0], planes[1], planes[0]);
	Mat imageMagnitude = planes[0];

	//Convert to logarithmic scales
	imageMagnitude+= Scalar::all(1);
	log(imageMagnitude, imageMagnitude);

	// Crop the spectrum
	imageMagnitude = imageMagnitude(Rect(0, 0, imageMagnitude.cols & -2, imageMagnitude.rows & -2));


	//Normalize the spectrum
	normalize(imageMagnitude, imageMagnitude, 0, 1, NORM_MINMAX); 
	
	// Display of the original Image
	namedWindow("Original Image", WINDOW_AUTOSIZE);
	imshow("Original Image", image_original);
 
	

	// Truncate the image dft by l1 and l2
	int k1, k2;
	int l1 = 65;
	int l2 = 65;
	for (k1 = l1; k1 < 255 - l1 + 1; k1++) {
		for (k2 = l2; k2 < 255 - l2 + 1; k2++)
		{
			complexPlane.at<signed>(k1, k2) = 0;

		}
	}



	// Reconstructing original imae from the DFT coefficients
	Mat invDFT, invDFTcvt;
	idft(complexPlane, invDFT, DFT_SCALE | DFT_REAL_OUTPUT); // Applying IDFT
	invDFT.convertTo(invDFTcvt, CV_8U);


	namedWindow("InverseDFT", WINDOW_AUTOSIZE);
	imshow("InverseDFT", invDFTcvt);
	

	// DCT
	Mat convertedImage, invDCTcvt, invDCT, image_padded_dct;

	// Convert the image to 32f and calculate dct of the image
	image.convertTo(convertedImage, CV_32F);
	dct(convertedImage, image_dct, 0);
	


	// Setting values of the pixels over l to zero
	int l =  85;

	for (k1 = l; k1 < 255; k1++) {
		for (k2 = l; k2 < 255 ; k2++)
		{
			image_dct.at<signed>(k1, k2) = 0;

		}
	}

	// Calculating the IDCT and normalizing the output
	idct(image_dct, invDCT,0);
	normalize(invDCT, invDCT, 0, 1, NORM_MINMAX);	

	// Display of IDCT with truncated output
	namedWindow("Inverse DCT", WINDOW_AUTOSIZE);
	imshow("Inverse DCT", invDCT);

	waitKey(0);
	destroyAllWindows();

}