#pragma once
#include<opencv\cv.h>  
#include<opencv\highgui.h>

#include "windows.h"
#include <string>
#include <cstring>
#include <tchar.h>
#include <iostream>

namespace imgc{
	//-------------------------my function-----------------------
	union Union_mint
	{
		int a;
		char b[4];
	};


	typedef struct iplimg_head
	{
		Union_mint height, width, depth, step, channel;
		/*height = img->height;
		widht = img->width;
		step = img->widthStep;
		channel = img->nChannels;*/
	} hdr;

	char* union_int2char(Union_mint in)
	{
		char* out = (char*)malloc(sizeof(hdr));
		int i;
		for (i = 0; i < 4; i++)
		{
			out[i] = in.b[i];
		}
		return out;
	}

	char* hdr2Char(hdr* hd)
	{
		char* out = (char*)malloc(sizeof(hdr));
		int i;
		for (i = 0; i < 4; i++)
		{
			out[i] = union_int2char(hd->height)[i];
		}
		for (i = 0; i < 4; i++)
		{
			out[i + 4] = union_int2char(hd->width)[i];
		}
		for (i = 0; i < 4; i++)
		{
			out[i + 8] = union_int2char(hd->depth)[i];
		}
		for (i = 0; i < 4; i++)
		{
			out[i + 12] = union_int2char(hd->step)[i];
		}
		for (i = 0; i < 4; i++)
		{
			out[i + 16] = union_int2char(hd->channel)[i];
		}

		return out;

	}

	void iplimg2har(hdr *hd, IplImage *img)
	{
		hd->height.a = img->height;
		hd->width.a = img->width;
		hd->depth.a = img->depth;
		hd->step.a = img->widthStep;
		hd->channel.a = img->nChannels;
	}



	IplImage* setImageHeadData(hdr *hd)
	{
		return cvCreateImageHeader(cvSize(hd->width.a, hd->height.a), hd->depth.a, hd->channel.a);
	}


	IplImage* getImgFromByte(char* arr, int len)
	{
		int i;
		int hdLen = sizeof(hdr);

		char *hdrbuf = (char *)malloc(hdLen);
		for (i = 0; i < hdLen; i++)
		{
			hdrbuf[i] = arr[i];
		}
		hdr*hd = (hdr*)hdrbuf;
		int dataLen = hd->step.a*hd->height.a;
		//char *imgdatabuf = (char *)malloc(dataLen);
		//imgdatabuf = &arr[hdLen];   
		char *imgdatabuf = &arr[hdLen];//竟然可以这样	
		//imgdatabuf = &arr[hdLen];   //竟然可以这样
		/*for (i = 0; i < dataLen; i++)
		{
		imgdatabuf[i] = arr[i + hdLen];
		}*/

		IplImage *image_dst = setImageHeadData(hd);
		cvSetData(image_dst, imgdatabuf, hd->step.a);
		free(hd);
		//free(hdrbuf);//!!!!重复释放
		//free(imgdatabuf);

		return image_dst;
	}

	//记得释放返回的指针
	char* image2byte(IplImage* image_src)
	{
		//IplImage *image_src = cvLoadImage("hello.jpg");//载入图片  
		int i;
		int lhd = sizeof(hdr);
		hdr* hd = (hdr*)malloc(lhd);
		iplimg2har(hd, image_src);
		char *hdrbuf = &hdr2Char(hd)[0];

		int ldata = hd->step.a*hd->height.a;  //imagedata的长度
		int len = ldata + lhd;
		char *sendData = (char*)malloc(ldata + lhd);
		memset(sendData, 0, ldata);
		for (i = 0; i < lhd; i++)
		{
			sendData[i] = hdrbuf[i];
		}
		free(hd);
		free(hdrbuf);
		for (i = 0; i < ldata; i++)
		{
			sendData[i + lhd] = image_src->imageData[i];
		}
		return sendData;
	}


	void testimag()
	{
		IplImage *image_src = cvLoadImage("hello.jpg");//载入图片  
		IplImage *image_dst = cvCreateImage(cvSize(image_src->height, image_src->width), 8, 1);//放置灰度图像  
		//-------------to char-------s
		hdr* hd = (hdr*)malloc(sizeof(hdr));
		iplimg2har(hd, image_src);

		int i;

		int ldata = hd->step.a*hd->height.a;  //imagedata的长度
		//char sendData[len] = "";
		char *sendData = (char*)malloc(ldata);
		memset(sendData, 0, ldata);
		//cvNamedWindow("client", 1);
		/*cvShowImage("dst1", image_src);
		waitKey(20);*/
		//cvCvtColor(image_src, image_dst, CV_RGB2GRAY);
		cvShowImage("src232", image_src);

		//hdr* hd = (hdr *)malloc(sizeof(hdr));
		int l = sizeof(hdr);
		char *hdrbuf = (char *)malloc(sizeof(hdr));
		for (i = 0; i < l; i++)
		{
			hdrbuf[i] = hdr2Char(hd)[i];
		}

		//--------to char------------e

		//memcpy(buf, &hd, sizeof(hdr));	

		for (i = 0; i < ldata; i++)
		{
			sendData[i] = image_src->imageData[i];
		}
		hdr*dsthd = (hdr*)hdrbuf;
		//从 IplImage到 char* ：
		//char* data = (char*)image_src->imageData; //对齐的图像数据  
		//或者data = image->imageDataOrigin //未对齐的原始图像数据  
		//从 char* 到 IplImage：
		image_dst = setImageHeadData(dsthd);
		cvSetData(image_dst, sendData, dsthd->step.a);
		cvShowImage("dst232", image_dst);
		cv::waitKey(20);
	}
	//----------------------my function end----------------------------------------------
}