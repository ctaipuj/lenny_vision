#include "../include/bridge.h"
#include "../include/perception.h"
#include "../include/detection.h"
#include<boost/assign/std/vector.hpp>

int k_roll=1;
//!< This variable controls the communication through ROS
/*!< k_roll is a int variable that manages the communication between the robot and the computer vision system through ROS. It is 1 when the system is ready 0 when the robot is executing some other task.
*/
std::ofstream myfile;
//!< This ofstream creates a file to store data to evaluate the framework
/*!< It is a file where registers are stored to analize the performance of the framework.
*/

	//tf2_ros::Buffer tfBuffer;
	//tf2_ros::TransformListener tfListener(tfBuffer);

ImageConverter::ImageConverter()
	: it_(nh_)
{
	// Subscribe to input video feed and publish output video feed
	image_sub_color = it_.subscribe("/camera/rgb/image_color", 1, &ImageConverter::kinect_color_callback, this);
	image_sub_depth = it_.subscribe("/camera/depth_registered/image", 1, &ImageConverter::kinect_depth_callback, this);
	sub = nh_.subscribe("pick_and_place/command", 30,  &ImageConverter::lenny_asks_callback,this); //pick_and_place/command
	
	pub= nh_.advertise<vision::bottle_data>("vision/bottle_data", 50);
	
	cv::namedWindow("Color Image",CV_WINDOW_NORMAL);
	myfile.open("data.txt",std::ios::app);
	if(!myfile.is_open()){
		ROS_WARN("Something went wrong with the data.txt file");
		return;
	}
	time_t now=time(0);
	char* dt=ctime(&now);
	std::string date=dt;
	//myfile<<date<<"X,Y,Z,w,x,y,z,Height,Width,Type\n";
}

ImageConverter::~ImageConverter(){

	cv::destroyWindow("Color Image");
	myfile.close();
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
	cv::waitKey(3);
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
	//cv::imshow("Depth", B_depth);
	
	if(k_roll!=0){
	ROS_INFO("Time before processing");
	double secs_before =ros::Time::now().toSec();
	computer_vision(data_to_ros);
	double secs_after=ros::Time::now().toSec();
	ROS_WARN("Elapsed time %f and the FPS processed are %f",secs_after-secs_before,1/(secs_after-secs_before));
	myfile<<"Elapsed time "<<secs_after-secs_before<<" FPS processed "<<1/(secs_after-secs_before)<<"\n";
	publisher(data_to_ros);
	}
	
}

void ImageConverter::publisher(std::vector<std::vector<double> > &data_to_ros){

	if(data_to_ros.empty()){
		ROS_ERROR("Nothing detected or no depth data available");
		return;
	}
	char key=0;
	//tf2_ros::TransformListener tfListener(tfBuffer);
	//tf2_ros::TransformListener tfListener2(tfBuffer);

	vision::bottle_data msg;
	//geometry_msgs::PoseStamped kinect_frame_data;
	geometry_msgs::Pose kinect_frame_data;
	geometry_msgs::Pose transformed;
	
	geometry_msgs::PoseStamped transformed_pose;
	//geometry_msgs::TransformStamped base_link_to_base_kinect_tf;
	tf::StampedTransform transform1;
	tf::StampedTransform transform2;
	
	std::vector<double> message(data_to_ros[0].size());
	message= selector(data_to_ros);
	
	//kinect_frame_data.header.stamp = ros::Time::now();
	//kinect_frame_data.header.frame_id = "camera_rgb_optical_frame";
	
	transformed_pose.header.stamp = ros::Time::now();
	transformed_pose.header.frame_id = "camera_rgb_optical_frame";
		
	/*kinect_frame_data.pose.position.x = message[0];
	kinect_frame_data.pose.position.y = message[1];
	kinect_frame_data.pose.position.z = message[2];
	
	kinect_frame_data.pose.orientation.x = message[3];
	kinect_frame_data.pose.orientation.y = message[4];
	kinect_frame_data.pose.orientation.z = message[5];
	kinect_frame_data.pose.orientation.w = message[6];*/
	
	kinect_frame_data.position.x = message[0];
	kinect_frame_data.position.y = message[1];
	kinect_frame_data.position.z = message[2];
	
	kinect_frame_data.orientation.x = message[3];
	kinect_frame_data.orientation.y = message[4];
	kinect_frame_data.orientation.z = message[5];
	kinect_frame_data.orientation.w = message[6];
	
	 try {
	//base_link_to_base_kinect_tf = tfBuffer.lookupTransform("base_link", "kinect", ros::Time::now(),ros::Duration(3.0));
	transform_listener_ptr->waitForTransform("kinect", "camera_rgb_optical_frame", ros::Time::now(),ros::Duration(3.0));
	transform_listener_ptr->lookupTransform("kinect", "camera_rgb_optical_frame", ros::Time(0),transform2);
	
	transform_listener_ptr->waitForTransform("kinect", "camera_rgb_optical_frame", ros::Time::now(),ros::Duration(3.0));
	transform_listener_ptr->lookupTransform("base_link", "kinect", ros::Time(0),transform1);
	
	tf::Transform in,out;
	tf::poseMsgToTF(kinect_frame_data,in);
	
	out=transform1*transform2*in;
	
	tf::poseTFToMsg(out,transformed);
	
	transformed_pose.pose=transformed;
	
            //std::cout << transformStamped << std::endl;
            //tf2::doTransform(kinect_frame_data,transformed_pose,base_link_to_base_kinect_tf);

//          pcl_ros::transformPointCloud("robotarm", cloud_in, cloud_out, tfListener);

        } catch (tf2::TransformException &ex) {
            ROS_WARN("%s", ex.what());
            ros::Duration(1.0).sleep();
        }
	msg.P=transformed_pose;
		
	msg.height=message[7];
	msg.width=message[8];
		
	msg.type=message[9];
		
		//tfb.sendTransform(msg.T);
	cv::waitKey(0);
	pub.publish(msg);
}
	
