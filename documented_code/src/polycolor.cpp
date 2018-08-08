#include"../include/detection.h"

using namespace std;

polycolor::polycolor(){}

cv::Mat polycolor::getwhite(cv::Mat hsv,int s,int v){

	cv::Mat blancos=cv::Mat::zeros(cv::Size(hsv.size()),CV_8U);
	cv::MatIterator_<cv::Vec3b> it=hsv.begin<cv::Vec3b> ();
	cv::MatIterator_<cv::Vec3b> itend=hsv.end<cv::Vec3b> ();
	cv::MatIterator_<uchar> itb=blancos.begin<uchar> ();
    
	while(it!=itend){
		if((*it)[1]<s && (*it)[2]>v){
			*itb=255;
			++it;
			++itb;
		}
		else{
			++it;
			++itb;
		}
	}
	return blancos;
}

cv::Mat polycolor::getred(cv::Mat h,int r1,int r2){

	cv::Mat maskred;
	cv::Mat lut(1,256,CV_8U); //rojo

	for (int i=0;i<256;i++){
		if(i<r1 || i>r2){
			lut.at<uchar>(i)=255;
		}else{
			lut.at<uchar>(i)=0;
		}
	}
	
	cv::LUT(h,lut,maskred);
	return maskred;
}

cv::Mat polycolor::getblue(cv::Mat h,int a1,int a2){

	cv::Mat maskblue;
	cv::Mat lut(1,256,CV_8U); //azules
	for (int i=0;i<256;i++){
		if(i>a1 && i<a2){
			lut.at<uchar>(i)=255;
		}else{
			lut.at<uchar>(i)=0;
		}
	}
	
	cv::LUT(h,lut,maskblue);
	return maskblue;
}

cv::Mat polycolor::getgreen(cv::Mat h,int g1,int g2){
	
	cv::Mat maskgreen;
	cv::Mat lut(1,256,CV_8U); //Verde
	for (int i=0;i<256;i++){
		if(i>g1 && i<g2){
			lut.at<uchar>(i)=255;
		}else{
			lut.at<uchar>(i)=0;
		}
	}
	
	cv::LUT(h,lut,maskgreen);
	return maskgreen;
}

cv::Mat polycolor::getsaturation(cv::Mat s,int s0,int s1,int s2){

	cv::Mat masksaturation;
	cv::Mat lut(1,256,CV_8U); //saturation
	
	for (int i=0;i<256;i++){
		if(i>s0 && i<s1 || i>s2){//90){//140
			lut.at<uchar>(i)=255;
		}else{
			lut.at<uchar>(i)=0;
		}
	}
	
	cv::LUT(s,lut,masksaturation);
	return masksaturation;
}

cv::Mat polycolor::getvalue(cv::Mat v,int v1,int v2){

	cv::Mat maskvalue;
	cv::Mat lut(1,256,CV_8U); //Value
	
	for (int i=0;i<256;i++){
		if(i>v1 && i<v2){//144){//235){//90){//140
			lut.at<uchar>(i)=255;
		}else{
			lut.at<uchar>(i)=0;
		}
	}
	
	cv::LUT(v,lut,maskvalue);
	return maskvalue;
}

cv::Mat polycolor::findcolors(cv::Mat maskwhite,cv::Mat maskred,cv::Mat maskgreen,cv::Mat maskblue,cv::Mat maskvalue){

	cv::Mat segmented; 
	cv::Mat no_white_color;
	cv::Mat colors;
	cv::Mat value_colors;
	
	cv::dilate(maskwhite,maskwhite,cv::Mat(),cv::Point(-1,-1),6);	
	cv::dilate(maskgreen,maskgreen,cv::Mat(),cv::Point(-1,-1),4);
	
	cv::bitwise_not(maskwhite,maskwhite);
	cv::bitwise_not(maskgreen,maskgreen);
		
	cv::bitwise_or(maskred,maskblue,colors);
	
	cv::bitwise_and(maskvalue,colors,value_colors);
	cv::bitwise_and(value_colors,maskwhite,no_white_color);   
	cv::bitwise_and(no_white_color,maskgreen,segmented);
	
	return segmented;
}

