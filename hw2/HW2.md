# Homework 2 - Camera Calibration

### Usage
```
cd hw2
wget http://cv2.csie.ntu.edu.tw/CV2/_private/photo.rar
unrar x photo.rar 
mkdir build && cd build
cmake ..
make
cd ..
./build/hw2
```

## Description
* Camera calibration i.e. compute object displacement(#mm/ pixel )
* Calculate horizontal field of view in degrees of angle.
* Calculate theoretical values (FOV) and compare with measured values
* Use lens of focal length: `18mm`, `53mm`, `135mm`
* Object displacement of: `1mm`, `5mm`, `10mm`, `20mm`
* Object distance of: `0.6m`, `1.2m`, `1.8m`
* Camera : `Pentax K-7, 18-135mm`
* Sensor size : `23.4 x 15.6 mm`
