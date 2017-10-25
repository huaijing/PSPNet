#include <vector>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

using namespace std;
using namespace cv;

int loadFilenames(const string &inputDir, vector<string> &imageFilenames) {
    struct dirent *ptr = NULL;
    DIR *dir = NULL;
    dir = opendir(inputDir.c_str());
    if (dir == NULL) {
        printf("[loadFilenames] Failed to open directory: %s\n", inputDir.c_str());
        return -1;
    }
    
    int res = 0;
    
    while ((ptr = readdir(dir)) != NULL) {
        int namlen = strlen(ptr->d_name);
        if (ptr->d_name[namlen - 3] == 'p' && ptr->d_name[namlen - 2] == 'n' && ptr->d_name[namlen-1] == 'g') {
            imageFilenames.push_back(ptr->d_name);
            res = 1;
        } else if (ptr->d_name[namlen - 3] == 'j' && ptr->d_name[namlen - 2] == 'p' && ptr->d_name[namlen-1] == 'g') {
            imageFilenames.push_back(ptr->d_name);
            res = 2;
        }
    }
    closedir(dir);
    return res;
}

void cropImage(const string &inputPath, const string &outputDir, const string &outputFilename,
                int width, int height, int rWidth, int rHeight) {
    cv::Mat image = cv::imread(inputPath);
    if(image.empty()) {
        printf("imread error %s\n", inputPath.c_str());
        return;
    }

    float ratio = (float)width / (float)rWidth;
    int tHeight = ratio * height;
    printf("tHeight %d. width %d\n", tHeight, width);
    cv::Mat roi = image(cv::Rect(0, 0, width, tHeight));

    cv::Mat resultImage;
    cv::resize(roi, resultImage, cv::Size(rWidth, rHeight));


 //   imshow("result image", resultImage);
 //   cvWaitKey(0);

    string outputPath = outputDir + outputFilename;
    printf("cropImage %s\n", outputPath.c_str());
    imwrite(outputPath, resultImage);
}               

void fillImage(const string &inputPath, const string &outputDir, const string &outputFilename,
                int width, int height, int rWidth, int rHeight) {
                    
    cv::Mat image = cv::imread(inputPath);
    if(image.empty()) {
        printf("imread error %s\n", inputPath.c_str());
        return;
    }

    float ratio = (float)rWidth / (float)width;
    int tHeight = ratio * rHeight;
    cv::Mat resizeImage;
    printf("resize %d, %d\n", rWidth, rHeight);
    cv::resize(image, resizeImage, cv::Size(rWidth, tHeight));

    int meanB = 104;
    int meanG = 127;
    int meanR = 124;

    cv::Mat resultImage(rHeight, rWidth, CV_8UC(3));
    if (resultImage.empty()) {
        return;
    }
    for(int i = 0; i < rHeight; i++) {
        for(int j = 0; j < rWidth; j++) {
            resultImage.at<uchar>(i, j * 3) = meanB;
            resultImage.at<uchar>(i, j * 3 + 1) = meanG;
            resultImage.at<uchar>(i, j * 3 + 2) = meanR;
        }
    } 
 

 //   imshow("reize image", resizeImage);
 //   cvWaitKey(0);

    //cv::Rect r(0, 0, rWidth, tHeight);
    //resultImage(r) = resizeImage;
    for(int i = 0; i < tHeight; i++) {
        for(int j = 0; j < rWidth; j++) {
            resultImage.at<uchar>(i, j * 3) = resizeImage.at<uchar>(i, j * 3);
            resultImage.at<uchar>(i, j * 3 + 1) = resizeImage.at<uchar>(i, j * 3 + 1);
            resultImage.at<uchar>(i, j * 3 + 2) = resizeImage.at<uchar>(i, j * 3 + 2);
        }
    } 


//    imshow("result image", resultImage);
//    cvWaitKey(0);

  
    string outputPath = outputDir + outputFilename;
    imwrite(outputPath, resultImage);
}

int main(int argc, const char* argv[]) {
    string inputDir;
    
    int imgWidth = 640;
    int imgHeight = 480;
   
    int resultWidth = 473;
    int resultHeight = 473;


    int isPre = true;
     
    /// parse arguments
    if (argc == 3) {
        inputDir = string(argv[1]) + "/";
        isPre = strtol(argv[2], NULL, 10);
    } else {
        printf("Usage: ./crop.cpp inputDir  isPre\n");
        return -1;
    }
    
    
    /// load image filenames
    vector<string> imageFilenames;
    int type = loadFilenames(inputDir, imageFilenames);

    
    // create the output directory if it does not exist
    string outputDir;
    if(isPre) {
        outputDir = inputDir + "fill/";
    } else {
        outputDir = inputDir + "crop/";
    }
        
    DIR *od = opendir(outputDir.c_str());
    if (od) {
        closedir(od);
    } else {
         mkdir(outputDir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    for (size_t i = 0; i < imageFilenames.size(); i++) {
        string inputPath = inputDir + imageFilenames[i];
        printf("Process image %s\n", inputPath.c_str());

        string outputFilename = imageFilenames[i];
       
        if(isPre) {
            fillImage(inputPath, outputDir, outputFilename, imgWidth, imgHeight, resultWidth, resultHeight);
        } else {
            cropImage(inputPath, outputDir, outputFilename, resultWidth, resultHeight, imgWidth, imgHeight);
        }
    }

    return 0;
}
