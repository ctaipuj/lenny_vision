#ifndef POLY_COLOR
#define POLY_COLOR

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<math.h>
#include<string>

class polycolor{

	private:

	public:
	
	polycolor();
	
	cv::Mat getwhite(cv::Mat hsv,int s,int v);

	cv::Mat getred(cv::Mat h,int r1,int r2);

	cv::Mat getblue(cv::Mat h,int a1,int a2);

	cv::Mat getgreen(cv::Mat h,int g1,int g2);

	cv::Mat getsaturation(cv::Mat s,int s0,int s1,int s2);

	cv::Mat getvalue(cv::Mat v,int v1,int v2);

	cv::Mat findcolors(cv::Mat maskwhite,cv::Mat maskred,cv::Mat maskgreen,cv::Mat maskblue,cv::Mat maskvalue);
	
	cv::Mat findgreen(cv::Mat maskgreen,cv::Mat maskwhite,cv::Mat maskvalue);

	cv::Mat findwhite(cv::Mat maskwhite,cv::Mat maskvalue);

	std::vector<std::vector<cv::Point> > findcontours(cv::Mat proc,int cmin,int cmax, int max_detection);
	
	cv::Mat drawcontors(cv::Mat image,std::vector<std::vector<cv::Point> > contornos,int r,int g,int b);

	cv::Mat write_id(cv::Mat image,std::vector<std::vector<double> > contornos,std::vector<std::vector<double> > data);
		
	void clean_contours(cv::Mat image,std::vector<std::vector<cv::Point> > * contornos);
	
	//std::vector<std::vector<cv::Point> > push_contours(std::vector<std::vector<cv::Point> > color, std::vector<std::vector<cv::Point> > green, std::vector<std::vector<cv::Point> > white);
	
	void pusher(std::vector<std::vector<double> > A, std::vector<std::vector<double> > &push_data);
};

#endif
