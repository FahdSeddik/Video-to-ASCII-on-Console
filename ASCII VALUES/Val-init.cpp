//#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
//#include <opencv2/imgproc.hpp>
//#include <iostream>
//#include "opencv2/core/utils/logger.hpp"
//#include <stdint.h>
//#include <bits/stdc++.h>
//#include <fstream>
//using namespace cv;
//using namespace std;
//
//
// /*<summary>
// To assign each of the ascii characters a specific value of how dark it is
// (the higher the number the darker the ascii character)
// writes the result in a file
// //</summary>*/
//
//void main() {
//    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
//    
//    vector<int> grey_value;
//    for(int c=33;c<=126;c++){
//        string path = "ASCII Chars/";
//        path += to_string(c);
//        path += ".png";
//        Mat img = imread(path);
//        Mat imgBW;
//        cvtColor(img, imgBW, COLOR_BGR2GRAY);
//        int sum = 0,cp=0;
//        for (int r = 0; r < imgBW.rows; r++) {
//            for (int c = 60; c <= 110; c++)
//            {
//                if (imgBW.at<uint8_t>(r, c) == 255)
//                    continue;
//                //sum+=imgBW.at<uint8_t>(r, c);
//                cp++;
//            }
//        }
//        grey_value.push_back(cp);
//        //imshow("Image", imgBW);
//        //waitKey(1);
//    }
//    float max = *max_element(grey_value.begin(), grey_value.end());
//    for (int i = 0; i < (126 - 33 + 1); i++) {
//        grey_value[i] = 255 - grey_value[i] / max * 255;
//    }
//    ofstream ofile;
//    ofile.open("Values.txt");
//    for (int i = 0; i < (int)grey_value.size(); i++)
//    {
//        ofile << i + 33 << " " << grey_value[i] << endl;
//    }
//    ofile.close();
//
//}
