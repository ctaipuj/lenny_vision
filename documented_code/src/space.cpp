#include"../include/perception.h"

using namespace std;

space::space(){}

vector<cv::Point> space::getpoints(vector<vector<cv::Point> > contornos){

	vector<cv::Point> centroides(contornos.size());
	vector<vector<cv::Point> >::iterator itc= contornos.begin();
	int it=0;
	
	while(itc!=contornos.end()){ //mientras no llegue al final del vector de los contornos
		cv::Moments mom=cv::moments(cv::Mat(*itc++)); //calcule todos los momentos del contorno
		//if(mom.m00!=0)
		centroides[it]=cv::Point(mom.m10/mom.m00,mom.m01/mom.m00);
		it++;
	}
	return centroides;
}

vector<cv::Point> space::scale(vector<cv::Point> Points,int  originalcols, int originalrows, int tinycols, int tinyrows){

	vector<cv::Point> scaled(Points.size());
	double alpha= 1.0*tinycols/originalcols;
	double beta= 1.0*tinyrows/originalrows;
	 
	for (int i=0;i<Points.size();i++){
		scaled[i].x=round(Points[i].x*alpha);
		scaled[i].y=round(Points[i].y*beta);
	}
	return scaled;
}

vector<vector<double> > space::find_depth(vector<cv::Point> Points_scaled, vector<cv::Point> Points_real, cv::Mat depth){
	
	vector<vector<double> > uvz(Points_real.size(),vector<double>(3)); //,
	
	for (int i =0;i<Points_real.size();i++){
		if(!isnan(depth.at<float>(Points_scaled[i]))){//|| depth.at<float>(Points_scaled[i])!=0){
			uvz[i][0]=Points_real[i].x;
			uvz[i][1]=Points_real[i].y;
			uvz[i][2]=depth.at<float>(Points_scaled[i]);
		}
	}
	return uvz;
}

vector<vector<double> > space::xyz_coord(vector<vector<double> > Points_xyz, double fx, double fy, double cx, double cy){

	vector<vector<double> > kinect_coord(Points_xyz.size(),vector<double>(3));
	
	for (int i=0;i<Points_xyz.size();i++){
		kinect_coord[i][0]= (Points_xyz[i][2]*((Points_xyz[i][0]-cx)/fx));//dividir en 10 para obtener cm 
		kinect_coord[i][1]= (Points_xyz[i][2]*((Points_xyz[i][1]-cy)/fy));
		kinect_coord[i][2]= Points_xyz[i][2];
	}
	return kinect_coord;
}

vector<vector<double> > space::getgeometrydata(vector<vector<double> > kinect_xyz, vector<vector<cv::Point> > contornos, int bottle_type, double fx,double fy,double z){//OJO AL Z A VER SI ES NECESARIO ENVIAR EL DATO.

	vector<vector<double> > tf_coord(kinect_xyz.size(),vector<double>(10));
	
	if(!kinect_xyz.empty()){

		cv::RotatedRect r;
		double pi=3.14159265359;
		double angle_rad;
		vector<double> q(4);
	
		for(int i=0;i<kinect_xyz.size();i++){
			r=cv::fitEllipse(contornos[i]);
		
			tf_coord[i][0]=kinect_xyz[i][0];//+0.045;//translacion data
			tf_coord[i][1]=kinect_xyz[i][1];//+0.028;
			tf_coord[i][2]=kinect_xyz[i][2];//+0.0025;
		
			if(r.angle<=90)
				angle_rad=(r.angle)*(pi/180);
				else
				angle_rad=(r.angle-180)*(pi/180);
		
			q=getquaternion(0,0,angle_rad);
		
			tf_coord[i][3]=q[0]; //rotation data x
			tf_coord[i][4]=q[1]; // y
			tf_coord[i][5]=q[2]; //z
			tf_coord[i][6]=q[3]; // w

			tf_coord[i][7]=kinect_xyz[i][2]*r.size.height/((fx+fy)/2);
			tf_coord[i][8]=kinect_xyz[i][2]*r.size.width/((fx+fy)/2);
			tf_coord[i][9]=bottle_type;
		}
	
	}
	return tf_coord;
}

vector<double> space::getquaternion(double roll, double pitch, double yaw){

	vector<double> q(4);
	
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);

	q[0] = cy * sr * cp - sy * cr * sp; //x
	q[1] = cy * cr * sp + sy * sr * cp; //y
	q[2] = sy * cr * cp - cy * sr * sp; //z
	q[3] = cy * cr * cp + sy * sr * sp; //w
	
	return q;
}

vector<vector<double> > space::push_data(vector<vector<double> > color, vector<vector<double> > green, vector<vector<double> > white){

	vector<vector<double> > data_to_ros;
	
	if(!color.empty())
		pusher(color, data_to_ros);
	
	if(!green.empty())
		pusher(green, data_to_ros);
	
	if(!white.empty())
		pusher(white, data_to_ros);
	
	return data_to_ros;
}		
		
void space::pusher(vector<vector<double> > &A, vector<vector<double> > &push_data){
	
	for(int i =0;i<A.size();i++){
	if(A[i][0]==-0 && A[i][1]==-0 || A[i][2]==0);
	else
		push_data.push_back(A[i]);	
	}
}
