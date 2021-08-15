#include<iostream>
#include<cmath>
#include<vector>
#include<sstream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void drawOpticalFlow(Mat&, Mat&, int, double);
void calHornSchunck(Mat&, Mat&, Mat&, Mat&, int, double);
void calGradient(Mat&, Mat&, Mat&, Mat&, Mat&);

int main(int argc, char **argv)
{
    if(argc != 2)
    {
        cout << "Error usage!" << endl;
        return 1;
    }

    string img_path = argv[1];
    Mat img = imread(img_path, IMREAD_GRAYSCALE);
    if(img.empty())
    {
        cout << "Could not read the image: " << img_path << endl;
        return 1;
    }

    Mat img_shift;
    double shift = 1.0;
    Mat shiftM = (Mat_<double>(2, 3) << 1, 0, shift, 0, 1, shift);
    warpAffine(img, img_shift, shiftM, Size(img.cols, img.rows));

    vector<int> iterSet = {1, 4, 16, 64};
    vector<double> lambSet = {0.1, 1, 10};

    for(auto p : iterSet)
        for(auto t : lambSet)
            drawOpticalFlow(img, img_shift, p, t);

    return 0;
}

void drawOpticalFlow(Mat &img1, Mat &img2, int iter, double lamb)
{
    Mat u, v, result;
    img2.copyTo(result);
    int scale = 10;

    calHornSchunck(img1, img2, u, v, iter, lamb);
    for(int i = 2; i < u.cols; i += scale)
        for(int j = 2; j < v.cols; j += scale)
            arrowedLine(result, Point(i, j), Point(i + scale * u.at<double>(i, j), j + scale * v.at<double>(i, j)), Scalar(255));

    ostringstream temp;
    temp << lamb;
    string file_name = "demo/HornSchunck_iter" + to_string(iter) + "_lamb" + temp.str() + ".jpg";
    imwrite(file_name, result);
    cout << "Downloaded " + file_name << endl;
}

void calHornSchunck(Mat &img1, Mat& img2, Mat &u, Mat &v, int iter, double lamb)
{
    Mat gradX, gradY, gradt;
    calGradient(img1, img2, gradX, gradY, gradt);
    u = Mat::zeros(gradt.rows, gradt.cols, CV_64FC1);
    v = Mat::zeros(gradt.rows, gradt.cols, CV_64FC1);
    int window_size = 3;
    Mat kernel = Mat::ones(window_size, window_size, CV_64FC1) / pow(window_size, 2);
    Point anchor(window_size - (window_size / 2) - 1, window_size - (window_size / 2) - 1);
    
    for (int i = 0; i < iter; ++i)
    {
        Mat uAvg, vAvg, gradXuAvg, gradYvAvg, gradXgradX, gradYgradY, updateConst, uUpdateConst, vUpdateConst;
        
        filter2D(u, uAvg, u.depth(), kernel, anchor, 0, BORDER_CONSTANT);
        filter2D(v, vAvg, v.depth(), kernel, anchor, 0, BORDER_CONSTANT);
        
        multiply(gradX, uAvg, gradXuAvg);
        multiply(gradY, vAvg, gradYvAvg);
        multiply(gradX, gradX, gradXgradX);
        multiply(gradY, gradY, gradYgradY);
        
        divide((gradXuAvg + gradYvAvg + gradt), (pow(lamb, 2) + gradXgradX + gradYgradY), updateConst);
        multiply(gradX, updateConst, uUpdateConst);
        multiply(gradY, updateConst, vUpdateConst);
        
        u = uAvg - uUpdateConst;
        v = vAvg - vUpdateConst;
    }
}

void calGradient(Mat &img1, Mat &img2, Mat &X, Mat &Y, Mat &t)
{
    Mat img1Norm, img2Norm;
    
    img1.convertTo(img1Norm, CV_64FC1);
    img2.convertTo(img2Norm, CV_64FC1);
    Sobel(img1Norm, X, -1, 1, 0, 3);
    Sobel(img1Norm, Y, -1, 0, 1, 3);
    t = img2Norm - img1Norm;
}