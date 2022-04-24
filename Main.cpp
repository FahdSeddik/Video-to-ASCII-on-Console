#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "opencv2/core/utils/logger.hpp"
#include <stdint.h>
#include <bits/stdc++.h>
#include <fstream>
using namespace cv;
using namespace std;



void main() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	string path = "Paint.mp4";
    VideoCapture cap(path);
    Mat img,imgBW,imgC;
	ifstream ifile;
	ifile.open("ASCII VALUES/Values.txt");
	vector<int> grey_value;
	for (int i = 0; i < (126 - 33 + 1); i++) {
		int x;
		ifile >> x >> x;
		grey_value.push_back(x);
	}
	ifile.close();
	/*vector<vector<char>> output;
	cap.read(img);
	int frames = cap.get(CAP_PROP_FRAME_COUNT);
	output.resize(frames);
	for (int i = 0; i < frames; i++)
	{
		output[i].resize(img.rows*img.cols);
	}
	int i = 0;
	int j = 0;*/
	vector<vector<vector<char>>> o;
    while (cap.read(img))
    {
        cvtColor(img, imgBW, COLOR_BGR2GRAY);
        imgBW.convertTo(imgC, -1, 1, 0); //contrast
		vector<vector<char>> f;
		vector<char> row;
		for (int r = 0; r < imgC.rows; r += img.rows / 50) {
			for (int c = 0; c < imgC.cols; c += img.cols / 100) {
				if (imgC.at<uint8_t>(r, c) == 0) {
					row.push_back(' ');
					continue;
				}
				int x = imgC.at<uint8_t>(r, c);
				int ci = 0;
				for (int a = 34; a <= 126; a++) {
					if (abs(255-grey_value[ci] - x) > abs(255-grey_value[a - 33] - x))
						ci = a - 33;
				}
				row.push_back((char)(ci + 33));
			}
			f.push_back(row);
			cout << endl;
		}
		o.push_back(f);
		system("CLS");
        //imshow("Image", imgC);
		//waitKey(1);
    }
	/*i = 0;
	for (int f = 0; f < frames; f++) {
		for (int r = 0; r < imgC.rows; r += img.rows / 50) {
			for (int c = 0; c < imgC.cols; c += img.cols / 100) {
				cout << output[f][i];
			}
			i++;
			cout << endl;
		}
		i = 0;
		system("CLS");
	}*/
	
	for (int i = 0; i < o.size(); i++)
	{
		for (int j = 0; j < o[i].size(); j++) {
			for (int k = 0; k < o[i][j].size(); k++)
			{
				cout << o[i][j][k];
				if (k % (img.cols / 100) == 0)
					cout << endl;
			}
		}
	}
}