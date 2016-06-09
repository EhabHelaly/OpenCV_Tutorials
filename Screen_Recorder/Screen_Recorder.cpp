/// This program is a Desktop screen recorder
// OpenCV: image reading from buffer, image processing, video writing, GUI
// Other :  device context, bitmap
// libraries to link:
//      libopencv_highgui310.dll.a
//      libopencv_imgproc310.dll.a
//      libopencv_videoio310.dll.a
//      libgdi32.a

#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

#include <windows.h>
#include <iostream>
using namespace std;
using namespace cv;

void ScreenShot(BYTE* (&ScreenData),int w,int h)
{
    // copy screen to bitmap
    HDC     hScreen = GetDC(NULL);
    HDC     hDC     = CreateCompatibleDC(hScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, w, h);
    HGDIOBJ old_obj = SelectObject(hDC, hBitmap);
    BitBlt(hDC, 0, 0, w, h, hScreen, 0, 0, SRCCOPY);
    SelectObject(hDC, old_obj);

    BITMAPINFOHEADER bmi = {0};
    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biPlanes = 1;
    bmi.biBitCount = 32;
    bmi.biWidth = w;
    bmi.biHeight = -h;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = 0 ;

    if(ScreenData)
        free(ScreenData);
    ScreenData = (BYTE*)malloc(4 * w * h);
    GetDIBits(hDC, hBitmap, 0, h, ScreenData, (BITMAPINFO*)&bmi,DIB_RGB_COLORS);

    // clean up
    SelectObject(hDC, old_obj);
    DeleteDC(hDC);
    ReleaseDC(NULL, hScreen);
    DeleteObject(hBitmap);

}
int main(int argc, char** argv)
{
    // get screen dimensions
    int w  = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int h  = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    int t_start,t_end,frames=0;

    cout<<"time delay to start in seconds: ";   cin>>t_start;
    cout<<"Record time in seconds: ";cin>>t_end;

    // initialize output video writer
    VideoWriter cam("screen.avi",CV_FOURCC('P','I','M','1'),60,cv::Size(1366,768));

    Mat screen(768,1366,CV_8UC4);// holds the screen shot (4 channels) as hbitmap decodes image to 4 channels
    Mat frame (768,1366,CV_8UC3);// holds the screen shot (3 channels) as video writing does not accept 4 channels images
    int from_to[6]={0,0,1,1,2,2};// describes channels adjustments between screen and frame

    cvNamedWindow("record",CV_WINDOW_NORMAL);
    cvResizeWindow("record",w/3,h/3);

    // delay until start
    DWORD time_start=GetTickCount()+t_start*1000;
    while(GetTickCount()<time_start)
    {
        // wait
    }

    // start
    DWORD time_tot=time_start+t_end*1000;
    while(GetTickCount()<time_tot)
    {
        screen.data=0;
        ScreenShot(screen.data,w,h);
        if (screen.empty()||cvWaitKey(30)==27)
            break;
        cv::mixChannels(screen,frame,from_to,3);// remove the 4th channel of screen shot
        imshow("record",frame);
        frames++;
        cam<<frame;
    }
    DWORD time=GetTickCount()-time_start;
    printf("\nframes: %d\ntime: %ld\nfps: %.2f\n",frames,time/1000,frames*1000.0/time);
    cout<<"for fps adjustments, simply use Video_Editor";

    cam.release();

    return 0;
}
