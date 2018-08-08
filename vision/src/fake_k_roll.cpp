#include <ros/ros.h>
#include <std_msgs/UInt32.h>

int main(int argc, char* argv[])
{
    ros::init(argc, argv, "fake_carol");
    ros::NodeHandle node;

    //tf2_ros::TransformBroadcaster tfb;

    ros::Rate loop_rate(0.2);
    
   ros::Publisher pub = node.advertise<std_msgs::UInt32>("fake_k_roll", 1000); 
    
    ROS_INFO("Starting fake publisher");
    while(ros::ok()) {
    
		std_msgs::UInt32 msg;
		
		msg.data = 1;
		
		pub.publish(msg);
		ros::spinOnce();
		ROS_INFO("Lenny is asking for cv_data ");//%.2f, %.2f, %.2f, %.2f, %d, %.2f", msg.x, msg.y, msg.z, msg.angle * 180.0 / 3.141592, msg.type);
        // Wait 1 second to publish again
		loop_rate.sleep();
    }

    ROS_INFO("Publisher done.");
    return 0;
}
