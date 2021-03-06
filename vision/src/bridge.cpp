#include "../include/bridge.h"
#include "../include/perception.h"
#include "../include/detection.h"

#include<boost/assign/std/vector.hpp>

ImageConverter::ImageConverter()
	: it_(nh_)
{
	// Subscribe to input video feed and publish output video feed
	image_sub_color = it_.subscribe("/kinect/rgb/image_color", 1, &ImageConverter::kinect_color_callback, this);
	image_sub_depth = it_.subscribe("/kinect/depth_registered/image", 1, &ImageConverter::kinect_depth_callback, this);
	sub = nh_.subscribe("pick_and_place/command", 30,  &ImageConverter::lenny_asks_callback,this); ///pick_and_place/command
	
	pub_right= nh_.advertise<vision::bottle_data>("vision/bottle_data_arm_right", 50);
	ros::Duration(0.5).sleep();
	pub_left= nh_.advertise<vision::bottle_data>("vision/bottle_data_arm_left", 50);
	ros::Duration(0.5).sleep();
	
	cv::namedWindow("Color Image",CV_WINDOW_NORMAL);
	k_roll=1;
}

ImageConverter::~ImageConverter(){

	cv::destroyWindow("Color Image");
}

void ImageConverter::lenny_asks_callback(const std_msgs::UInt32& msg){
	
	k_roll=msg.data;
}

void ImageConverter::kinect_color_callback(const sensor_msgs::ImageConstPtr& color_msg){
	
	
	cv_bridge::CvImagePtr cv_ptr_color;
	try{
		cv_ptr_color = cv_bridge::toCvCopy(color_msg, sensor_msgs::image_encodings::BGR8);
		}
		
	catch (cv_bridge::Exception& e){
		ROS_ERROR("cv_bridge exception: %s", e.what());
	return;
	}

	cv::imshow("Color Image", cv_ptr_color->image);
	// Output modified video stream
	//image_pub_.publish(cv_ptr_color->toImageMsg());
	A_color=cv_ptr_color->image;
}

void ImageConverter::kinect_depth_callback(const sensor_msgs::ImageConstPtr& depth_msg){

	std::vector<std::vector<double> > data_to_ros;

	cv_bridge::CvImagePtr cv_ptr_depth;
	try{
	cv_ptr_depth = cv_bridge::toCvCopy(depth_msg,"");//sensor_msgs::image_encodings::mono16);
	}
	
	catch (cv_bridge::Exception& e){
		ROS_ERROR("cv_bridge exception: %s", e.what());
	return;
	}
	B_depth=cv_ptr_depth->image;
	
	if(k_roll!=0){
	ROS_INFO("\e[1m  Time before processing \e[0m");
	double secs_before =ros::Time::now().toSec();
	computer_vision(data_to_ros); //ACA SE PROCESA Todo
	double secs_after=ros::Time::now().toSec();
	ROS_INFO("\e[1m Elapsed time %f and the FPS processed are %f \e[0m",secs_after-secs_before,1/(secs_after-secs_before));
	publisher(data_to_ros);
	}
	
}

void ImageConverter::publisher(std::vector<std::vector<double> > &data_to_ros){

	if(data_to_ros.empty()){
		ROS_ERROR("Nothing detected or no depth data available");
		return;
	}
	char key=0;

	vision::bottle_data msg_left;
	vision::bottle_data msg_right;
	
	std::vector<double> message_left(data_to_ros[0].size());
	std::vector<double> message_right(data_to_ros[0].size());
	
	message_left= selector(data_to_ros);
	
	msg_left.P=find_transformation(message_left);
		
	msg_left.length=message_left[7];
	msg_left.width=message_left[8];
		
	msg_left.type=message_left[9];
	
	if(data_to_ros.size()>1){
		
		message_right=selector(data_to_ros);
		
		msg_right.P=find_transformation(message_right);
		
		msg_right.length=message_right[7];
		msg_right.width=message_right[8];
		
		msg_right.type=message_right[9];
	}		
		//tfb.sendTransform(msg.T);
	ROS_WARN("Press a key to publish the message to Lenny. (Verification Step)");
	cv::waitKey(0);
	ROS_INFO("Message succesfully Published.....");
	pub_left.publish(msg_left);
	if(data_to_ros.size()>1){
		pub_right.publish(msg_right);
	}
}
	
