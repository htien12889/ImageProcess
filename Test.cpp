/**
* @file MatchTemplate_Demo.cpp
* @brief Sample code to use the function MatchTemplate
* @author OpenCV team
*/

/************************************************
* FFT code from the book Numerical Recipes in C *
* Visit www.nr.com for the licence.             *
************************************************/

// The following line must be defined before including math.h to correctly define M_PI
#include "stdafx.h"
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/ocl.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace std;
using namespace cv;
int photoToCartoon();
int photoToCartoonValue();
int photoToWaterColor();
int waterMark();
int pencil();


/** @function main */
int main()
{
	clock_t start = clock();
	photoToCartoonValue();
	clock_t stop = clock();
	printf("Processing times: %d [ms]\n", stop - start);
	return 0;
}


int photoToCartoonValue()
{
	Mat src, srcThresh;
	src = imread("tien1.png");
	cvtColor(src, srcThresh, CV_BGR2GRAY);
	threshold(srcThresh, srcThresh, 80, 255, 0);
	
	 
	for (int i = 0; i < srcThresh.rows; i++)
	{
		for (int j = 0; j < srcThresh.cols; j++)
		{
			if (srcThresh.at<uchar>(i, j) != 255)
			{
				src.at<Vec3b>(i, j)[0] = 0;
				src.at<Vec3b>(i, j)[1] = 0;
				src.at<Vec3b>(i, j)[2] = 0;
			}
		}
	}
	cv::imwrite("pencil.bmp", src);
	return 0;
}

int pencil()
{
	Mat src, srcGray, srcDopg, dst;
	src = imread("tien1.png");
	cvtColor(src, srcGray, CV_BGR2GRAY);
	GaussianBlur(srcGray, srcDopg, Size(15, 15), 0, 0, BORDER_DEFAULT);
	//srcDopg = Mat(srcGray.size(), CV_8UC1);
	for (int i = 0; i < srcGray.rows; i++)
	{
		for (int j = 0; j < srcGray.cols; j++)
		{
			srcDopg.at<uchar>(i, j) = 255 - srcDopg.at<uchar>(i, j);
			int tmp = (int)((1 - 0.5) * srcGray.at<uchar>(i, j)) + (int)(0.5 * srcDopg.at<uchar>(i, j));
			if (tmp > 255)
			{
				tmp = 255;
			}

			if (tmp != 255)
			{
				tmp = (tmp << 8) / (255 - tmp);
				tmp *= 0.970;
				if (tmp > 255)
				{
					tmp = 255;
				}
			}
			srcDopg.at<uchar>(i, j) = tmp;
		}
	}
	
	cv::imwrite("pencil.bmp", srcDopg);
	return 0;
}

