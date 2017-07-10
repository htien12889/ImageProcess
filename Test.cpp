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
#include <string>   // for strings

using namespace std;
using namespace cv;
int photoToCartoon();
int photoToCartoonValue();
int photoToWaterColor();
int waterMark(); 
int pencil();
int createVideo();
int pencilColor();
int pencil1();
int pencil1Color();

/** @function main */
int main()
{
	clock_t start = clock();
	pencil1Color();
	clock_t stop = clock();
	printf("Processing times: %d [ms]\n", stop - start);
	return 0;
}

int createVideo()
{
	Mat images; //fill this somehow
	images = imread("video/1.jpg");
	if (!images.data)
	{
		return -1;
	}
	Size S = images.size();
	VideoWriter outputVideo;  // Open the output
	outputVideo.open("testing.mov", CV_FOURCC('m', 'p', '4', 'v'), 100, S, true);

	if (!outputVideo.isOpened()){
		cout << "Could not open the output video for write: " << endl;
		return -1;
	}
	Mat src1, src2, outImage;
	outImage = Mat(images.size(), CV_8UC3);
	for (int i = 1; i < 5; i++)
	{
		stringstream ss;
		ss << i;
		string str1 = ss.str();
		stringstream sa;
		int nextImage = i + 1;
		sa << nextImage;
		string str2 = sa.str();

		src1 = imread("video/" + str1 + ".jpg");
		src2 = imread("video/" + str2 + ".jpg");
		if (!src1.data || !src2.data)
		{
			return -1;
		}
		int step = 0;
		if (i == 1)
		{
			while (true)
			{
				int desCoor = src1.cols - (step * 10);
				int x, y, z;
				int stepIndex = step * 10;
				for (x = 0; x < src1.rows; x++)
				{
					for (y = 0; y < desCoor; y++)
					{
						//grad.at<uchar>(i, j) = 255 - grad.at<uchar>(i, j);
						outImage.at<Vec3b>(x, y) = src1.at<Vec3b>(x, y + stepIndex);
					}
					for (z = 0; y < src1.cols; y++, z++)
					{
						//grad.at<uchar>(i, j) = 255 - grad.at<uchar>(i, j);
						outImage.at<Vec3b>(x, y) = src2.at<Vec3b>(x, z);
					}
				}

				outputVideo << outImage;
				step++;
				if (desCoor <= 0){
					break;
				}
			}
		}
		
		else
		{
			while (true)
			{
				int desCoor = src1.cols - (step * 10);
				int x, y, z;
				int stepIndex = step * 10;
				for (x = 0; x < src1.rows; x++)
				{
					for (y = 0; y < desCoor; y++)
					{
						//grad.at<uchar>(i, j) = 255 - grad.at<uchar>(i, j);
						outImage.at<Vec3b>(x, y) = src1.at<Vec3b>(x, y + stepIndex);
					}
					for (z = 0; y < src1.cols; y++, z++)
					{
						//grad.at<uchar>(i, j) = 255 - grad.at<uchar>(i, j);
						outImage.at<Vec3b>(x, y) = src2.at<Vec3b>(x, z);
					}
				}

				outputVideo << outImage;
				step++;
				if (desCoor <= 0){
					break;
				}
			}
		}

		for (int stop = 0; stop < 200; stop++){
			outputVideo << outImage;
		}
	}

	/*for (int i = 0; i<images.size(); i++){
		outputVideo << res;
	}*/

	cout << "Finished writing" << endl;
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

int pencilColor()
{
	Mat src, srcGray, srcDopg, dst, srcHLS;
	src = imread("Tulips.jpg");
	cvtColor(src, srcGray, CV_BGR2GRAY);
	GaussianBlur(srcGray, srcDopg, Size(5, 5), 0, 0, BORDER_DEFAULT);
	dst = Mat(src.size(), CV_8UC3);
	cvtColor(src, srcHLS, CV_BGR2HLS);
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

			}
			for (int k = 0; k < 3; k++)
			{
				tmp = (tmp * tmp) / 255;
			}
			if (tmp > 255)
			{
				tmp = 255;
			}
			srcDopg.at<uchar>(i, j) = tmp;
			dst.at<Vec3b>(i, j)[0] = srcHLS.at<Vec3b>(i, j)[0];
			dst.at<Vec3b>(i, j)[1] = tmp;
			dst.at<Vec3b>(i, j)[2] = srcHLS.at<Vec3b>(i, j)[2]; 
		}
	}
	cvtColor(dst, dst, CV_HLS2BGR);
	//cv::imwrite("pencil.bmp", srcDopg);
	cv::imwrite("pencilColor.bmp", dst);
	return 0;
}

