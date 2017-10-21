#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <opencv\ml.h>
#include <opencv\cxcore.h>
#include <opencv2\opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2\imgcodecs.hpp>
#include <stdio.h>
#include <stdlib.h>
#include<Windows.h>
#include<string.h>
#include<iostream>
using namespace cv;
using namespace std;

int main() {
	// matrix of Car Image
	Mat CarImage;
	// matrix of Car Image Gray
	Mat CarImage_gray;
	// matrix of Source image
	Mat src;
	Mat CarImage_dia;
	// Binary Image Matrix
	Mat CarImage_binary;
	// erosion Matrix
	Mat CarImage_ero;
	int i,Imgnumber;
	string ss = "CAR NUMBER PLATE RECOGNIZATION \n\n\n" +
	 "Ahsan Ali 2015-CS-02\n\n" +
	 "Muhammad Ramzan 2015-CS-54\n\n" +
	 "Adnan Akbar Khan 2015-CS-16\n\n\n";
	// facy way of the displaying something in the console
	for (i = 0;i < ss.length();++i)
	{
		cout << ss[i];
		Sleep(50);
	}
	string ImageName;
	cout << "\tWrite name of Image with extension 'exmple: image.png'" << endl;
	cin >> ImageName;
	//inputing original car image
	CarImage = imread(ImageName);        
	if (CarImage.empty()) {
		std::cout << "Image is not found\n\n";
		abort();
	}
	// by making little blur to smooth the image
	medianBlur(CarImage, src, 7);
	// now converting image from RGB to Gray. So white & black gets separte.Recognization becomes easy
	cvtColor(CarImage, CarImage_gray, CV_BGR2GRAY);
	// Setting threshold to set pixels from binary and gray image
	// to set threshold 120,255 images at mid distzance 
	threshold(CarImage_gray, CarImage_binary, 150, 200, THRESH_BINARY);
	// again makin little blur to smooth the image 
	medianBlur(CarImage_binary, CarImage_binary, 5);
	//Eroding and getting dilate of Image
	int erosion_size = 1;
	int dialate_size = 0;
	// the bright areas of the image's background get thinner, 
	// whereas the dark zones i.e characters gets bigger and clear
	Mat ForEroding = getStructuringElement(MORPH_RECT,
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	// Using this function the background dilates around the black regions of the characters in image....
	Mat ForDilate = getStructuringElement(MORPH_RECT,           
		Size(2 * dialate_size + 1, 2 * dialate_size + 1),
		Point(dialate_size, dialate_size));
	// eroding the image (binary form) and saving to CarImage_ero
	erode(CarImage_binary, CarImage_ero, ForEroding);
	// Dilating the image (binary form) and saving to CarImage_dia
	dilate(CarImage_binary, CarImage_dia, ForDilate);   
	medianBlur(CarImage_binary, CarImage_binary, 1);
	namedWindow("CarImage", CV_WINDOW_NORMAL);
	imshow("CarImage", CarImage);
	// saving new image of eroded.
	imwrite("CarImage_ero.jpg", CarImage_ero);       
	// Mat contours
	Mat bs;
	bs = imread("CarImage_ero.jpg");
	Mat bs_int;
	Mat bs_cany;
	// smoothing the image
	threshold(bs, bs_int, 150, 255, THRESH_BINARY_INV);
	// detecting edges from eroded image
	Canny(bs_int, bs_cany, 100, 200, 3);
	vector<vector<Point>> contours_bs;
	// creating two vectors for point(x,y) and rectang detection
	vector<Vec4i> hierarchy_bs;
	// findcontour function to detect the closed paths
	findContours(bs_cany, contours_bs, hierarchy_bs, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	vector<int> selected_bs;
	Mat drawing_bs = Mat::zeros(bs_cany.size(), CV_8UC3);
	Rect R_bs;
	// variables of rectangle (number plate)
	int w_threshold_bs = 10;
	int h_threshold_bs = 50;
	for (int i = 0; i < contours_bs.size(); i++)
	{
		Scalar color_bs = Scalar(0, 255, 0);
		// getting the boundery of rectangle(number plate)         
		R_bs = boundingRect(contours_bs[i]);
		// marking the contours(shapes/characters) inside the rectangle
		if (R_bs.width / (double)R_bs.height > 0.3 && R_bs.width / (double)R_bs.height < 0.6 && R_bs.width > 10 && R_bs.height > 40 && R_bs.width < 100 && R_bs.height < 200)
		{
			selected_bs.push_back(i);
			drawContours(drawing_bs, contours_bs, i, color_bs, 2, 8, hierarchy_bs, 0, Point());
		}
	}
	// drawing red color boxes around the characters in the number plate
	for (size_t i = 0; i < selected_bs.size(); i++)
	{
		rectangle(CarImage, boundingRect(contours_bs[selected_bs[i]]), Scalar(0, 0, 255), 5);
	}
	imwrite("so.jpg", CarImage);
	Mat outputImg = imread("so.jpg");
	namedWindow("Resulted_Img", CV_WINDOW_NORMAL);
	imshow("Resulted_Img", outputImg);
	waitKey(0);
	return(0);
}
