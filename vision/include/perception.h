#ifndef SPATIAL_COORD
#define SPATIAL_COORD

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<math.h>

class space{
	
	private:
	
	public:
	
	space();
	
	std::vector<cv::Point> getpoints(std::vector<std::vector<cv::Point> > contornos);
	
	std::vector<cv::Point> scale(std::vector<cv::Point> Points,int  originalcols, int originalrows, int tinycols, int tinyrows);
	
	std::vector<std::vector<double> > find_depth(std::vector<cv::Point> Points_scaled,std::vector<cv::Point> Points_real,cv::Mat depth);
	
	std::vector<std::vector<double> > xyz_coord(std::vector<std::vector<double> > Points, double fx, double fy, double cx, double cy);
	
	std::vector<std::vector<double> > getgeometrydata(std::vector<std::vector<double> > kinect_xyz, std::vector<std::vector<cv::Point> > contornos, int bottle_type,double fx, double fy);
	
	std::vector<double> getquaternion(double roll, double pitch, double yaw);
	
	std::vector<std::vector<double> > push_data(std::vector<std::vector<double> > color,std::vector<std::vector<double> > green, std::vector<std::vector<double> > white);
		
	void pusher(std::vector<std::vector<double> > &A, std::vector<std::vector<double> > &data_to_ros);
	
};
#endif
