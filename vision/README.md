![imagen](https://raw.githubusercontent.com/ctaipuj/lenny_vision/master/lenny_vision.png)
# Vision Package *Developed by Nicolas Barrero 2017*

## Overview
This package contains a computer vision algorithm that classify three types of plastic bottles. The algorithm uses a kinect sensor for RGB image processing and then it uses kinect's depth data to estimate the 3D pose of the detected object with respect to the robot's base. This is part of an undergraduate project developed at *Pontificia Universidad Javeriana*. See the [published document](https://repository.javeriana.edu.co/bitstream/handle/10554/36451/BarreroLizarazoNicolas2017.pdf?sequence=1&isAllowed=y) for further information.

## Contents

1. launch  
2. msg
3. include
4. src

## Requirements

1. [opencv](https://opencv.org/releases.html)
2. [Avin2's module for Kinect] (https://github.com/avin2/SensorKinect)
3. [environment control package](https://github.com/ctaipuj/environment_interaction)  
In your `catkin_ws/src`  
`git clone https://github.com/ctaipuj/environment_interaction.git`
4. [cv_bridge package](http://wiki.ros.org/cv_bridge/Tutorials/UsingCvBridgeToConvertBetweenROSImagesAndOpenCVImages)  
In your `catkin_ws/src`  
`git clone https://github.com/ros-perception/vision_opencv.git`

Then compile your workspace.

## 1. launch

This directory contains the required launch file. The `vision.launch` file runs the following nodes:

```xml
<node name="kinect_to_robot_tf" pkg="vision" type="kinect_to_robot_tf"/>
  
<node name="rqt_reconfigure" pkg="rqt_reconfigure" type="rqt_reconfigure"/>
  
<node name="bins_tracker" pkg="objects_tracking" type="bins_tracker"/>
  
<node name="bottle_publisher" pkg="objects_tracking" type="bottle_publisher"/>
  
<node name="bottle_tf_publisher" pkg="objects_tracking" type="bottle_tf_publisher"/>
```

## 2. msg

This include the created message that transport the required information through ROS.

### bottle_data.msg

This message is composed by

* `geometry_msgs/PoseStamped P` 3D pose of the detected object wrt the robot's base link
* `float64 length` object length in m
* `float64 width` object width in m
* `int32 type` type of detected object
	* `1` for Colored PET
	* `2`for Green-colored PET 
	* `3` for HDPE/PS

You can include this message in your own node:

```c++
#include <vision/bottle_data.h>

vision::bottle_data msg;
```

In this package this message is published through two topics each one has the information of the target for each arm of the robot. (Dual-arm robot):

* `vision/bottle_data_arm_right`
* `vision/bottle_data_arm_left`

## 3. include

Include files.

* `bridge.h` Core functions for image transport in ROS, deals with data publication and subscription.
* `detection.h`Class focused to classify the bottles according to the plastic type.
* `perception.h`Class created to deal with 3D pose estimation with respect to the robot of the bottle using the RGB-D sensor.

See [documented code](https://github.com/ctaipuj/lenny_vision/tree/master/documented_code) for more information about the code.


## 4. src

source files.

* `kinect_to_robot.cpp` is a ROS node that creates a tf between the `camera_link` and the `torso_base_link`
* `fake_k_roll.cpp` Is a node that publish a fake variable. Used for individual tests 
* `image_converter.cpp` The main node that manages the ROS image data type and transforms it into *OpenCV* image data, executes the 3D pose estimation and publish spatial coordinates to ROS.
* `polycolor.cpp`source code for detection class
* `space.cpp` source code for perception class

Other files in the src directory are deprecated.

## Usage

First launch the Kinect camera:

`roslaunch openni_launch openni.launch`

Then in another terminal launch the vision package:

`roslaunch vision vision.launch`

This launch file runs these nodes:

* `kinect_to_robot_tf` publishes a static tf
* `rqt_reconfigure` launches reconfiguration GUI
* `bins_tracker` publishes bins geometries to MoveIt! planning scene interface
* `bottle_publisher` publishes bottles to MoveIt! planning scene interface

Reconfigure the Kinect's driver in order to change image resolution and enable depth registration:

![imagen](https://raw.githubusercontent.com/ctaipuj/lenny_vision/master/vision/rqt_reconfigure.png)

Once the reconfiguration is done you can close the `rqt_reconfigure`window.

Finally run the node that executes the computer vision algorithm in another terminal:

`rosrun vision image_converter`

This node will publish the `vision/bottle_data_arm_right` and `vision/bottle_data_arm_left` topics with the data of the detected object.

To interact with the node follow the instructions shown in console:

Command line Interface:
![imagen](https://raw.githubusercontent.com/ctaipuj/lenny_vision/master/vision/command_line.png)

Message Publication:
![imagen](https://raw.githubusercontent.com/ctaipuj/lenny_vision/master/vision/publication.png) 

### Known Issues
* Remember to select the image window when pressing the key to continue.
* Bottles IDs are also printed in the *detected bottles* window.
* For unknown reasons in the first attempt it is neccesary to write the object ID twice.
* The first message published is published to the left arm the second one is the command for the right one.
* The node will wait until the robot finishes its execution. If something goes wrong with the robot you will have to kill the node.

### NOTE

* If something goes wrong you can always `ctrl^c`the node and run it again.
* Usually the windows of OpenCV will go dark. It is okay since keyboard interaction with the system is required.
* The computer vision algorithm assumes the robot is "left-handed" so the first message goes to the left arm and the second message goes to the right arm. 



***
*Created by Nicolas Barrero May 2018*  
*Modified by Nicolas Barrero Nov 2018*  
**Developed at Pontificia Universidad Javeriana Nov 2018 Bogotá D.C-Colombia**  
**"Centro Tecnologico de Automatizacion Industrial" CTAI  
Perception For Industrial Robots Project**

![imagen](https://bit.ly/2qVzHyL)