std::vector<double> ImageConverter::selector(std::vector<std::vector<double> > data_to_ros){ //hacer mas robusta la seleccion.
	
	std::vector<double> message(data_to_ros[0].size());
	
	for(int i=0;i<10;i++){
		message[i]=data_to_ros[0][i];
	}
	return message;
}

void ImageConverter::computer_vision(std::vector<std::vector<double> > &data_to_ros){

	
	if(!B_depth.empty() && !A_color.empty()){
	k_roll=0;
	using namespace std;

	double fx=1.0466649972613395e+03;//9.8560256011624597e+02;//mm/px
	double fy=1.0466649972613395e+03;//9.8560256011624597e+02;//mm/px
	double cx=640.0;
	double cy=512.0;
	double z=1.114;
	
	int max_detection=100;
	
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
	
	
	cv::namedWindow("Colores Segmentada", CV_WINDOW_NORMAL );
	cv::namedWindow("Verde Segmentada", CV_WINDOW_NORMAL );
	cv::namedWindow("Blancos Segmentada", CV_WINDOW_NORMAL );
	
	cv::split(hsv,canaleshsv);
	
	
	data_color=c.findcontours(c.findcolors(c.getwhite(hsv,25,133),c.getred(canaleshsv[0],50,130),c.getgreen(canaleshsv[0],55,96),c.getblue(canaleshsv[0],95,130),c.getvalue(canaleshsv[2],90,144)),90,670,max_detection);//110 para no detectar manijas. 90 u 88 detecta tapas
	
	data_green=c.findcontours(c.findgreen(c.getgreen(canaleshsv[0],55,88),c.getwhite(hsv,25,133),c.getvalue(canaleshsv[2],72,144)),193,515,max_detection);//minimo 88 400

	data_white=c.findcontours(c.findwhite(c.getwhite(hsv,25,133),c.getvalue(canaleshsv[2],90,144)),110,705,max_detection);	
	
    
	//cv::Mat color=c.findcolors(c.getwhite(hsv,25,133),c.getred(canaleshsv[0],33,130),c.getgreen(canaleshsv[0],43,88),c.getblue(canaleshsv[0],95,130),c.getvalue(canaleshsv[2],90,144)); //Metodo para hallar botellas de color
	
	
	
	if(data_color.size()!=0){
		colorpoints=s.getpoints(data_color);
		colorpoints_depth=s.scale(colorpoints,image.cols,image.rows,depth.cols,depth.rows);
		color_coordinates=s.xyz_coord(s.find_depth(colorpoints_depth,colorpoints,depth,data_color),fx,fy,cx,cy);
		cv::imshow("Colores Segmentada",c.drawcontors(image,data_color,255,127,80));//110 para no detectar manijas. 90 u 88 detecta tapas


	}
	if(data_green.size()!=0){
		greenpoints=s.getpoints(data_green);
		greenpoints_depth=s.scale(greenpoints,image.cols,image.rows,depth.cols,depth.rows);
		green_coordinates=s.xyz_coord(s.find_depth(greenpoints_depth,greenpoints,depth,data_green),fx,fy,cx,cy);
		cv::imshow("Verde Segmentada",c.drawcontors(image2,data_green,0,206,209));//400

	}
	if(data_white.size()!=0){
		whitepoints=s.getpoints(data_white);
		whitepoints_depth=s.scale(whitepoints,image.cols,image.rows,depth.cols,depth.rows);
		white_coordinates=s.xyz_coord(s.find_depth(whitepoints_depth,whitepoints,depth,data_white),fx,fy,cx,cy);
		cv::imshow("Blancos Segmentada",c.drawcontors(image3,data_white,255,218,185));
		
	}
	
	vector<vector<double> > p; //data_to_ros;
			
	p= s.push_data(s.getgeometrydata(color_coordinates,data_color,1,fx,fy,z),s.getgeometrydata(green_coordinates,data_green,2,fx,fy,z),s.getgeometrydata(white_coordinates,data_white,3,fx,fy,z));
	
	data_to_ros=p;
	
	for (int j=0;j<p.size();j++){
		//cout<<pepe[j][0];
		cout<<" X "<<p[j][0]<<" Y "<<p[j][1]<<" Z "<<p[j][2]<<" x "<<p[j][3]<<" y "<<p[j][4]<<" z "<<p[j][5]<<" w "<<p[j][6]<<" Height "<<p[j][7]<<" Width "<<p[j][8]<<" Type "<<p[j][9]<<"\n";
		
		//myfile<<p[j][0]<<","<<p[j][1]<<","<<p[j][2]<<","<<p[j][3]<<","<<p[j][4]<<","<<p[j][5]<<","<<p[j][6]<<","<<p[j][7]<<","<<p[j][8]<<","<<p[j][9]<<"\n";
	}
	//cout<<colorpoints<<depth.size()<<image.size()<<"dfghjklhgfdfghjklkjhgfdfghjkl"<<alpha<<"\n";
	//cv::imshow("Prueba",prueba);  
	//cout<<prueba;
	//cout<<"hkfgsfgk"<<depth.at<uint16_t>(250,246);*/
	//cv::waitKey(27);
	}
}