cv::Mat polycolor::findgreen(cv::Mat maskgreen,cv::Mat maskwhite,cv::Mat maskvalue){

	cv::Mat green_bottles;
	cv::Mat real_green;
	
	cv::dilate(maskwhite,maskwhite,cv::Mat(),cv::Point(-1,-1),6);	
	//cv::dilate(maskgreen,maskgreen,cv::Mat(),cv::Point(-1,-1),4);
	
	cv::bitwise_not(maskwhite,maskwhite);
	cv::bitwise_and(maskgreen,maskwhite,real_green);
	//cv::imshow("G1",maskgreen);
	//cv::imshow("G2",real_green);
	
	cv::bitwise_and(real_green,maskvalue,green_bottles);
	
	return green_bottles;
}

cv::Mat polycolor::findwhite(cv::Mat maskwhite,cv::Mat maskvalue){
	
	cv::Mat white;

	cv::dilate(maskwhite,maskwhite,cv::Mat(),cv::Point(-1,-1),6);
	cv::bitwise_and(maskwhite,maskvalue,white);
	
	return white;
}

vector<vector<cv::Point> > polycolor::findcontours(cv::Mat proc,int cmin,int cmax,int max_detection){

	cv::Mat edges;
	cv::RotatedRect r;
	
	vector<vector<cv::Point> > contornos(max_detection);//Vector de vectores donde se almacenan los contornos  
	
	cv::morphologyEx(proc,edges,cv::MORPH_GRADIENT,cv::Mat()); //usar operacion gradiente para hallar bordes*/
 	cv::findContours(edges,contornos,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE); //OJO MODIFICADO CV_CHAIN_APPROX_NONE //encontrar contornos, solo externos, sin jerarquias
 	vector<vector<cv::Point> >::iterator itc= contornos.begin();//Iterador para los n contornos hallados
	
	while (itc!=contornos.end()) { //mientras hayan contornos
		
		if (itc->size() < cmin|| itc->size() > cmax)//eliminar los menores o los mayores
			itc= contornos.erase(itc); //Eliminar contornos que no se ajusten a las medidas correspondientes
		else
			++itc;//si se ajusta evalue el siguiente	
	}
	
	for (int i=0;i<contornos.size();i++){
		cv::Moments mom=cv::moments(contornos[i]);
		if(contornos[i].size()<=10 || mom.m00==0){
			contornos.erase(contornos.begin()+i);
		}
		else{
			r=cv::fitEllipse(contornos[i]);
			if(r.size.width<50||r.size.width>230||r.size.height<68)//210
				contornos.erase(contornos.begin()+i);
		}
	}
	return contornos;
}

cv::Mat polycolor::drawcontors(cv::Mat image,vector<vector<cv::Point> > contornos,int r,int g,int b){

	vector<cv::Rect> rectangulos(contornos.size()); //Vector de rectangulos
	vector<cv::RotatedRect> ellipse(contornos.size());//Vector de elipses	
	vector<vector<cv::Point> >::iterator itc= contornos.begin();//Iterador para los n contornos hallados
	
	for(int i=0;i<contornos.size();i++){ //for que dibuja rectangulos
		ellipse[i]=cv::fitEllipse(contornos[i]);
		rectangulos[i]=cv::boundingRect(contornos[i]); //alamcenar en la posicion i del rectangulo el rectangulo mas pequeño que encierre el contorno
		//cout<<rectangulos[i]<<"\n"; //Imprima los rectangulos solo para probar
		cv::rectangle(image,rectangulos[i],cv::Scalar(b,g,r),1); //dibuje rectangulos en la imagen original
		cv::ellipse(image,ellipse[i],cv::Scalar(b,g,r),1);
		/*cv::Point2f rect_points[4]; ellipse[i].points( rect_points );
		for( int j = 0; j < 4; j++ ){
			line(image, rect_points[j], rect_points[(j+1)%4], cv::Scalar(b,g,r), 1, 8 );
		}*/    
		cv::Moments mom=cv::moments(contornos[i]); //calcule todos los momentos del contorno
		cv::circle(image,cv::Point(mom.m10/mom.m00,mom.m01/mom.m00),2,cv::Scalar(255-b,0+g,255-r),4); //dibuje un circulo en el centro de masa x=m10/m00 y y=m01/m00
		//cout<<cv::Point(mom.m10/mom.m00,mom.m01/mom.m00)<<"\n"; // Imprima los centros de masa solo para probar
	}
	return image;
}
