#include <ros/ros.h>
#include <tf2_ros/transform_broadcaster.h>
#include <tf2/LinearMath/Quaternion.h>

int main(int argc, char** argv){
  ros::init(argc, argv, "skeleton_tracker_broadcaster");
  ros::NodeHandle node;

   tf2_ros::TransformBroadcaster tfb;
  geometry_msgs::TransformStamped transformStamped;

 
  transformStamped.header.frame_id = "camera_depth_frame";
  transformStamped.child_frame_id = "openni_depth_frame"; //PARAMETROS PROPIOS DE LA GEOMETRIA DEL KINECT DO NOT CHANGE!
  transformStamped.transform.translation.x = 0.0;
  transformStamped.transform.translation.y = 0.0;
  transformStamped.transform.translation.z = 0.0;
  tf2::Quaternion q;
        q.setRPY(0 ,0, 0);
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
