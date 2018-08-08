#include "../include/bridge.h"
#include "../include/perception.h"
#include "../include/detection.h"

//cv::Mat B(480,640,CV_16UC1);
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