int waterMark()
{
	Mat src, waterMark, dst;
	waterMark = imread("LinuxLogo.png", IMREAD_UNCHANGED);
	src = imread("HD2160.bmp");
	/*Mat waterMathTemp(src.size(), CV_8UC3);
	waterMathTemp.setTo(Scalar(0, 0, 0));
	int indexWaterX = 0, indexWaterY = 0;
	int rowStart = (waterMathTemp.rows / 2) - (waterMark.rows / 2), rowEnd = rowStart + waterMark.rows;
	int colStart = (waterMathTemp.cols / 2) - (waterMark.cols / 2), colEnd = colStart + waterMark.cols;
	int alpha = 0.5, beta = 1 - alpha;
	for (int i = rowStart; i < rowEnd; i++)
	{
		for (int j = colStart; j < colEnd; j++)
		{
			for (int c = 0; c < 3; c++)
			{
				waterMathTemp.at<Vec3b>(i, j)[c] = waterMark.at<Vec3b>(indexWaterY, indexWaterX)[c];
			}
			indexWaterX++;
		}
		indexWaterY++;
		indexWaterX = 0;
	}*/
	int indexWaterX = 0, indexWaterY = 0;
	int rowStart = (src.rows / 4) - (waterMark.rows / 2), rowEnd = rowStart + waterMark.rows;
	int colStart = (src.cols / 2) - (waterMark.cols / 2), colEnd = colStart + waterMark.cols;
	for (int i = rowStart; i < rowEnd; i++)
	{
		for (int j = colStart; j < colEnd; j++)
		{
			float alphaValue = (float)(waterMark.at<Vec4b>(indexWaterY, indexWaterX)[3]) / 255;
			for (int c = 0; c < 3; c++)
			{
				int kq = (int)((1 - alphaValue) * src.at<Vec3b>(i, j)[c]) + (int)(alphaValue * waterMark.at<Vec4b>(indexWaterY, indexWaterX)[c]);
				if (kq < 0){
					kq = 0;
				}
				else if (kq > 255){
					kq = 255;
				} 
				src.at<Vec3b>(i, j)[c] = kq;
			}
			indexWaterX++;
		}
		indexWaterY++;
		indexWaterX = 0;
	}
	//addWeighted(waterMathTemp, 0.5, src, 1, 0, dst);
	cv::imwrite("waterMark.bmp", src);
	return 0;
}
int photoToWaterColor()
{
	Mat src;
	Mat grad, dst;

	/// Load an image
	src = imread("HD2160.bmp");

	if (!src.data)
	{
		return -1;
	}
	cvtColor(src, grad, CV_BGR2GRAY);

	for (int i = 0; i < grad.rows; i++)
	{
		for (int j = 0; j < grad.cols; j++)
		{
			grad.at<uchar>(i, j) = 255 - grad.at<uchar>(i, j);
		}
	}

	Mat gray3Chanel(grad.size(), CV_8UC3);
	for (int y = 0; y < gray3Chanel.rows; y++)
	{
		for (int x = 0; x < gray3Chanel.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				gray3Chanel.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(grad.at<uchar>(y, x));
			}
		}
	}
	addWeighted(src, 0.2, gray3Chanel, 0.5, 0, dst);
	cv::imwrite("photoToWaterColor.bmp", dst);
	return 0;
}
int photoToCartoon()
{
	Mat src;
	Mat grad;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	int c;

	/// Load an image
	src = imread("abcd.jpg");

	if (!src.data)
	{
		return -1;
	}


	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	//GaussianBlur(src, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(src, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	/// Total Gradient (approximate)
	int indentRight = 235;
	int indentLeft = 15;
	int xdist = indentRight - indentLeft; // indent as index from 0 to 256, for the above picture: indentRight: 235, indentLeft: 15
	double alpha = 256.0 / xdist;
	int beta = (int)-(indentLeft* alpha);
	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	cvtColor(grad, grad, CV_BGR2GRAY);
	Mat highPassFilter(grad.size(), CV_8UC1);
	for (int y = 0; y < grad.rows; y++)
	{
		for (int x = 0; x < grad.cols; x++)
		{
			highPassFilter.at<uchar>(y, x) = grad.at<uchar>(y, x);
		}
	}
	cv::imwrite("hightPass1.bmp", highPassFilter);
	equalizeHist(grad, grad);

	for (int i = 0; i < grad.rows; i++)
	{
		for (int j = 0; j < grad.cols; j++)
		{
			grad.at<uchar>(i, j) = 255 - grad.at<uchar>(i, j);
		}
	}

	Mat tong;
	addWeighted(highPassFilter, 0.2, grad, 0.5, 0, tong);
	cv::imwrite("edge.bmp", grad);
	cv::imwrite("result2.bmp", tong);

	Mat tongRGB(grad.size(), CV_8UC3);
	for (int y = 0; y < tong.rows; y++)
	{
		for (int x = 0; x < tong.cols; x++)
		{
			for (int c = 0; c < 3; c++)
			{
				tongRGB.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(tong.at<uchar>(y, x));
			}
		}
	}
	addWeighted(src, 0.5, tongRGB, 0.5, 0, tongRGB);
	cv::imwrite("result4.bmp", tongRGB);

	return 0;
}
