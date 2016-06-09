#include <opencv2/opencv.hpp>
#include <windows.h>
#include <iostream>
using namespace std;
using namespace cv;

void edge_detection(Mat frame)
{
    Mat edges;
    cvtColor(frame, edges, COLOR_BGR2GRAY);
    GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
    Canny(edges, edges, 0, 30, 3);
    cvtColor(edges,frame, COLOR_GRAY2BGR);
}
int main(int argc, char** argv)
{
    VideoCapture cam(0); // 0 FOR default camera
    if (!cam.isOpened())
    {
        MessageBeep(NULL);
        MessageBoxA(NULL,"Failed to open camera","Error",NULL);
        return -1;
    }
    cam.set(CAP_PROP_FRAME_WIDTH,640);
    cam.set(CAP_PROP_FRAME_HEIGHT,480);
    cam.set(CAP_PROP_FPS,60);

    VideoWriter vid("cam.avi",CV_FOURCC('P','I','M','1'),60,cv::Size(640,480),true);
    int key,mode;
    Mat frame;

    cvNamedWindow("record",CV_WINDOW_NORMAL);
    do
    {
        cam>>frame;
        if      (mode==1)
            cvtColor(frame,frame,COLOR_BGR2HSV);
        else if (mode==2)
            cvtColor(frame,frame,COLOR_BGR2YUV);
        else if (mode==3)
            edge_detection(frame);
        vid<<frame;
        imshow("record",frame);

        key=waitKey(10);
        switch (key)
        {
            case 48: // 0
                mode=0;
                break;
            case 49: // 1
                mode=1;
                break;
            case 50: // 2
                mode=2;
                break;
            case 51: // 3
                mode=3;
                break;
            case 52: // 4
                mode=4;
                break;
        }

    }while(key!=27);

    return 0;
}
