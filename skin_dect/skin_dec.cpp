#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bmp.h"
using namespace std;
char *inname=NULL, *outname=NULL;
//================rgb_array region===================//
int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];

int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];
//==================================================//
float *NR = NULL, *NG = NULL, *NB = NULL;
float *V = NULL,*S=NULL,*H=NULL;
float *temp = NULL;
float comax(float *temp);
float comin(float *temp);
//================open cam===================//
int main()
{
	temp = (float*)malloc(sizeof(float)*3);
	int width, height;
	system("ls input_image/");//list all bmp file 
	inname = (char *)malloc(sizeof(char) * 100);
	outname = (char *)malloc(sizeof(char) * 100);
	//==========request input file name======//
	puts("=====================================================");
	printf("Please input you want choose image:");
	scanf("%s", inname);
	string nametemp, file = "./input_image/", bmp(inname);
	nametemp = file + bmp;
	const char *bmpname = nametemp.c_str();
	open_bmp((char*)bmpname, R, G, B, width, height);
	NR = (float *)malloc(sizeof(float)*width*height);
	NG = (float *)malloc(sizeof(float)*width*height);
	NB = (float *)malloc(sizeof(float)*width*height);
	V =  (float *)malloc(sizeof(float)*width*height);
	S = (float *)malloc(sizeof(float)*width*height);
	H = (float *)malloc(sizeof(float)*width*height);
	memset(NR, 0, sizeof(float)*width*height);
	memset(NG, 0, sizeof(float)*width*height);
	memset(NB, 0, sizeof(float)*width*height);
	memset(V, 0, sizeof(float)*width*height);
	memset(S, 0, sizeof(float)*width*height);
	memset(H, 0, sizeof(float)*width*height);
	//======================================/
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float delta,mmin,fr=0,fg=0,fb=0;
			//normalized
			//printf("R=%d G=%d B=%d\n",R[i][j],G[i][j],B[i][j]);
			fr=(float)R[i][j],fg=(float)G[i][j],fb=(float)B[i][j];
			temp[0] = NR[i*width + j] = fr/ 255 ;
			temp[1] = NG[i*width + j] = fg/ 255 ;
			temp[2] = NB[i*width + j] = fb/ 255 ;
			//printf("R=%d G=%d B=%d\n",R[i][j],G[i][j],B[i][j]);
			//printf("temp[0]=%f temp[1]=%f temp[2]=%f\n", temp[0], temp[1], temp[2]);
			//calculate H S V
			V[i*width + j] = comax(temp); //V
			mmin = comin(temp);
			delta = V[i*width + j] - mmin;
			S[i*width + j] = delta / V[i*width + j]; //S
			//getc(stdin);
			if (NR[i*width + j] == V[i*width + j])
			{
				H[i*width + j] = (1.0 / 6.0)*((NG[i*width + j] - NB[i*width + j]) / delta); //H
			}
			else if (NG[i*width + j] == V[i*width + j])
			{
				H[i*width + j] = (1.0 / 6.0)*(2 + ((NB[i*width + j] - NR[i*width + j]) / delta));
			}
			else if (NB[i*width + j] == V[i*width + j])
			{
				H[i*width + j] = (1.0 / 6.0)*(4 + ((NR[i*width + j] - NG[i*width + j]) / delta));
			}
			//printf("V=%f S=%f\n",V[i*width + j],S[i*width + j]);
			//if (H[i*width + j] < 0)
				//H[i*width + j] += 1;
			//printf("V=%f S=%f H=%f\n",V[i*width + j],S[i*width + j],H[i*width+j]);
			//getc(stdin);
			//H[i*width + j] *= 360; //H
			//skin HSV range
			//printf("H[%d]=%f\n", i*width + j, H[i*width + j]);
			if ((6.0/360.0) <= H[i*width + j] && H[i*width + j] <= (38.0/360.0) )
			{
				if (0.23 <= S[i*width + j] && S[i*width + j] <= 0.68)
				{
					//printf("111111111111\n");
					r[i][j] = 0;
					g[i][j] = 255;
					b[i][j] = 0;
				}
				else
				{
					r[i][j] = 255;
					g[i][j] = 0;
					b[i][j] = 0;
				}
			}
			else
			{
				r[i][j] = 255;
				g[i][j] = 0;
				b[i][j] = 0;
			}
		}
		//getc(stdin);
	}
	//==========request input file name======//
	printf("File new name:");
	scanf("%s", outname);
	string outbmp(outname), outfile = "./output_image/";
	nametemp = outfile + outbmp;
	const char *outbmpname = nametemp.c_str();
	save_bmp((char*)outbmpname, r, g, b);
	//======================================//
	printf("Job Finished!\n");
	puts("=====================================================");
	close_bmp();
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
