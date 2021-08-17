# Homework 3 - Calculating Optical Flow

### Origin Iamge
![lena](data/lena.bmp)

### Usage
```
cd hw3
mkdir build && cd build
cmake ..
make
cd ..
./build/hw3 data/lena.bmp
```
All generated images are saved in `demo`

## Description
* Implement Horn & Schunck optical flow estimation.
* Synthetically translate lena.bmp one pixel to the right and downward.
* Try λ of `0.1`, `1`, `10`.
* For each λ, show the result after `1`, `4`, `16`, `64` iterations.

## Result
|Iteration|lambda = 0.1|lambda = 1|lambda = 10|
|-|-|-|-|
|1|![HornSchunck_iter1_lamb0.1](demo/HornSchunck_iter1_lamb0.1.jpg)|![HornSchunck_iter1_lamb1](demo/HornSchunck_iter1_lamb1.jpg)|![HornSchunck_iter1_lamb10](demo/HornSchunck_iter1_lamb10.jpg)|
|4|![HornSchunck_iter4_lamb0.1](demo/HornSchunck_iter4_lamb0.1.jpg)|![HornSchunck_iter4_lamb1](demo/HornSchunck_iter4_lamb1.jpg)|![HornSchunck_iter4_lamb10](demo/HornSchunck_iter4_lamb10.jpg)|
|16|![HornSchunck_iter16_lamb0.1](demo/HornSchunck_iter16_lamb0.1.jpg)|![HornSchunck_iter16_lamb1](demo/HornSchunck_iter16_lamb1.jpg)|![HornSchunck_iter16_lamb10](demo/HornSchunck_iter16_lamb10.jpg)|
|64|![HornSchunck_iter64_lamb0.1](demo/HornSchunck_iter64_lamb0.1.jpg)|![HornSchunck_iter64_lamb1](demo/HornSchunck_iter64_lamb1.jpg)|![HornSchunck_iter64_lamb10](demo/HornSchunck_iter64_lamb10.jpg)|

## Lambda = 0.1 changes with increasing iterations 
![lambda 0.1](img/lambda0.1.gif)
