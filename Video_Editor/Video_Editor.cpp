#include <opencv2/opencv.hpp>
#include <windows.h>
#include <iostream>
using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
    VideoCapture vid("CAM.avi");
    if (!vid.isOpened())
    {
        printf("file is not found\n");
        return -1;
    }
    cvNamedWindow("Video",0);
    Mat frame;
    vid>>frame;
    while (1)
    {
        if (frame.empty())
        {
            printf("video ended\n");
            break;
        }
        erode(frame,frame,Mat());
        dilate(frame,frame,Mat());
        imshow("Video",frame);
        char c=waitKey(80);
        if (c==27)
            break;
        else if (c=='a')
            vid>>frame;
    }
    return 0;
}
