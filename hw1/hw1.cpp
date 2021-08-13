#include<iostream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    if(argc != 3)
    {
        cout << "Error usage!" << endl;
        return 1;
    }

    string img1_path = argv[1];
    string img2_path = argv[2];
    Mat img1 = imread(img1_path, IMREAD_GRAYSCALE);
    Mat img2 = imread(img2_path, IMREAD_GRAYSCALE);
    if(img1.empty())
    {
        cout << "Could not read the image: " << img1_path << endl;
        return 1;
    }
    else if(img2.empty())
    {
        cout << "Could not read the image: " << img2_path << endl;
        return 1;
    }

    return 0;
}