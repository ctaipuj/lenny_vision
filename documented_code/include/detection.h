#ifndef POLY_COLOR
#define POLY_COLOR

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<math.h>

//!A class dedicated to the image processing based in openCV
/*
	This class allows user to detect different objects based on color, saturation and hue using some OpenCV functions
*/
class polycolor{

	private:

	public:
	//!Class constructor
	polycolor();
	//!Member that gets white detections based on a HSV image
	/*!
		This function finds white detections based on saturation and value levels
		\param hsv as a cv::Mat. The image in HSV color space.
		\param s max saturation
		\param v min value
		\return Binary cv::Mat image of detected pixels
	*/
	cv::Mat getwhite(cv::Mat hsv,int s,int v);
	//!Member that gets red detections based on a one channel image
	/*!
		This function finds red detections based on one-channel image (hue)
		\param h as a cv::Mat. One-channel image. Hue channel
		\param r1 min hue level
		\param r2 max hue level
		\return Binary cv::Mat image of detected pixels
	*/
	cv::Mat getred(cv::Mat h,int r1,int r2);
	//!Member that gets blue detections based on a one channel image
	/*!
		This function finds red detections based on one-channel image (hue)
		\param h as a cv::Mat. One-channel image. Hue channel
		\param a1 min hue level
		\param a2 max hue level
		\return Binary cv::Mat image of detected pixels
	*/
	cv::Mat getblue(cv::Mat h,int a1,int a2);
	//!Member that gets green detections based on a one channel image
	/*!
		This function finds green detections based on one-channel image (hue)
		\param h as a cv::Mat. One-channel image. Hue channel
		\param g1 min hue level
		\param g2 max hue level
		\return Binary cv::Mat image of detected pixels
	*/
	cv::Mat getgreen(cv::Mat h,int g1,int g2);
	//!Member that gets saturation level detections based on a one channel image
	/*!
		This function finds saturation detections based on one-channel image (saturation)
		\param s as a cv::Mat. One-channel image. Saturation channel
		\param s0 min saturation level
		\param s1 max saturation level
		\param s2 min saturation level to establish other limit (s0=s2 if s2 is not necessary)
		\return Binary cv::Mat image of detected pixels
	*/
	cv::Mat getsaturation(cv::Mat s,int s0,int s1,int s2);
	//!Member that gets value level detections based on a one channel image
	/*!
		This function finds value detections based on one-channel image (value)
		\param v as a cv::Mat. One-channel image. Value channel
		\param v1 min saturation level
		\param v2 max saturation level
		\return Binary cv::Mat image of detected pixels
	*/
	cv::Mat getvalue(cv::Mat v,int v1,int v2);
	//!Member that finds color bottles except green ones and white.
	/*!
		This function finds color bottles based on different masks for each subtype. The cv::Mat arguments are all binary images.
		\param maskwhite as a cv::Mat white detections
		\param maskred as a cv::Mat red detections
		\param maskgreen as a cv::Mat green detections
		\param maskblue as a cv::Mat blue detections
		\param maskvalue as a cv:Mat acceptable value levels
		\return Binary cv::Mat image of HDPE-colored detected bottles
		\sa getvalue, getwhite, getred, getgreen, getblue
	*/
	cv::Mat findcolors(cv::Mat maskwhite,cv::Mat maskred,cv::Mat maskgreen,cv::Mat maskblue,cv::Mat maskvalue);
	//!Member that finds green bottles.
	/*!
		This function finds green bottles based on different masks for each subtype. The cv::Mat arguments are all binary images.
		\param maskgreen as a cv::Mat green detections
		\param maskwhite as a cv::Mat white detections
		\param maskvalue as a cv:Mat acceptable value levels
		\return Binary cv::Mat image of PET-colored detected bottles
		\sa getgreen, getvalue
	*/	
	cv::Mat findgreen(cv::Mat maskgreen,cv::Mat maskwhite,cv::Mat maskvalue);
	//!Member that finds white bottles.
	/*!
		This function finds white bottles based on different masks for each subtype. The cv::Mat arguments are all binary images.
		\param maskwhite as a cv::Mat white detections
		\param maskvalue as a cv:Mat acceptable value levels
		\return Binary cv::Mat image of HDPE detected bottles
		\sa getwhite, getvalue
	*/
	cv::Mat findwhite(cv::Mat maskwhite,cv::Mat maskvalue);
	//!Member that finds contours of bottles
	/*!
		This function finds the real contours of the detected objects.
		\param proc as a cv::Mat with the detected objects
		\param cmin min contours size that is acceptable
		\param cmax max contours size that is acceptable
		\param max_detection max number of contours allowed
		\return A vector of contours detected
		\sa findcolors, findwhite, findgreen, findcolors
	*/
	std::vector<std::vector<cv::Point> > findcontours(cv::Mat proc,int cmin,int cmax, int max_detection);
	//!Member that draws fitted rectangles to contours of bottles
	/*!
		This function draws fitted rectangles and ellipses in each contour.
		\param contornos as a vector<vector<cv::Point> > that stores the contour data
		\param r red intensity of the line to draw (255-r)
		\param g red intensity of the line to draw
		\param b red intensity of the line to draw (255-b)
		\return a cv::Mat RGB image whith the fitted object draw to the original image.
		\sa findcontours
	*/
	cv::Mat drawcontors(cv::Mat image,std::vector<std::vector<cv::Point> > contornos,int r,int g,int b);
};

#endif
