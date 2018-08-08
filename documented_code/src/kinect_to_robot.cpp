#include <ros/ros.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>
//! This function launches the ros node that manage spatial static transformations.
/*!
	This main launch a ros node that manage spatial transformation between the camera's frame and the robot's one. This node perform a "spatial-geometric communication between both systems through the framework.
	*/
int main(int argc, char** argv){
  ros::init(argc, argv, "kinect_broadcaster");
  ros::NodeHandle node;

   tf2_ros::TransformBroadcaster tfb;
  geometry_msgs::TransformStamped transformStamped;

 
  transformStamped.header.frame_id = "kinect";
  transformStamped.child_frame_id = "camera_link";
  transformStamped.transform.translation.x = 0.14;
  transformStamped.transform.translation.y = 0.0;
  transformStamped.transform.translation.z = 0.05;
  tf2::Quaternion q;
        q.setRPY(0 ,0,-1.57);
  transformStamped.transform.rotation.x = q.x();
  transformStamped.transform.rotation.y = q.y();
  transformStamped.transform.rotation.z = q.z();
  transformStamped.transform.rotation.w = q.w();

  ros::Rate rate(50);
  while (node.ok()){
    transformStamped.header.stamp = ros::Time::now();
    tfb.sendTransform(transformStamped);
    rate.sleep();
  }

};
