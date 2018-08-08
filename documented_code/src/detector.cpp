#include "../include/perception.h"
#include "../include/detection.h"

using namespace std;
//parameros intrincecos de la camara
//! int fx focal length in x-axis expressed in pixels
/*! Camera matrix's focal length in x-axis expressed in pixels
*/
 double fx=1.0466649972613395e+03;//9.8560256011624597e+02;//mm/px
//! int fy focal lenght in y-axis expressed in pixels
/*! Camera matrix's focal length in y-axis expressed in pixels
*/
 double fy=1.0466649972613395e+03;//9.8560256011624597e+02;//mm/px
//! int cx Point in x-axis of the origin of the image
/*! Camera matrix's Point in x-axis of the origin of the image
*/
 double cx=640.0;
//! int cy Point in y-axis of the origin of the image
/*! Camera matrix's Point in y-axis of the origin of the image
*/
 double cy=512.0;
 
//!This function executes the computer vision code when it is used stand-alone mode
/*!
	This function executes the computer vision code when it is used stand-alone mode
*/
	 
int main(int argc,char** argv){

	if ( argc != 2 ){
		cout<<"<Image_Path> not defined\n"; //Incluir los argumentos al momento de abrir la pagina
		return -1;
	}
    
	polycolor c; // crear objeto de la clase polycolor
	space s; //crear un objeto de la clase space
    
	cv::Mat image=cv::imread(argv[1]); // siempre la matriz image carga la imagen
	cv::Mat depth;	
	depth=cv::imread("../depth_picture_84.tiff",CV_16UC1);
	cv::Mat image2=image.clone();
	cv::Mat image3=image.clone();
	cv::imshow("ghjk",depth);
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
	
	cv::namedWindow("Colores Segmentada", CV_WINDOW_NORMAL );
	cv::namedWindow("Verde Segmentada", CV_WINDOW_NORMAL );
	cv::namedWindow("Blancos Segmentada", CV_WINDOW_NORMAL );
	
	cv::split(hsv,canaleshsv);
	
	
	data_color=c.findcontours(c.findcolors(c.getwhite(hsv,25,133),c.getred(canaleshsv[0],50,130),c.getgreen(canaleshsv[0],55,96),c.getblue(canaleshsv[0],95,130),c.getvalue(canaleshsv[2],90,144)),90,670);//110 para no detectar manijas. 90 u 88 detecta tapas
	
	data_green=c.findcontours(c.findgreen(c.getgreen(canaleshsv[0],55,88),c.getwhite(hsv,25,133),c.getvalue(canaleshsv[2],72,144)),193,515);//minimo 88 400

	data_white=c.findcontours(c.findwhite(c.getwhite(hsv,25,133),c.getvalue(canaleshsv[2],90,144)),110,705);	
	
    
	//cv::Mat color=c.findcolors(c.getwhite(hsv,25,133),c.getred(canaleshsv[0],33,130),c.getgreen(canaleshsv[0],43,88),c.getblue(canaleshsv[0],95,130),c.getvalue(canaleshsv[2],90,144)); //Metodo para hallar botellas de color
	
	cv::imshow("Colores Segmentada",c.drawcontors(image,data_color,255,127,80));//110 para no detectar manijas. 90 u 88 detecta tapas
	cv::imshow("Verde Segmentada",c.drawcontors(image2,data_green,0,206,209));//400
	cv::imshow("Blancos Segmentada",c.drawcontors(image3,data_white,255,218,185));
	
	if(data_color.size()!=0){
		colorpoints=s.getpoints(data_color);
		colorpoints_depth=s.scale(colorpoints,image.cols,image.rows,depth.cols,depth.rows);
		cv::imshow("Colores Segmentada",c.drawcontors(image,data_color,255,127,80));//110 para no detectar manijas. 90 u 88 detecta tapas
	
	}
	if(data_green.size()!=0){
		greenpoints=s.getpoints(data_green);
		greenpoints_depth=s.scale(greenpoints,image.cols,image.rows,depth.cols,depth.rows);
		cv::imshow("Verde Segmentada",c.drawcontors(image2,data_green,0,206,209));//400

	}
	if(data_white.size()!=0){
		whitepoints=s.getpoints(data_white);
		whitepoints_depth=s.scale(whitepoints,image.cols,image.rows,depth.cols,depth.rows);
		cv::imshow("Blancos Segmentada",c.drawcontors(image3,data_white,255,218,185));
	}


	//cout<<colorpoints_depth;//_depth;
	
	
	//cout<<colorpoints<<endl;
	vector<vector<double> > pepe;
	pepe=s.find_depth(whitepoints_depth,whitepoints,depth);
	
	vector<vector<double> > maria;
	maria=s.xyz_coord(pepe,fx,fy,cx,cy);
	
			vector<vector<double> > p;
		p=s.getgeometrydata(maria,data_color);
	
	for (int j=0;j<p.size();j++){
		//cout<<pepe[j][0];
		cout<<" X "<<p[j][0]<<" Y "<<p[j][1]<<" Z "<<p[j][2]<<" w "<<p[j][3]<<" x "<<p[j][4]<<" y "<<p[j][5]<<" z "<<p[j][6]<<" Height "<<p[j][7]<<" Width "<<p[j][8]<<" Type "<<p[j][9]<<"\n";}
	//cout<<colorpoints<<depth.size()<<image.size()<<"dfghjklhgfdfghjklkjhgfdfghjkl"<<alpha<<"\n";
	//cv::imshow("Prueba",prueba);  
	//cout<<prueba;
	//cout<<"hkfgsfgk"<<depth.at<uint16_t>(250,246);*/
    cv::waitKey(0);		
		
    return 0;    
}