std::vector<double> ImageConverter::selector(std::vector<std::vector<double> > data_to_ros){

	std::vector<double> message(data_to_ros[0].size());
	int n;
	for (int j=0;j<data_to_ros.size();j++){
		ROS_INFO("\e[1m Object ID: %i---Bottle Type %.0f \e[0m \n//T: \e[1mX\e[0m %.3f \e[1mY\e[0m %.3f \e[1mZ\e[0m %.3f | R(quaternions): x %f y %f z %f w %f // Length %.2f Width %.2f \n", j+1, data_to_ros[j][9], data_to_ros[j][0], data_to_ros[j][1], data_to_ros[j][2], data_to_ros[j][3], data_to_ros[j][4], data_to_ros[j][5], data_to_ros[j][6], data_to_ros[j][7], data_to_ros[j][8]);
	}
	ROS_WARN("Please Identify the bottles detected. Press any key to continue (Verification Step)");
	cv::waitKey(0);
	ROS_WARN("Type the Object ID to write a message");
	std::cin.clear();
	std::cin.ignore(INT_MAX,'\n');
	std::cin>>n;
	
	if (!std::isnan(n) && n>0 && n<= data_to_ros.size()){
		for(int i=0;i<10;i++){
			message[i]=data_to_ros[n-1][i];
		}
		ROS_INFO("Writing message to Lenny using data from the \e[1mObject ID %i\e[0m ..........",n);
	}else{
		ROS_ERROR("Wrong number introduced. Writing a message for Object ID 1 as default");
		for(int i=0;i<10;i++){
			message[i]=data_to_ros[0][i];
		}
	}
	return message;
}

geometry_msgs::PoseStamped ImageConverter::find_transformation(std::vector<double> message){
	
	geometry_msgs::Pose kinect_frame_data;
	geometry_msgs::Pose transformed;
	geometry_msgs::PoseStamped transformed_pose;
	
	tf::StampedTransform transform1;
	tf::StampedTransform transform2;
	
	transformed_pose.header.stamp = ros::Time::now();
	transformed_pose.header.frame_id = "camera_rgb_optical_frame";
	
	kinect_frame_data.position.x = message[0];
	kinect_frame_data.position.y = message[1];
	kinect_frame_data.position.z = message[2];
	
	kinect_frame_data.orientation.x = message[3];
	kinect_frame_data.orientation.y = message[4];
	kinect_frame_data.orientation.z = message[5];
	kinect_frame_data.orientation.w = message[6];
	
	try {
	transform_listener_ptr->waitForTransform("kinect", "kinect_rgb_optical_frame", ros::Time::now(),ros::Duration(3.0));
	transform_listener_ptr->lookupTransform("kinect", "kinect_rgb_optical_frame", ros::Time(0),transform2);
	
	transform_listener_ptr->waitForTransform("kinect", "kinect_rgb_optical_frame", ros::Time::now(),ros::Duration(3.0));
	transform_listener_ptr->lookupTransform("torso_base_link", "kinect", ros::Time(0),transform1);
	
	tf::Transform in,out;
	tf::poseMsgToTF(kinect_frame_data,in);
	
	out=transform1*transform2*in;
	
	tf::poseTFToMsg(out,transformed);
	
	transformed_pose.pose=transformed;
	
        } catch (tf2::TransformException &ex) {
            ROS_WARN("%s", ex.what());
            ros::Duration(1.0).sleep();
        }
     return transformed_pose;
}