int pencil1Color()
{
	Mat src, srcGray, srcDopg, dst, srcHLS;
	src = imread("Tulips.jpg");
	cvtColor(src, srcGray, CV_BGR2GRAY);
	GaussianBlur(srcGray, srcDopg, Size(5, 5), 0, 0, BORDER_DEFAULT);
	//srcDopg = Mat(srcGray.size(), CV_8UC1);
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat grad;

	Sobel(srcGray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel(srcGray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	cvtColor(src, srcHLS, CV_BGR2HLS);
	dst = Mat(src.size(), CV_8UC3);
	for (int i = 0; i < srcGray.rows; i++)
	{
		for (int j = 0; j < srcGray.cols; j++)
		{
			srcDopg.at<uchar>(i, j) = 255 - srcDopg.at<uchar>(i, j);
			grad.at<uchar>(i, j) = 255 - grad.at<uchar>(i, j);
			int tmp = (int)((1 - 0.5) * srcGray.at<uchar>(i, j)) + (int)(0.5 * srcDopg.at<uchar>(i, j));
			if (tmp > 255)
			{
				tmp = 255;
			}

			if (tmp != 255)
			{
				tmp = (tmp << 8) / (255 - tmp);

			}
			for (int k = 0; k < 3; k++)
			{
				tmp = (tmp * tmp) / 255;
			}
			if (tmp > 255)
			{
				tmp = 255;
			}
			srcDopg.at<uchar>(i, j) = tmp;
			tmp = (int)((1 - 0.5) * grad.at<uchar>(i, j)) + (int)(0.5 * srcDopg.at<uchar>(i, j));
			if (tmp > 255)
			{
				tmp = 255;
			}
			dst.at<Vec3b>(i, j)[0] = srcHLS.at<Vec3b>(i, j)[0];
			dst.at<Vec3b>(i, j)[1] = tmp;
			dst.at<Vec3b>(i, j)[2] = srcHLS.at<Vec3b>(i, j)[2];
		}
	}

	//addWeighted(grad, 0.5, srcDopg, 0.5, 0, dst);
	cvtColor(dst, dst, CV_HLS2BGR);
	cv::imwrite("pencil1Color.bmp", dst);
	return 0;
}

int pencil1()
{
	Mat src, srcGray, srcDopg, dst;
	src = imread("Tulips.jpg");
	cvtColor(src, srcGray, CV_BGR2GRAY);
	GaussianBlur(srcGray, srcDopg, Size(5, 5), 0, 0, BORDER_DEFAULT);
	//srcDopg = Mat(srcGray.size(), CV_8UC1);
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;
	Mat grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y;
	Mat grad;

	Sobel(srcGray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel(srcGray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
	for (int i = 0; i < srcGray.rows; i++)
	{
		for (int j = 0; j < srcGray.cols; j++)
		{
			srcDopg.at<uchar>(i, j) = 255 - srcDopg.at<uchar>(i, j);
			grad.at<uchar>(i, j) = 255 - grad.at<uchar>(i, j);
			int tmp = (int)((1 - 0.5) * srcGray.at<uchar>(i, j)) + (int)(0.5 * srcDopg.at<uchar>(i, j));
			if (tmp > 255)
			{
				tmp = 255;
			}

			if (tmp != 255)
			{
				tmp = (tmp << 8) / (255 - tmp);

			}
			for (int k = 0; k < 3; k++)
			{
				tmp = (tmp * tmp) / 255;
			}
			if (tmp > 255)
			{
				tmp = 255;
			}
			srcDopg.at<uchar>(i, j) = tmp;
		}
	}
	
	addWeighted(grad, 0.5, srcDopg, 0.5, 0, dst);

	cv::imwrite("pencil1.bmp", dst);
	return 0;
}

int pencil()
{
	Mat src, srcGray, srcDopg, dst;
	src = imread("Tulips.jpg");
	cvtColor(src, srcGray, CV_BGR2GRAY);
	GaussianBlur(srcGray, srcDopg, Size(5, 5), 0, 0, BORDER_DEFAULT);
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
				
			}
			for (int k = 0; k < 3; k++)
			{
				tmp = (tmp * tmp) / 255;
			}
			if (tmp > 255)
			{
				tmp = 255;
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
	src = imread("1498468701-660kane1-copy.jpg");

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
	cv::imwrite("hightPass1.jpg", highPassFilter);
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
	cv::imwrite("edge.jpg", grad);
	cv::imwrite("result2.jpg", tong);

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
	cv::imwrite("result4.jpg", tongRGB);

	return 0;
}
