/*
Submitted by: Greeshma Sasidharan Nair
Project Name: 2D Animation
File name:  assignment1.cpp
OpenCV version: 2.4.10 
Microsoft Visual Studio 2010 
purpose: To generate animation for the dinosaur and girl and generate a moving background effect.
*/


#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#define DINO_YCO 260
#define DINO_XCO 200
#define GIRL_YCO 300
#define GIRL_XCO 600

signed int x_adj=0,y_adj=0;

void loadImages(Mat g_img[],Mat girl_mask[],Mat d_img[],Mat d_mask[]);
char doAnimation(Mat g_img[],Mat girl_mask[],Mat d_img[],Mat d_mask[],Mat bg_scene,Mat screen_img);

int main()
{
	Mat g_img[15],girl_mask[15],d_img[15],d_mask[15];
	loadImages(g_img,girl_mask,d_img,d_mask);	
	Mat bg_scene=imread("imgs/bg_scene.jpg");
	if (bg_scene.empty()) 
	{
		cout << "Cannot load background image!" << endl;
		exit (1);
	}
	Mat screen_img;
	screen_img.create(bg_scene.rows,bg_scene.cols-5,CV_8UC(3));
	screen_img=Scalar(0,0,0);
	char key=doAnimation(g_img,girl_mask,d_img,d_mask,bg_scene,screen_img);
	while(1){
		if(key==27)  //Esc
			break;
		switch(key){	
			case 'd':	x_adj=x_adj+10;
						key=doAnimation(g_img,girl_mask,d_img,d_mask,bg_scene,screen_img);				
						break;
			case 'a':	x_adj=x_adj-10;
						key=doAnimation(g_img,girl_mask,d_img,d_mask,bg_scene,screen_img);					
						break;
			case 'w':	y_adj=y_adj-10;
						key=doAnimation(g_img,girl_mask,d_img,d_mask,bg_scene,screen_img);					
						break;
			case 's':	y_adj=y_adj+10;
						key=doAnimation(g_img,girl_mask,d_img,d_mask,bg_scene,screen_img);
						break;
		}

	}
	return 0;
}

/*
To do the animation for girl,Dinosaur and to generate moving effect for background
Arguments: g_img[],girl_mask[],d_img[],d_mask[],bg_scene,screen_img
Return : char, which holds the user key input
*/

char doAnimation(Mat g_img[],Mat girl_mask[],Mat d_img[],Mat d_mask[],Mat bg_scene,Mat screen_img){
	char key;
	static int prevYValue,prevXValue;
	if(DINO_YCO+y_adj==0||DINO_YCO+y_adj==560)		//check for the boundary limit, both x and y for the keyboard events
		prevYValue=y_adj;	
	if(DINO_YCO+y_adj<0 || DINO_YCO+y_adj>560)
		y_adj=prevYValue;
	if(DINO_XCO+x_adj==0 || GIRL_XCO+x_adj==1200)
		prevXValue=x_adj;	
	if(DINO_XCO+x_adj<0 || GIRL_XCO+x_adj>1200)	
		x_adj=prevXValue;
	int col=0;
	while(1){
		bool flag=0;
		for(int index=0;index<13;index++){
			Mat bg_crop = bg_scene(Rect(col,0,bg_scene.cols/2,bg_scene.rows)); // for sliding window
			resize(bg_crop, bg_crop, Size(screen_img.cols,screen_img.rows));
			bg_crop.copyTo(screen_img);
			col=col+5;
			if(col>bg_scene.cols-col)
				col=0;  
			d_img[index].copyTo(screen_img(Range(DINO_YCO+y_adj,d_img[index].rows+DINO_YCO+y_adj), Range(DINO_XCO+x_adj,d_img[index].cols
				+DINO_XCO+x_adj)),d_mask[index]);
			g_img[index].copyTo(screen_img(Range(GIRL_YCO+y_adj,g_img[index].rows+GIRL_YCO+y_adj), Range(GIRL_XCO+x_adj,g_img[index].cols+GIRL_XCO
				+x_adj)),girl_mask[index]);				
			imshow("Display Animation", screen_img);			
			key = waitKey(60); 
			if (key == 27 || key=='d' || key=='a' || key=='w' || key=='s'){ //"Esc" 27-ASCII 
				flag=1;	  
				break;
			}					
		}
		if(flag)
			break;
	}	
	return key;
}

/*
To load images from the harddisk to memory
Arguments: g_img[],girl_mask[],d_img[],d_mask[]
Return : null
*/

void loadImages(Mat g_img[],Mat girl_mask[],Mat d_img[],Mat d_mask[]){
	char name[50];
	int count=00;
	while(1){
		sprintf(name,"imgs/girl%02d.jpg",count);
		g_img[count]=imread(name);
		if (g_img[count].empty()) 
		{
			cout << "Cannot load girl image!" << endl;
			break;
		}
		sprintf(name,"imgs/girl_mask_%02d.bmp",count);
		girl_mask[count]=imread(name,0);
		if (girl_mask[count].empty()) 
		{
			cout << "Cannot load girl mask!" << endl;
			break;
		}
		sprintf(name,"imgs/dinosaur%02d.jpg",count);
		d_img[count]=imread(name);
		if (d_img[count].empty()) 
		{
			cout << "Cannot load dinosaur image!" << endl;
			break;
		}
		sprintf(name,"imgs/dinosaur_mask_%02d.bmp",count);
		d_mask[count]=imread(name,0);	
		if (d_mask[count].empty()) 
		{
			cout << "Cannot load dinosaur mask!" << endl;
			break;
		}
		count++;		
	}
}