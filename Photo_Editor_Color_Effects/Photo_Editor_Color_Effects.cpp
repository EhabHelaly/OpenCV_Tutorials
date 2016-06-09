/// This program uses HSV colors space to efficiently change image colors
// OpenCV: image reading and writing, image threshing,image processing, GUI, track bars
// Other : open file name dialog, windows API,check file existence
// Libraries to link:
//      libopencv_imgcodecs310.dll.a
//      libopencv_imgproc310.dll.a
//      libopencv_highgui310.dll.a
//      libcomdlg32.a

#include <windows.h>
#include <stdio.h>
#include <cstring>
#include <sys/stat.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
using namespace cv;

Mat image  ;
Mat h_image;
Mat n_image;

int h_min=0;
int s_min=0;
int v_min=0;

int h_max=179;
int s_max=255;
int v_max=255;

int h_new=0;
int s_new=0;
int v_new=0;

int x_min=0;
int y_min=0;
int x_max;
int y_max;

void resize_windows()
{
    cvResizeWindow("Image", 200,200*image.rows/image.cols);
    cvResizeWindow("Binary Image", 200,200*image.rows/image.cols);
    cvResizeWindow("New Image", 500,500*image.rows/image.cols);
    x_max=image.rows;
    y_max=image.cols;
}
void change_h(int pos, void* userdata)
{
    cvSetTrackbarPos("New Hue ","HSV",pos);
}
void change_s(int pos, void* userdata)
{
    cvSetTrackbarPos("New Sat ","HSV",pos);
}
void change_v(int pos, void* userdata)
{
    cvSetTrackbarPos("New Val ","HSV",pos);
}
void thresh_image()
{

    cvtColor(n_image,n_image,CV_BGR2HSV);
    inRange(image,Scalar(h_min,s_min,v_min),Scalar(h_max,s_max,v_max),h_image);
    Vec3b color;
    for(int x=0;x<image.rows;x++)
    {
        for(int y=0;y<image.cols;y++)
        {
            color = image.at<Vec3b>(Point(y,x));
            if(x>=x_min && x<x_max && y>=y_min && y<y_max && color[0] >= h_min && color[0] <= h_max && color[1] >= s_min && color[1] <= s_max && color[3] >= v_min && color[3] <= v_max)
            {
                color[0]=color[0]+h_new-h_min;
                if (color[0]>=180)
                    color[0]-=180;
                else if (color[0]<0)
                    color[0]+=180;

                color[1]=color[1]+s_new-s_min;
                if (color[1]>255)
                    color[1]=255;
                else if (color[1]<0)
                    color[1]=0;

                color[2]=color[2]+v_new-v_min;
                if (color[2]>255)
                    color[2]=255;
                else if (color[2]<0)
                    color[2]=255;
            }
            n_image.at<Vec3b>(Point(y,x))=color;
        }
    }
    cvtColor(n_image,n_image,CV_HSV2BGR);

}
int main(int argc,char* argv[])
{
    char filename[MAX_PATH]={""};
    OPENFILENAME ofn;
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename; //buffer to place the the file path
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFilter = "Images\0*.png;*.jpg;*.bmp;*.jpeg;*.tiff;*.pbm,;*.pgm,;*.ppm;*.pxm,;*.pnm\0All\0*.*\0";// kind of files to be opened
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // program starts here
    printf("Press:\n\tCtrl+a : apply changes\n\tCtrl+s : save to same image\n\tCtrl+d : save to new  image\n\tCtrl+o : open another image\n\tESQ    : exit\n");

    GetOpenFileName(&ofn);
    image=imread(filename,IMREAD_LOAD_GDAL);
    if (image.empty())
    {
        MessageBeep(0);
        MessageBoxA(NULL,"Failed to open image!","Error",MB_OK);
        return -1;
    }
    // windows initialization
    cvNamedWindow("Image",WINDOW_KEEPRATIO);
    cvNamedWindow("Theashold Based Image",CV_WINDOW_KEEPRATIO);
    cvNamedWindow("New Image",CV_WINDOW_KEEPRATIO);
    cvNamedWindow("HSV",CV_WINDOW_KEEPRATIO);

    resize_windows();
    cvResizeWindow("HSV",500,600);

    // track bars initialization
    createTrackbar("Hue Min","HSV",&h_min,179,change_h);
    createTrackbar("Hue Max","HSV",&h_max,179,NULL    );

    createTrackbar("Sat Min","HSV",&s_min,255,change_s);
    createTrackbar("Sat Max","HSV",&s_max,255,NULL    );

    createTrackbar("Val Min","HSV",&v_min,255,change_v);
    createTrackbar("Val Max","HSV",&v_max,255,NULL    );

    createTrackbar("New Hue ","HSV",&h_new,180,NULL);
    createTrackbar("New Sat ","HSV",&s_new,255,NULL);
    createTrackbar("New Val ","HSV",&v_new,255,NULL);

    createTrackbar("Min Row","HSV",&x_min,image.rows,NULL);
    createTrackbar("Max Row","HSV",&x_max,image.rows,NULL);
    createTrackbar("Min Col","HSV",&y_min,image.cols,NULL);
    createTrackbar("Max Col","HSV",&y_max,image.cols,NULL);

    n_image=image.clone();
    imshow("Image",image);
    cvtColor(image,image,CV_BGR2HSV);

    int key;
    do
    {
        thresh_image();
        imshow("New Image",n_image);
        imshow("Theashold Based Image",h_image);
        //Wait 80mS
        key = cvWaitKey(80);
        //If 'ESC' is pressed, break the loop
        switch(key)
        {
            case  1:
                {
                    cvtColor(image,image,CV_HSV2BGR);
                    image=n_image.clone();
                    imshow("Image",image);
                    cvtColor(image,image,CV_BGR2HSV);
                    h_new=h_min;s_new=s_min;v_new=v_min;
                    break;
                }
            case 15:
                {
                    GetOpenFileName(&ofn);
                    image=imread(filename,IMREAD_LOAD_GDAL);
                    if (image.empty())
                    {
                        MessageBeep(0);
                        MessageBoxA(NULL,"Failed to open image!","Error",MB_OK);
                        printf("Error: Failed to image");
                        return -1;
                    }
                    n_image=image.clone();
                    resize_windows();
                    imshow("Image",image);
                    cvtColor(image,image,CV_BGR2HSV);
                    break;
                }
            case 4:
                {
                    char* name=new char[MAX_PATH];
                    char* buffer=new char[MAX_PATH];
                    char* ext=new char[10];
                    strcpy(name,filename);
                    strtok(name,".");
                    sprintf(buffer,"%s.%%s",name);
                    sscanf(filename,buffer,ext);
                    sprintf(filename,"%s_edited.%s",name,ext);
                }
            case 19:
                imwrite(filename,n_image);
                struct stat path;
                if (stat(filename,&path))
                {
                    MessageBeep(0);
                    MessageBox(NULL,"Failed to save image","Error",MB_OK);
                }
                else
                {
                    char *message=new char[MAX_PATH];
                    sprintf(message,"Successfully saved image to:\n%s",filename);
                    MessageBox(NULL,message,"Message",MB_OK);
                }
            default: break;
        }
    }while(key!=27);

    return 0;
}
