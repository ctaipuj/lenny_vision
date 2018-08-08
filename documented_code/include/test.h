#ifndef TEST_CLASS
#define TEST_CLASS

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>
#include<math.h>

//! A class that is used when the framework is tested.
/*!
	This class is used when the computer vision algorithm is tested in a loop mode to analize a library of images.
	*/
	
class test{
	private:
	
	public:
	//! A class constructor
	/*! Class constructor*/
	test();	
	//! Member that organize the data to be printed to a file.
	/*
		This function organize the data of the computer vision algorithm so it can be printed to a file in human-friendly style.
		\param contornos a vector<vector<cv::Point> > that stores the contours of the objects detected
		\param band a int that is used as a flag. 1 to detection analisis 2 to pose estimation analisis.
		\return a vector<double> with the data ready to be saved in a file.
		\sa get_data_to_print
		*/
	std::vector<double> get_data(int cols,int rows,std::vector<std::vector<cv::Point> > contornos,int band);
	//!Member that gathers the information of all bottle types and print the data to a file
	/*!
		This function gathers the information of all bottle types into a single data line making it easy to compare with a database.
		\param color the get_data of color bottles (colored HDPE)
		\param green the get_data of green bottles (colored PET)
		\param white the get_data of white bottles (HDPE)
		\param band a int that is used as a flag. 1 to detection analisis 2 to pose estimation analisis.
		\return a vector<double> whith the line of data that will be saved in a file
		\sa get_data
		*/ 
	std::vector<double> get_data_to_print(std::vector<double> color,std::vector<double> green, std::vector<double> white, int image,int band);

};
#endif
