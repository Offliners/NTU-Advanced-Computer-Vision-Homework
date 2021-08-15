#include<iostream>
#include<fstream>
#include<vector>
#include<cmath>
#include<limits>
#include<utility>
#include<fstream>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#define num_f 3
#define num_object_displacement 5
#define num_object_distance 3
#define sensor_width 23.4

using namespace std;
using namespace cv;

typedef vector< pair<Point2i, Mat> > imgSet;

imgSet getBlock(Mat&, Point2i, int, int);
int getDisplacementPixel(Mat&, Point2i, Mat&);
double theoreticalFOV(int);
double mmPrePixel(int, int);
double measuredFOV(int, int, double);

int main(int argc, char **argv)
{
    int f[num_f] = {18, 53, 135};
    int object_displacement[num_object_displacement] = {0, 1, 5, 10, 20};
    int object_distance[num_object_distance] = {600, 1200, 1800};
    vector< vector< vector<Mat> > > imgDataset(num_f, vector< vector<Mat> >(num_object_distance, vector<Mat>(num_object_displacement)));

    // Read image
    for(int i = 0; i < num_f; ++i)
    {
        for(int j = 0; j < num_object_distance; ++j)
        {
            for(int k = 0; k < num_object_displacement; ++k)
            {
                string img_name = "Photo/" + to_string(f[i]) + "mm/" + to_string(object_distance[j]) + "mm_" + to_string(object_displacement[k]) + "mm.jpg";
                Mat img = imread(img_name);

                if(img.empty())
                {
                    img_name = "Photo/" + to_string(f[i]) + "mm/" + to_string(object_distance[j]) + "mm_" + to_string(object_displacement[k]) + "mm.JPG";
                    img = imread(img_name);
                }
                // imgDataset[i][j].push_back(img);
                img.copyTo(imgDataset[i][j][k]);
            }
        }
    }

    // Calculate mm per pixel
    vector< vector< vector<int> > > pixel_dataset(num_f, vector< vector<int> >(num_object_distance, vector<int>(num_object_displacement)));
    for(int i = 0; i < num_f; ++i)
    {
        for(int j = 0; j < num_object_distance; ++j)
        {
            string roi_name = to_string(f[i]) + "mm_" + to_string(object_distance[j]) + "mm";
            string window_name = "Select ROI " + roi_name;
            namedWindow(window_name, WINDOW_NORMAL);
            Rect2d img_obj = selectROI(window_name, imgDataset[i][j][0]);
            destroyWindow(window_name);
            Mat obj = imgDataset[i][j][0](img_obj);
            Point2i obj_loc(img_obj.x, img_obj.y);

            imwrite("roi/" + roi_name + ".jpg", obj);
            cout << roi_name << " done" << endl;

            for(int k = 0; k < num_object_displacement; ++k)
                pixel_dataset[i][j][k] = getDisplacementPixel(obj, obj_loc, imgDataset[i][j][k]);
        }
    }

    // Write CSV
    ofstream output;
    output.open("output/output.csv");
    output << "Focal length(mm), Object distance(mm), Object actual displacement(mm), Object displacement(pixel), mm/pixel, FOV theoretical value(degree), FOV measured value(degree)" << endl;
    for(int i = 0; i < num_f; ++i)
    {
        
        for(int j = 0; j < num_object_distance; ++j)
        {
            for(int k = 0; k < num_object_displacement; ++k)
            {
                if(object_displacement[k] == 0)
                    continue;

                double mm_pre_pixel = mmPrePixel(object_displacement[k], pixel_dataset[i][j][k]);
                int img_width = imgDataset[i][j][k].cols;

                output << f[i] << "," << object_distance[j] << "," << object_displacement[k] << "," << pixel_dataset[i][j][k] << "," << mm_pre_pixel << "," << theoreticalFOV(f[i]) << "," << measuredFOV(img_width, object_distance[j], mm_pre_pixel) << endl;
            }
        }
    }
    output.close();

    return 0;
}

imgSet getBlock(Mat &img, Point2i obj_loc, int window_row, int window_col)
{
    imgSet temp;
    for(int i = obj_loc.x; i < img.cols - window_col; i += 1)
    {
        Mat window(img, Rect(i, obj_loc.y, window_col, window_row));
        temp.push_back( make_pair(Point2i(i, obj_loc.y), window) );
    }

    return temp;
}

int getDisplacementPixel(Mat &obj, Point2i obj_loc, Mat &img)
{
    imgSet img_dataset = getBlock(img, obj_loc, obj.rows, obj.cols);
    double loss = numeric_limits<double>::max();
    Point2i matchPoint;
    double search_range = 600.0;

    for(int i = 0; i < img_dataset.size(); ++i)
    {
        double dis = img_dataset[i].first.x - obj_loc.x;
        if(dis <= search_range)
        {
            Mat diff;
            absdiff(obj, img_dataset[i].second, diff);
            double current_loss = sum(diff)[0] + sum(diff)[1] + sum(diff)[2];
            if(current_loss < loss)
            {
                loss = current_loss;
                matchPoint = img_dataset[i].first;
            }
        }
    }

    return matchPoint.x - obj_loc.x;
}

double theoreticalFOV(int f)
{
    return 2 * atan(sensor_width / 2 / f) * 180 / M_PI;
}

double mmPrePixel(int d, int p)
{
    return (double)d / p;
}

double measuredFOV(int img_width, int d, double mm_pre_pixel)
{
    return 2 * atan(img_width * mm_pre_pixel / 2 / d) * 180 / M_PI;
}