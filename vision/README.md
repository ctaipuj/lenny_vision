# vision_for_lenny Package 
### Developed by Nicolas Barrero 2017

This package integrates a computer vision algorithm with ROS's moveit! to perform a waste separation task (Sorting of plastic bottles)

## Content

1. **launch** Launch files
2. **msg** A created message
3. **include** .h files for the computer vision task
4. **src** .cpp files of the computer vision task
	* **kinect_to_robot.cpp** is a ROS node that creates a tf between the `camera_link` and the `torso_base_link`
	* **fake_k_roll.cpp** Is a node that publish a fake variable. Used for individual test. 
	* **image_converter.cpp** The main node that manages the ROS' image data type and transforms it into *OpenCV* image data executes the 3d pose estimation and publish spatial coordinates to ROS.

### NOTE

* Please see the *documented_code* folder for more information about the code

* The code used here has slights modification from the code in *documented_code* folder this due to make the useful with ROS

*Created by Nicolas May 2018*
