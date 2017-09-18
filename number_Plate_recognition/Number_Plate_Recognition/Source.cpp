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
	Mat CarImage;
	Mat CarImage_gray;
	Mat src;    ////???? median blur??
				//Mat CarImage_dia;
	Mat CarImage_dia;// dalate
	Mat CarImage_binary;
	Mat CarImage_ero;// erosion







	int i,Imgnumber;
	string ss = "CAR NUMBER PLATE RECOGNIZATION \n\n\nAhsan Ali 2015-CS-02\n\nMuhammad Ramzan 2015-CS-54\n\nAdnan Akbar Khan 2015-CS-16\n\n\n";
	for (i = 0;i < ss.length();++i)
	{
		cout << ss[i];
		Sleep(50);

	}
	string ImageName;
	cout << "\tWrite name of Image with extension 'exmple: image.png'" << endl;
	cin >> ImageName;


	////////////////////////////////////////////////////////

	CarImage = imread(ImageName);        //inputing original car image
	
	if (CarImage.empty()) {
		std::cout << "Image is not found\n\n";
		abort();
}

	medianBlur(CarImage, src, 7);     //by making little blur to smooth the image

	cvtColor(CarImage, CarImage_gray, CV_BGR2GRAY);  //now converting image from RGB to Gray. So white & black gets separte.Recognization becomes easy...

	threshold(CarImage_gray, CarImage_binary, 150, 200, THRESH_BINARY);        //Setting threshold to set pixels from binary and gray image.... to set threshold 120,255 images at mid distzance 

	medianBlur(CarImage_binary, CarImage_binary, 5);   // again makin little blur to smooth the image 
	

	//Eroding and getting dilate of Image

	int erosion_size = 1;
	int dialate_size = 0;

	Mat ForEroding = getStructuringElement(MORPH_RECT,     // the bright areas of the image's background get thinner, whereas the dark zones i.e characters gets bigger and clear....
		Size(2 * erosion_size + 1, 2 * erosion_size + 1),
		Point(erosion_size, erosion_size));
	
	Mat ForDilate = getStructuringElement(MORPH_RECT,           //Using this function the background dilates around the black regions of the characters in image....
		Size(2 * dialate_size + 1, 2 * dialate_size + 1),
		Point(dialate_size, dialate_size));
	
	erode(CarImage_binary, CarImage_ero, ForEroding);    // eroding the image (binary form) and saving to CarImage_ero
	
	dilate(CarImage_binary, CarImage_dia, ForDilate);   // Dilating the image (binary form) and saving to CarImage_dia
	
	medianBlur(CarImage_binary, CarImage_binary, 1);
	namedWindow("CarImage", CV_WINDOW_NORMAL);
	imshow("CarImage", CarImage);
	//	Mat CarImage_dia;// dalate
	
	imwrite("CarImage_ero.jpg", CarImage_ero);       //saving new image of eroded.



	//////////////////////////////
	//Mat contours

	Mat bs;
	
	bs = imread("CarImage_ero.jpg");
	
	Mat bs_int;
	
	Mat bs_cany;

	threshold(bs, bs_int, 150, 255, THRESH_BINARY_INV); //smoothing the image
	
	Canny(bs_int, bs_cany, 100, 200, 3);               // detecting edges from eroded image
	
	vector<vector<Point>> contours_bs;
	                                                 //creating two vectors for point(x,y) and rectang detection
	vector<Vec4i> hierarchy_bs;  
	                    
	                                                //findcontour function to detect the closed paths
	findContours(bs_cany, contours_bs, hierarchy_bs, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	
	vector<int> selected_bs;
	                                               
	Mat drawing_bs = Mat::zeros(bs_cany.size(), CV_8UC3);
	
	Rect R_bs;
	
	int w_threshold_bs = 10;               //variables of rectangle (number plate)
	
	int h_threshold_bs = 50;
	
	for (int i = 0; i < contours_bs.size(); i++)
	{
		Scalar color_bs = Scalar(0, 255, 0);         
		R_bs = boundingRect(contours_bs[i]);              //getting the boundery of rectangle(number plate)
		                                                  // marking the contours(shapes/characters) inside the rectangle
		if (R_bs.width / (double)R_bs.height > 0.3 && R_bs.width / (double)R_bs.height < 0.6 && R_bs.width > 10 && R_bs.height > 40 && R_bs.width < 100 && R_bs.height < 200)
		{
			selected_bs.push_back(i);
			drawContours(drawing_bs, contours_bs, i, color_bs, 2, 8, hierarchy_bs, 0, Point());
		}
	}

	for (size_t i = 0; i < selected_bs.size(); i++)      //drawing red color boxes around the characters in the number plate
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
