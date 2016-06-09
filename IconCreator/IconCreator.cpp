/// This program creates 23 various sized PNG icons used in packing Android Apps
// OpenCV : image reading and writing, image processing
// Other  : open file name dialog, checking for directories and creating them, parsing input passed arguments
// libraries to link:
//      libopencv_imgcodecs310.dll.a
//      libopencv_imgproc310.dll.a
//      libcomdlg32.a

#include <windows.h>
#include <direct.h>
#include <sys/stat.h>

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
using namespace cv;

int main(int argc,char *argv[])
{
    OPENFILENAME ofn;
    char filename[MAX_PATH];
    // Initialize OPENFILENAME
    memset(&ofn,0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.nMaxFile = sizeof(filename);
    ofn.lpstrFile[0] = '\0';
    ofn.lpstrFilter = "Images\0*.png;*.jpg;*.bmp;*.jpeg;*.tiff;*.pbm,;*.pgm,;*.ppm;*.pxm,;*.pnm\0All\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (argc>=2)
    {
        sprintf(filename,"%s",argv[1]);
        for (int i=2;i<argc;i++)
            sprintf(filename,"%s %s",filename,argv[i]);
    }
    else
    {
        GetOpenFileName(&ofn);
    }

    // image processing starts here
    Mat image;
    Mat src=cv::imread(filename,IMREAD_LOAD_GDAL);
    if(src.empty())                               // Check for invalid input
    {
        MessageBeep(0);
        MessageBox(NULL,"Could not open or find the source image","Error",0);
        return -1;
    }
    printf("Source image is : %s\n\n",filename);

    // define icons data (names,sizes)
    std::string names[23]={
         "Icon.png"
        ,"Icon@2x.png"
        ,"Icon-60.png"
        ,"Icon-60@2x.png"
        ,"Icon-60@3x.png"
        ,"Icon-72.png"
        ,"Icon-72@2x.png"
        ,"Icon-76.png"
        ,"Icon-76@2x.png"
        ,"Icon-hdpi.png"
        ,"Icon-ldpi.png"
        ,"Icon-mdpi.png"
        ,"Icon-Small.png"
        ,"Icon-Small@2x.png"
        ,"Icon-Small@3x.png"
        ,"Icon-Small-40.png"
        ,"Icon-Small-40@2x.png"
        ,"Icon-Small-40@3x.png"
        ,"Icon-Small-50.png"
        ,"Icon-Small-50@2x.png"
        ,"Icon-xhdpi.png"
        ,"Icon-xxhdpi.png"
        ,"Icon-xxxhdpi.png"
    };
    int ico_size[23]={57,114,60,120,180,72,144,76,152,72,36,48,29,58,87,40,80,120,50,100,96,144,192};

    // creates a folder named if "Icons" if it does not exist
    struct stat dir;
    if (stat("Icons",&dir))
        mkdir("Icons");

    for (int i=0;i<23;i++)
    {
        cv::resize(src,image,Size(ico_size[i],ico_size[i]));
        sprintf(filename,"Icons/%s",names[i].c_str());
        cv::imwrite(filename,image);
        printf("Successfully Created %s\n",names[i].c_str());
    }
    printf("Icon Creation is Complete.\n\n");
    system("pause");
    return 0;
}
