#include<iostream>
#include<vector>
#include<limits>
#include<cmath>
#include<utility>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

typedef vector< pair<Point2i, Mat> > imgSet;
typedef vector< pair<Point2i, Point2i> > vectorSet;

imgSet getBlock(Mat, int, int);
vectorSet getMotion(imgSet&, imgSet&, double);
void createResultImg(Mat, Mat, int);

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

    createResultImg(img1, img2, 9);   // 9×9
    createResultImg(img1, img2, 11);  // 11×11
    createResultImg(img1, img2, 15);  // 15×15
    createResultImg(img1, img2, 21);  // 21×21
    createResultImg(img1, img2, 31);  // 31×31

    return 0;
}

imgSet getBlock(Mat img, int windowSize, int stride)
{
    imgSet temp;
    for(int i = 0; i < img.rows - windowSize; i += stride)
    {
        for(int j = 0; j < img.cols - windowSize; j += stride)
        {
            Mat window(img, Rect(j, i, windowSize, windowSize));
            temp.push_back( make_pair(Point2i(j, i), window) );
        }
    }

    return temp;
}

vectorSet getMotion(imgSet &target, imgSet &source, double search_range)
{
    vectorSet temp;
    for(int i = 0; i < source.size(); ++i)
    {
        double loss = numeric_limits<double>::max();
        Point2i matchPoint;
        for(int j = 0; j < target.size(); ++j)
        {
            double dis = sqrt(pow(target[j].first.x - source[i].first.x, 2) + pow(target[j].first.y - source[i].first.y, 2));
            if(dis <= search_range)
            {
                Mat diff;
                absdiff(target[j].second, source[i].second, diff);
                double current_loss = sum(diff)[0];
                if(current_loss < loss)
                {
                    loss = current_loss;
                    matchPoint = target[j].first;
                }
            }
        }

        int center = source[0].second.rows / 2 + 1;
        matchPoint.x += center;
        matchPoint.y += center;
        Point2i start = source[i].first;
        start.x += center;
        start.y += center;
        temp.push_back( make_pair(start, matchPoint) );
    }

    return temp;
}

void createResultImg(Mat img1, Mat img2, int block_size)
{
    imgSet data_a = getBlock(img1, block_size, 1);
    imgSet data_b = getBlock(img2, block_size, block_size);
    vectorSet vec = getMotion(data_a, data_b, 50.0);
    Mat result(img2.rows, img2.cols, CV_8UC3, Scalar(255, 255, 255));
    for(auto p : vec)
        arrowedLine(result, p.first, p.second, Scalar(255, 0, 0));

    string file_name = "block" + to_string(block_size) + "×" + to_string(block_size) + ".png";
    imwrite("demo/" + file_name, result);
    cout << "Downloaded " + file_name << endl;
}