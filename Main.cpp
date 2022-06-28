#include <opencv2/opencv.hpp>
#include <iostream>
#include "opencv2/core/utils/logger.hpp"
#include <stdint.h>
#include <bits/stdc++.h>
#include <Windows.h>
#include <fstream>
#include <WinUser.h>
#include <stdio.h>
using namespace cv;
using namespace std;



void ClearScreen()
{
	// Get the Win32 handle representing standard output.
	// This generally only has to be done once, so we make it static.
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	COORD topLeft = { 0, 0 };

	// std::cout uses a buffer to batch writes to the underlying console.
	// We need to flush that to the console because we're circumventing
	// std::cout entirely; after we clear the console, we don't want
	// stale buffered text to randomly be written out.
	std::cout.flush();

	// Figure out the current width and height of the console window
	if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
		// TODO: Handle failure!
		abort();
	}
	DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	DWORD written;

	// Flood-fill the console with spaces to clear it
	FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);

	// Reset the attributes of every character to the default.
	// This clears all background colour formatting, if any.
	FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);

	// Move the cursor back to the top left for the next sequence of writes
	SetConsoleCursorPosition(hOut, topLeft);
}

void MaximizeWindow()
{
	HWND hWnd;
	SetConsoleTitle(L"ASCII Video Animation");
	hWnd = FindWindow(NULL, L"ASCII Video Animation");
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD NewSBSize = GetLargestConsoleWindowSize(hOut);
	SMALL_RECT DisplayArea = { 0, 0, 0, 0 };

	SetConsoleScreenBufferSize(hOut, NewSBSize);

	DisplayArea.Right = NewSBSize.X - 1;
	DisplayArea.Bottom = NewSBSize.Y - 1;

	SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);

	ShowWindow(hWnd, SW_MAXIMIZE);
}

