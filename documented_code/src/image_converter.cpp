#include "../include/bridge.h"
#include "../include/perception.h"
#include "../include/detection.h"

//! This function launches the ros node that perform the image processing task.
/*!
	This main launch a ros node that manage the ROS-OpenCV communication and conversions, then it executes the computer vision algorithm and send the data back to ROS
	*/
int main(int argc, char** argv)
{	
	ros::init(argc, argv, "computer_vision");
	ros::NodeHandle n;
	space s;
	polycolor c;
	ImageConverter ic;//mi clase para convertir entre ros y opencv
	ic.transform_listener_ptr=TransformListenerPtr(new tf::TransformListener());
	//cv::imwrite("../../depth_ros.tiff",B);
	//cv::imshow("sdsjdv",ic.getcvcolorimage());
	ros::spin();
	return 0;
}
