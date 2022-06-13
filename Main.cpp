#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include "opencv2/core/utils/logger.hpp"
#include <stdint.h>
#include <bits/stdc++.h>
#include <Windows.h>
#include <fstream>
using namespace cv;
using namespace std;

void ClearScreen(char fill = ' ')
{
	COORD tl = { 0, 0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(console, fill, cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

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
	int numframes=int(cap.get(CAP_PROP_FRAME_COUNT));
	stringstream* frames= new stringstream[numframes];
	vector<vector<vector<char>>> o;
	int fc=0;
	
    while (cap.read(img))
    {
        cvtColor(img, imgBW, COLOR_BGR2GRAY);
        imgBW.convertTo(imgC, -1, 1, 0); //contrast
		vector<vector<char>> f;
		vector<char> row;
		std::cout << "Processing " << numframes << " frames\n";
		std::cout << "Frame Number: " << fc << " out of " << numframes << endl;
		system("CLS");
		for (int r = 0; r < imgC.rows; r += img.rows / 50) {
			for (int c = 0; c < imgC.cols; c += img.cols / 100) {
				if (imgC.at<uint8_t>(r, c) == 0) {
					frames[fc] << " ";
					continue;
				}
				int x = imgC.at<uint8_t>(r, c);
				int ci = 0;
				for (int a = 34; a <= 126; a++) {
					if (abs(255-grey_value[ci] - x) > abs(255-grey_value[a - 33] - x))
						ci = a - 33;
				}
				frames[fc]<<(char)(ci + 33);
			}
			std::cout << endl;
		}
		fc++;
        //imshow("Image", imgC);
		//waitKey(1);
		
    }
	
	for (int i = 0; i < numframes; i++)
	{
		ClearScreen();
		
		printf("\r");
		printf("%s", frames[i].str().c_str());
		Sleep(100);
	}

	
	
}