void ChangeConsoleSize(int size)
{
	CONSOLE_FONT_INFOEX cfi{};
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = size;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

//Read the given dark values for the ascii characters
void LoadGreyValues(vector<int>& GreyValues) {
	ifstream ifile;
	ifile.open("ASCII VALUES/Values.txt");
	for (int i = 0; i < (126 - 33 + 1); i++) {
		int x;
		ifile >> x >> x;
		GreyValues[i] = x;
	}
	ifile.close();
}


//Takes in an image and converts it into a string
string ConvertToText(Mat& frame,vector<int>& GreyValues) {

	string Converted = "";
	for (int r = 0; r < frame.rows; r++)
	{
		for (int c = 0; c < frame.cols; c++)
		{
			int color = frame.at<uint8_t>(r, c);
			if (color == 0) {
				Converted += " ";
			continue;
			}
			int ci = 0;
			for (int a = 34; a <= 126; a++) {
				if (abs(GreyValues[ci] - color) > abs(GreyValues[a - 33] - color))
					ci = a - 33;
			}
			Converted += (char)(ci + 33);
		}
		Converted += '\n';
	}
	Converted.resize(Converted.size() - 1);
	return Converted;
}


//Get text from ConvertToText then write it to a txt file
void ProcessFrame(Mat& frame,int frameNumber,vector<int>& GreyValues) {
	ChangeConsoleSize(16);
	MaximizeWindow();
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int w_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int w_height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	double height_ratio, width_ratio;

	height_ratio = (double)w_height / frame.size().height;

	width_ratio = (double)w_width / frame.size().width;
	Mat frameResized, frameBW, frameC;
	resize(frame, frameResized, Size(), width_ratio, height_ratio);
	cvtColor(frameResized, frameBW, COLOR_BGR2GRAY);
	frameBW.convertTo(frameC, -1, 1, 0);
	string Converted = ConvertToText(frameC,GreyValues);

	ofstream ofile;
	ofile.open("DisplayFiles/" + to_string(frameNumber) + ".txt");
	ofile << Converted;
	ofile.close();
}

void DisplayVideo(int numberofFrames,int frameRate,int size) {
	ChangeConsoleSize(size);
	MaximizeWindow();

	ClearScreen();
	ifstream ifile;
	int FPS = frameRate;

	auto time_between_frames = chrono::microseconds(chrono::seconds(1)) / FPS;

	auto target_tp = chrono::steady_clock::now();
	stringstream str;
	string out;
	for (int i = 1; i <= numberofFrames; i++)
	{
		ifile.open("DisplayFiles/" + to_string(i) + ".txt");
		str << ifile.rdbuf();
		out = str.str();
		ifile.close();

		printf("\r");
		printf("%s", out.c_str());
		printf("\n");
		
		target_tp += time_between_frames;
		this_thread::sleep_until(target_tp);
	}
}


// Call
// ProcessFrame for each frame in the video
// DisplayVideo at the end of processing
void ProcessVideo(VideoCapture cap, string videoName,int size) {
	ChangeConsoleSize(size);
	ClearScreen();
	MaximizeWindow();
	vector<int> GreyValues(94, 0); //94 different characters

	LoadGreyValues(GreyValues); //gets values from text file

	int frameRate = cap.get(CAP_PROP_FPS);
	int numberofFrames = cap.get(CAP_PROP_FRAME_COUNT);
	Mat frame;
	string progressbar = "";
	string emptybar = "----------";
	cout << "Do you want to process the video from the beginning ? (Y/N): ";
	string ans;
	cin >> ans;
	if (ans == "Y") {
		for (int i = 1; i <= numberofFrames; i++)
		{
			cap.read(frame);
			ProcessFrame(frame, i, GreyValues);
			if (i % (numberofFrames / 10) == 0) {
				progressbar += "#";
				if (!emptybar.empty())
					emptybar.resize(emptybar.size() - 1);
			}
			ClearScreen();
			cout << "Processing frame number " << i << " of " << numberofFrames << " :";
			cout << "[" << progressbar << emptybar << "]\n";
		}
	}


	DisplayVideo(numberofFrames,frameRate,size);
}




/*
	Colors Selectors for Console
	system("Color F0") means White Background, Black Text
	0 = Black        8 = Gray
	1 = Blue        9 = Light Blue
	2 = Green        A = Light Green
	3 = Blue-Gray        B = Cyan
	4 = Redd      C = Light Red
	5 = Purple     D = Light Purple
	6 = Yellow        E = Light Yellow
	7 = White       F = Clear white

	*/


void main() {
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);

	////setup console screen
	////unicode chars
	//wchar_t* screen = new wchar_t[nScreenHeight * nScreenWidth];
	////console screen buffer
	//HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	////set it to be our console
	//SetConsoleActiveScreenBuffer(hConsole);
	//DWORD dwBytesWritten = 0;
	////null character to tell console to escape
	//screen[nScreenHeight * nScreenWidth - 1] = '\0';
	////(screen buffer, characters, dimensions or number, position to print x y , extra variable)
	//WriteConsoleOutputCharacter(hConsole, screen, nScreenHeight * nScreenWidth, { 0,0 }, &dwBytesWritten);
	int size = 16;
	system("Color F0");
	ChangeConsoleSize(size);
	ClearScreen();
	MaximizeWindow();
	



	string videoName;
	cout << "Enter video file name : ";
	cin >> videoName; 


	VideoCapture cap(videoName);

	ProcessVideo(cap, videoName,size);
	

}



//
//
//string path = "Paint.mp4";
//VideoCapture cap(path);
//Mat img, imgBW, imgC;
//ifstream ifile;
//ifile.open("ASCII VALUES/Values.txt");
//vector<int> grey_value;
//for (int i = 0; i < (126 - 33 + 1); i++) {
//	int x;
//	ifile >> x >> x;
//	grey_value.push_back(x);
//}
//ifile.close();
//int numframes = int(cap.get(CAP_PROP_FRAME_COUNT));
//stringstream* frames = new stringstream[numframes];
//vector<vector<vector<char>>> o;
//int fc = 0;
//
//while (cap.read(img))
//{
//	cvtColor(img, imgBW, COLOR_BGR2GRAY);
//	imgBW.convertTo(imgC, -1, 1, 0); //contrast
//	vector<vector<char>> f;
//	vector<char> row;
//	std::cout << "Processing " << numframes << " frames\n";
//	std::cout << "Frame Number: " << fc << " out of " << numframes << endl;
//	system("CLS");
//	for (int r = 0; r < imgC.rows; r += img.rows / 50) {
//		for (int c = 0; c < imgC.cols; c += img.cols / 100) {
//			if (imgC.at<uint8_t>(r, c) == 0) {
//				frames[fc] << " ";
//				continue;
//			}
//			int x = imgC.at<uint8_t>(r, c);
//			int ci = 0;
//			for (int a = 34; a <= 126; a++) {
//				if (abs(255 - grey_value[ci] - x) > abs(255 - grey_value[a - 33] - x))
//					ci = a - 33;
//			}
//			frames[fc] << (char)(ci + 33);
//		}
//		std::cout << endl;
//	}
//	fc++;
//	//imshow("Image", imgC);
//	//waitKey(1);
//
//}