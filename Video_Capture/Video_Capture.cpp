/// This program captures video from the desired camera, with various applicable color effects
// OpenCV : video capturing and writing, image processing, color space conversion, color maps, GUI
// Other  : windows API
// Libraries to link:
//          libopencv_highgui310.dll.a
//          libopencv_imgproc310.dll.a
//          libopencv_videoio310.dll.a

#include <windows.h>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
using namespace cv;

using namespace std;

void edge_detection(Mat frame)
{
    Mat edges;
    cvtColor(frame, edges, COLOR_BGR2GRAY);
    GaussianBlur(edges, edges, cv::Size(7,7), 1.5, 1.5);
    Canny(edges, edges, 0, 30, 3);
    cvtColor(edges,frame, COLOR_GRAY2BGR);
}
void gray_scale(Mat frame)
{
    Mat frame_c1(frame.rows,frame.cols,CV_8UC1);
    cvtColor(frame,frame_c1,COLOR_BGR2GRAY);
    // make a 3 channels image out of the gray scale image
    cvtColor(frame_c1,frame, COLOR_GRAY2BGR);
}
int main(int argc, char** argv)
{
    int cam_id;
    cout<<"Enter desired camera id (0 for default camera):";cin>>cam_id;
    cout<<"\nPress:"
        <<"\n\t0: No effects"
        <<"\n\t1: HSV color space"
        <<"\n\t2: HLV color space"
        <<"\n\t3: YUV color space"
        <<"\n\t4: XYZ color space"
        <<"\n\t5: Edge detection"
        <<"\n\t6: Gray scale"
        <<"\n\t7: Erosion    (iterations 0->9)"
        <<"\n\t8: Dilation   (iterations 0->9)"
        <<"\n\t9: Color maps (color maps 0->9)"
        <<"\n\tShift+(0->9): change number of iterations or color map"
        <<"\n\tESQ: Exit\n";

    VideoCapture cam(cam_id); // 0 FOR default camera
    if (!cam.isOpened())
    {
        MessageBeep(0);
        MessageBoxA(NULL,"Failed to open camera","Error",MB_OK);
        return -1;
    }
    cam.set(CAP_PROP_FRAME_WIDTH,640);
    cam.set(CAP_PROP_FRAME_HEIGHT,480);
    cam.set(CAP_PROP_FPS,60);

    VideoWriter vid("cam.avi",CV_FOURCC('P','I','M','1'),60,cv::Size(640,480),true);
    int key,mode=0,iteration_or_colormap=0;
    Mat frame;

    cvNamedWindow("record",CV_WINDOW_NORMAL);
    do
    {
        cam>>frame;
        if      (mode==1)
            cvtColor(frame,frame,COLOR_BGR2HSV);
        else if (mode==2)
            cvtColor(frame,frame,COLOR_BGR2HLS);
        else if (mode==3)
            cvtColor(frame,frame,COLOR_BGR2YUV);
        else if (mode==4)
            cv::cvtColor(frame,frame,COLOR_BGR2XYZ);
        else if (mode==5)
            edge_detection(frame);
        else if (mode==6)
            gray_scale(frame);
        else if (mode==7)
            cv::dilate(frame,frame,Mat(),cv::Point(-1,-1),iteration_or_colormap);// iteration
        else if (mode==8)
            cv::erode(frame,frame,Mat(),cv::Point(-1,-1),iteration_or_colormap);// iteration
        else if (mode==9)
            cv::applyColorMap(frame,frame,iteration_or_colormap);// color map

        vid<<frame;
        imshow("record",frame);

        key=waitKey(10);
        if (key>=48&&key<58)
            mode=key-48;
        else
        {
             switch (key)
            {
                case 41: // shift+ 0
                    iteration_or_colormap=COLORMAP_AUTUMN;break; // = 0
                case 33: // shift+ 1
                    iteration_or_colormap=COLORMAP_BONE;break;// = 1
                case 64: // shift+ 2
                    iteration_or_colormap=COLORMAP_JET;break;// = 2
                case 35: // shift+ 3
                    iteration_or_colormap=COLORMAP_WINTER;break;// = 3
                case 36: // shift+ 4
                    iteration_or_colormap=COLORMAP_RAINBOW;break;// = 4
                case 37: // shift+ 5
                    iteration_or_colormap=COLORMAP_OCEAN;break;// = 5
                case 94: // shift+ 6
                    iteration_or_colormap=COLORMAP_SUMMER;break;// = 6
                case 38: // shift+ 7
                    iteration_or_colormap=COLORMAP_SPRING;break;// = 7
                case 42: // shift+ 8
                    iteration_or_colormap=COLORMAP_COOL;break;// = 8
                case 40: // shift+ 9
                    iteration_or_colormap=COLORMAP_HSV;break;// = 9
            }
        }

    }while(key!=27);

    return 0;
}