void ImageConverter::computer_vision(std::vector<std::vector<double> > &data_to_ros){

	
	if(!B_depth.empty() && !A_color.empty()){
	k_roll=0;
	using namespace std;

	double fx=1050;//1.0466649972613395e+03;//9.8560256011624597e+02;//mm/px
	double fy=1050;//1.0466649972613395e+03;//9.8560256011624597e+02;//mm/px
	double cx=639.5;//640.0;
	double cy=479.5;//512.0;
	
	int max_detection=8;
	
	space s;
	polycolor c;		
	
	cv::Mat image=A_color; // siempre la matriz image carga la imagen
	cv::Mat depth=B_depth;
		
	cv::Mat image2=image.clone();
	cv::Mat image3=image.clone();
	//cv::imshow("ghjk",depth);
	cv::Mat imageblurred;
	cv::Mat hsv;
    
	cv::GaussianBlur(image,imageblurred,cv::Size(31,31),0); //filtrar la imagen usando gauss
	
	cv::cvtColor(imageblurred,hsv,CV_BGR2HSV); //cambio de campo de color
    
	vector<cv::Mat> canaleshsv; //separar los tres canales de la imagen HSV
	vector<cv::Mat> canalesycrcb;
	
	vector<cv::Point> colorpoints;
	vector<cv::Point> greenpoints;
	vector<cv::Point> whitepoints;
	
	vector<cv::Point> colorpoints_depth;
	vector<cv::Point> greenpoints_depth;
	vector<cv::Point> whitepoints_depth;
	
	vector<vector<cv::Point> > data_color;
	vector<vector<cv::Point> > data_green;
	vector<vector<cv::Point> > data_white;
	
	vector<vector<double> > color_coordinates;
	vector<vector<double> > green_coordinates;
	vector<vector<double> > white_coordinates;
	
	vector<vector<double> > bottle_coordinates;
	vector<vector<double> > bottle_centers;
	
	cv::namedWindow("Colores Segmentada", CV_WINDOW_NORMAL );
	cv::namedWindow("Verde Segmentada", CV_WINDOW_NORMAL );
	cv::namedWindow("Blancos Segmentada", CV_WINDOW_NORMAL );
	cv::namedWindow("Bottles Detected in Scene", CV_WINDOW_NORMAL );
	
	cv::split(hsv,canaleshsv);
	
	
	data_color=c.findcontours(c.findcolors(c.getwhite(hsv,25,133),c.getred(canaleshsv[0],50,130),c.getgreen(canaleshsv[0],55,96),c.getblue(canaleshsv[0],95,130),c.getvalue(canaleshsv[2],90,144)),90,670,max_detection);//110 para no detectar manijas. 90 u 88 detecta tapas
	
	data_green=c.findcontours(c.findgreen(c.getgreen(canaleshsv[0],55,88),c.getwhite(hsv,25,133),c.getvalue(canaleshsv[2],72,144)),193,515,max_detection);//minimo 88 400

	data_white=c.findcontours(c.findwhite(c.getwhite(hsv,25,133),c.getvalue(canaleshsv[2],90,144)),110,705,max_detection);	
	
    
	//cv::Mat color=c.findcolors(c.getwhite(hsv,25,133),c.getred(canaleshsv[0],33,130),c.getgreen(canaleshsv[0],43,88),c.getblue(canaleshsv[0],95,130),c.getvalue(canaleshsv[2],90,144)); //Metodo para hallar botellas de color
	
	
	
	if(data_color.size()!=0){
		colorpoints=s.getpoints(data_color);
		colorpoints_depth=s.scale(colorpoints,image.cols,image.rows,depth.cols,depth.rows);
		color_coordinates=s.xyz_coord(s.find_depth(colorpoints_depth,colorpoints,depth),fx,fy,cx,cy);
		//cv::imshow("Colores Segmentada",c.drawcontors(image,data_color,255,127,80));//110 para no detectar manijas. 90 u 88 detecta tapas
		cv::imshow("Colores Segmentada",c.drawcontors(image,data_color,255,127,80));
		c.pusher(s.find_depth(colorpoints_depth,colorpoints,depth),bottle_centers);
		c.pusher(color_coordinates, bottle_coordinates);

	}
	if(data_green.size()!=0){
		greenpoints=s.getpoints(data_green);
		greenpoints_depth=s.scale(greenpoints,image.cols,image.rows,depth.cols,depth.rows);
		green_coordinates=s.xyz_coord(s.find_depth(greenpoints_depth,greenpoints,depth),fx,fy,cx,cy);
		cv::imshow("Verde Segmentada",c.drawcontors(image,data_green,0,206,209));//400
		c.pusher(s.find_depth(greenpoints_depth,greenpoints,depth),bottle_centers);
		c.pusher(green_coordinates, bottle_coordinates);

	}
	if(data_white.size()!=0){
		whitepoints=s.getpoints(data_white);
		whitepoints_depth=s.scale(whitepoints,image.cols,image.rows,depth.cols,depth.rows);
		white_coordinates=s.xyz_coord(s.find_depth(whitepoints_depth,whitepoints,depth),fx,fy,cx,cy);
		cv::imshow("Blancos Segmentada",c.drawcontors(image,data_white,255,218,185));
		c.pusher(s.find_depth(whitepoints_depth,whitepoints,depth),bottle_centers);
		c.pusher(white_coordinates,bottle_coordinates);
		
	}
	
	vector<vector<double> > points_found; //data_to_ros;
			
	points_found= s.push_data(s.getgeometrydata(color_coordinates,data_color,1,fx,fy),s.getgeometrydata(green_coordinates,data_green,2,fx,fy),s.getgeometrydata(white_coordinates,data_white,3,fx,fy));
	
	data_to_ros=points_found;
	
	//if(!bottle_coordinates.empty())
	cv::imshow("Bottles Detected in Scene",c.write_id(image,bottle_centers,bottle_coordinates));
		
	//myfile<<p[j][0]<<","<<p[j][1]<<","<<p[j][2]<<","<<p[j][3]<<","<<p[j][4]<<","<<p[j][5]<<","<<p[j][6]<<","<<p[j][7]<<","<<p[j][8]<<","<<p[j][9]<<"\n";
	//cout<<colorpoints<<depth.size()<<image.size()<<"dfghjklhgfdfghjklkjhgfdfghjkl"<<alpha<<"\n";
	//cv::imshow("Prueba",prueba);  
	//cout<<prueba;
	//cout<<"hkfgsfgk"<<depth.at<uint16_t>(250,246);*/
	//cv::waitKey(27);
	}
}
