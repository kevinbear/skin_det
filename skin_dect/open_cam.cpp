#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "bmp.h"
#define w 1280
#define h 720
using namespace std;
using namespace cv;
//==================================================//
unsigned char *R=NULL,*G=NULL,*B=NULL,*r=NULL,*g=NULL,*b=NULL;
float *NR = NULL, *NG = NULL, *NB = NULL;
float *V = NULL,*S=NULL,*H=NULL;
float *temp = NULL;
float comax(float *temp);
float comin(float *temp);
//================open cam===================//
int main()
{
	VideoCapture cam;
	cam.open(0);
	cam.set(CV_CAP_PROP_FRAME_WIDTH,w);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT,h);
	Mat frame(h,w,CV_8UC3);
	
	temp = (float*)malloc(sizeof(float)*3);
	//==========request input file name======//
	R = (unsigned char *)malloc(sizeof(unsigned char)*w*h);
	G = (unsigned char *)malloc(sizeof(unsigned char)*w*h);
	B = (unsigned char *)malloc(sizeof(unsigned char)*w*h);
	r = (unsigned char *)malloc(sizeof(unsigned char)*w*h);
	g = (unsigned char *)malloc(sizeof(unsigned char)*w*h);
	b = (unsigned char *)malloc(sizeof(unsigned char)*w*h);
	NR = (float *)malloc(sizeof(float)*w*h);
	NG = (float *)malloc(sizeof(float)*w*h);
	NB = (float *)malloc(sizeof(float)*w*h);
	V =  (float *)malloc(sizeof(float)*w*h);
	S = (float *)malloc(sizeof(float)*w*h);
	H = (float *)malloc(sizeof(float)*w*h);
	memset(NR, 0, sizeof(float)*w*h);
	memset(NG, 0, sizeof(float)*w*h);
	memset(NB, 0, sizeof(float)*w*h);
	memset(V, 0, sizeof(float)*w*h);
	memset(S, 0, sizeof(float)*w*h);
	memset(H, 0, sizeof(float)*w*h);
	//======================================/
	while(1)
	{
		cam>>frame;
		for(int i=0;i<h*w;i++)
		{
			B[i]=frame.data[3*i];
			G[i]=frame.data[3*i+1];
			R[i]=frame.data[3*i+2];
		}

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				float delta,mmin,fr=0,fg=0,fb=0;
				//normalized
				//printf("R=%d G=%d B=%d\n",R[i][j],G[i][j],B[i][j]);
				fr=(float)R[i*w+j],fg=(float)G[i*w+j],fb=(float)B[i*w+j];
				temp[0] = NR[i*w + j] = fr/ 255 ;
				temp[1] = NG[i*w + j] = fg/ 255 ;
				temp[2] = NB[i*w + j] = fb/ 255 ;
				//printf("R=%d G=%d B=%d\n",R[i][j],G[i][j],B[i][j]);
				//printf("temp[0]=%f temp[1]=%f temp[2]=%f\n", temp[0], temp[1], temp[2]);
				//calculate H S V
				V[i*w+ j] = comax(temp); //V
				mmin = comin(temp);
				delta = V[i*w + j] - mmin;
				S[i*w + j] = delta / V[i*w + j]; //S
				//getc(stdin);
				if (NR[i*w + j] == V[i*w + j])
				{
					H[i*w + j] = (1.0 / 6.0)*((NG[i*w + j] - NB[i*w + j]) / delta); //H
				}
				else if (NG[i*w + j] == V[i*w + j])
				{
					H[i*w + j] = (1.0 / 6.0)*(2 + ((NB[i*w + j] - NR[i*w + j]) / delta));
				}
				else if (NB[i*w + j] == V[i*w + j])
				{
					H[i*w + j] = (1.0 / 6.0)*(4 + ((NR[i*w + j] - NG[i*w + j]) / delta));
				}
				//if (H[i*w + j] < 0)
					//H[i*w+ j] += 1;
				if ((6.0/360.0) <= H[i*w + j] && H[i*w + j] <= (50.0/360.0) )
				{
					if (0.23 <= S[i*w + j] && S[i*w + j] <= 0.68)
					{
						//printf("ind=%d\n",i*w+j);
						//printf("111111111111\n");
						r[i*w+j] = 255;
						g[i*w+j] = 255;
						b[i*w+j] = 255;
					}
					else
					{
						r[i*w+j] = R[i*w+j];
						g[i*w+j] = G[i*w+j];
						b[i*w+j] = B[i*w+j];
					}
				}
				else
				{
					r[i*w+j] = R[i*w+j];
					g[i*w+j] = G[i*w+j];
					b[i*w+j]= B[i*w+j];
				}
			}
		}
		for(int i=0;i<w*h;i++)
		{
			frame.data[3*i]=b[i];
			frame.data[3*i+1]=g[i];
			frame.data[3*i+2]=r[i];
		}
		//getc(stdin);
		imshow("skin_det",frame);
		if(cvWaitKey(10)>=0) break;
	}
	return 0;
}

float comax( float *temp)
{
	float Max = temp[0];
	for (int i = 1; i < 3; i++)
	{
		if (Max <= temp[i] )
		{
			Max = temp[i];
		}
	}
	return Max;
}
float comin(float *temp)
{
	float Min = temp[0];
	for (int i = 1; i < 3; i++)
	{
		if (Min >= temp[i])
		{
			Min = temp[i];
		}
	}
	return Min;
}

