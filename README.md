![imagen](https://raw.githubusercontent.com/ctaipuj/lenny_vision/master/lenny_vision.png)
# Lenny Vision Package *Developed by Nicolas Barrero 2017*

## Overview
This package contains a computer vision algorithm that classify three types of plastic bottles. The algorithm uses a kinect sensor for RGB image processing and then it uses kinect's depth data to estimate the 3D pose of the detected object with respect to the robot's base. This is part of an undergraduate project developed at *Pontificia Universidad Javeriana*. See the published document [here](https://repository.javeriana.edu.co/bitstream/handle/10554/36451/BarreroLizarazoNicolas2017.pdf?sequence=1&isAllowed=y) for further information.

It also include a modified version of the Openni Camera package.

## Contents

1. [Documented Code](https://github.com/ctaipuj/lenny_vision/tree/master/documented_code)  
Information about the computer vision algorithm.
2. [Openni Camera](https://github.com/ctaipuj/lenny_vision/tree/master/openni_camera)  
A modified version of the [openni_camera](https://github.com/ros-drivers/openni_camera) package. Required when using Primesense sensors as the Kinect.
3. [Vision](https://github.com/ctaipuj/lenny_vision/tree/master/vision)  
Main package for the computer vision algorithm.
4. kinect_cal.yaml  
Intrinsic calibration parameters for the Kinect sensor, used with *openni launch* package.

## NOTE

* Please see [Vision](https://github.com/ctaipuj/lenny_vision/tree/master/vision) for usage and other requirements.
* When using a kinect sensor it is necessary to use [avin2's plugin](https://github.com/avin2/SensorKinect). Find more information [here](http://blog.justsophie.com/installing-kinect-nite-drivers-on-ubuntu-14-04-and-ros-indigo/) and [here](https://docs.opencv.org/2.4/doc/user_guide/ug_kinect.html) about Kinect installation.

***
*Created by Nicolas Barrero May 2018*  
*Modified by Nicolas Barrero Nov 2018*  
**Developed at Pontificia Universidad Javeriana Nov 2018 Bogotá D.C-Colombia**  
**"Centro Tecnologico de Automatizacion Industrial" CTAI  
Perception For Industrial Robots Project**

![imagen](https://bit.ly/2qVzHyL)
