#include "../include/test.h"

using namespace std;

test::test(){}

vector<double> test::get_data(int cols,int rows,vector<vector<cv::Point> > contornos,int band){
	
	vector<double> data;
	
	switch (band){
	
		case 1:
			data.resize(7);
	
			if(!contornos.empty()){

				cv::RotatedRect r;
				int n1=0;
				int n2=0;
				int n3=0;
				int n4=0;
				int n5=0;
				int n6=0;
		
				for(int i=0;i<contornos.size();i++){
					r=cv::fitEllipse(contornos[i]);
					cv::Moments mom=cv::moments(contornos[i]); //calcule todos los momentos del contorno
					cv::Point centroide;
					centroide.x=mom.m10/mom.m00;
					centroide.y=mom.m01/mom.m00;
			
					if(centroide.x<cols/2 && centroide.y<rows/2) //clasificacion por segmentos de las botellas.
						n1=n1+1;
					if(centroide.x>cols/2 && centroide.y<rows/2)
						n2=n2+1;
					if(centroide.x<cols/2 && centroide.y>rows/2)
						n3=n3+1;
					if(centroide.x>cols/2 && centroide.y>rows/2)
						n4=n4+1;
					
					if(r.angle<=90){
						n5=n5+1;
					}
					else{
						n6=n6+1;
					}
				}
				
				data[0]=n1+n2+n3+n4;//botellas en la imagen y en los segmentos 
				data[1]=n1;
				data[2]=n2;
				data[3]=n3;
				data[4]=n4;
				data[5]=n5;
				data[6]=n6;
	
			}
			break;
		
		case 2:
		
			data.resize(7);
	
			if(!contornos.empty()){

				cv::RotatedRect r;
				double angle_rad;
		
				for(int i=0;i<contornos.size();i++){
					r=cv::fitEllipse(contornos[i]);
					cv::Moments mom=cv::moments(contornos[i]); //calcule todos los momentos del contorno
					cv::Point centroide;
					centroide.x=mom.m10/mom.m00;
					centroide.y=mom.m01/mom.m00;
			
					if(centroide.x<cols/2 && centroide.y<rows/2)
						data[0]=1;
					if(centroide.x>cols/2 && centroide.y<rows/2)
						data[1]=1;
					if(centroide.x<cols/2 && centroide.y>rows/2)
						data[2]=1;
					if(centroide.x>cols/2 && centroide.y>rows/2)
						data[3]=1;
				
					if(r.angle<=90){
						angle_rad=(r.angle); //Rotacion hacia derecha
						data[5]=1;
					}
					else{
						angle_rad=(r.angle-180); //Rotacion hacia izquierda
						data[6]=1;
					}
					data[4]=angle_rad;

					//data[5]=bottle_type;
				}
	
			}
			break;
		}
	return data;
}

vector<double>  test::get_data_to_print(vector<double> color,vector<double> green, vector<double> white, int image,int band){

	vector<double> data_to_print;

	switch(band){
	
	case 1:

	data_to_print.resize(27);
	
	data_to_print[0]=image;
	
	data_to_print[1]=color[0]+green[0]+white[0]; //Total de objetos en la imagen

	data_to_print[2]=color[1]+green[1]+white[1];//Evaluar por segmento totales
	data_to_print[3]=color[2]+green[2]+white[2];
	data_to_print[4]=color[3]+green[3]+white[3];
	data_to_print[5]=color[4]+green[4]+white[4];
	
	data_to_print[6]=color[0]; //Total de botellas por tipo 
	data_to_print[7]=green[0];
	data_to_print[8]=white[0];
		
	data_to_print[9]=color[1]; //Del tipo a en el segmento n
	data_to_print[10]=color[2];
	data_to_print[11]=color[3];
	data_to_print[12]=color[4];
	
	data_to_print[13]=green[1]; //Del tipo b en el segmento n
	data_to_print[14]=green[2];
	data_to_print[15]=green[3];
	data_to_print[16]=green[4];
	
	data_to_print[17]=white[1]; //Del tipo c en el segmento n
	data_to_print[18]=white[2];
	data_to_print[19]=white[3];
	data_to_print[20]=white[4];
	
	data_to_print[21]=color[5]; //rotacion a la derecha del tipo a
	data_to_print[22]=color[6]; //rotacion a la izquierda del tipo a
	
	data_to_print[23]=green[5]; //rotacion a la derecha del tipo b
	data_to_print[24]=green[6]; //rotacion a la izquierda del tipo b
	
	data_to_print[25]=white[5]; //rotacion a la derecha del tipo c
	data_to_print[26]=white[6]; //rotacion a la izquierda del tipo c
	break;
	
	case 2:
	
	data_to_print.resize(22);
	
	data_to_print[0]=image;
		
	data_to_print[1]=color[0]; //Del tipo a en el segmento n
	data_to_print[2]=color[1];
	data_to_print[3]=color[2];
	data_to_print[4]=color[3];
	
	data_to_print[5]=green[0]; //Del tipo b en el segmento n
	data_to_print[6]=green[1];
	data_to_print[7]=green[2];
	data_to_print[8]=green[3];
	
	data_to_print[9]=white[0]; //Del tipo c en el segmento n
	data_to_print[10]=white[1];
	data_to_print[11]=white[2];
	data_to_print[12]=white[3];
	
	data_to_print[13]=color[5]; //rotacion a la derecha del tipo a
	data_to_print[14]=color[6]; //rotacion a la izquierda del tipo a
	
	data_to_print[15]=green[5]; //rotacion a la derecha del tipo b
	data_to_print[16]=green[6]; //rotacion a la izquierda del tipo b
	
	data_to_print[17]=white[5]; //rotacion a la derecha del tipo c
	data_to_print[18]=white[6]; //rotacion a la izquierda del tipo c
	
	data_to_print[19]=color[4];
	data_to_print[20]=green[4];
	data_to_print[21]=white[4];
	break;
	}
	return data_to_print;
}
