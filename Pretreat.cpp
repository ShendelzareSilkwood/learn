#pragma once
#include "Pretreat.h"


Pretreat::Pretreat()
{
	result = new CorData();
	result->image_count = 0;
	result->imagesize = Size(0, 0);
}

Pretreat::~Pretreat()
{
	delete result;
}


CorData* Pretreat::cornor_calculate(QStringList filename_list)
{
	result->image_count = 0;							
	Size board_size = Size(11, 8);
	vector<Point2f> image_points_buf;
	int count = 0;
	for (QString filename : filename_list)
	{
		result->image_count++;
		string filename1 = filename.toStdString();
		Mat imageInput = imread(filename1);
		if (result->image_count == 1)
		{
			result->imagesize.width = imageInput.cols;
			result->imagesize.height = imageInput.rows;
		}
		Mat view_gray;
		cvtColor(imageInput, view_gray, CV_BGR2GRAY); 

		//int ret = findChessboardCorners(view_gray, board_size, image_points_buf, CALIB_CB_FILTER_QUADS);
		bool ret = findChessboardCornersSB(view_gray, board_size, image_points_buf, CALIB_CB_EXHAUSTIVE | CALIB_CB_ACCURACY);
		if (0 == ret)
		{
			return NULL;
		}
		else
		{
			cornerSubPix(view_gray, image_points_buf, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			count += image_points_buf.size();
			result->image_points_seq.push_back(image_points_buf);
			drawChessboardCorners(imageInput, board_size, image_points_buf,ret);
			result->image.push_back(imageInput);
		}
	}

	Size square_size = Size(30, 30);

	int i, j, t;
	for (t = 0; t < result->image_count; t++)
	{
		vector<Point3f> tempPointSet;
		for (i = board_size.height - 1; i >= 0 ; i--)
		{
			for (j = board_size.width - 1; j >= 0 ; j--)
			{
				Point3f realPoint;
				/* ����궨�������������ϵ��z=0��ƽ���� */
				//realPoint.x = i * square_size.width;
				//realPoint.y = j * square_size.height;
				realPoint.x = j * square_size.width;
				realPoint.y = i * square_size.height;
				realPoint.z = 0; 
				tempPointSet.push_back(realPoint);
			}
		}
		result->object_points.push_back(tempPointSet);
	}
	return result;
}

CorData* Pretreat::cornor_calculate1(QStringList filename_list)
{
	result->image_count = 0;							
	Size board_size = Size(13, 10);					
	vector<Point2f> image_points_buf;				
	int count = 0;									
	for (QString filename : filename_list)
	{
		result->image_count++;
		string filename1 = filename.toStdString();
		Mat imageInput = imread(filename1); 
		if (result->image_count == 1) {
			result->imagesize.width = imageInput.cols;
			result->imagesize.height = imageInput.rows;
		}
		Mat view_gray;
		cvtColor(imageInput, view_gray, CV_BGR2GRAY);

		//int ret = findChessboardCorners(view_gray, board_size, image_points_buf, CALIB_CB_FILTER_QUADS);
		bool ret = findChessboardCornersSB(view_gray, board_size, image_points_buf, CALIB_CB_EXHAUSTIVE | CALIB_CB_ACCURACY);
		if (0 == ret)
		{
			return NULL;
		}
		else
		{
			cornerSubPix(view_gray, image_points_buf, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
			count += image_points_buf.size();
			result->image_points_seq.push_back(image_points_buf);//���������ؽǵ�
			drawChessboardCorners(imageInput, board_size, image_points_buf, ret);
			result->image.push_back(imageInput);
		}
	}

	Size square_size = Size(50, 50);				

	//��ʼ���궨���Ͻǵ����ά����
	int i, j, t;
	for (t = 0; t < result->image_count; t++)
	{
		vector<Point3f> tempPointSet;
		for (i = board_size.height - 1; i >= 0; i--)
		{
			for (j = board_size.width - 1; j >= 0; j--)
			{
				Point3f realPoint;
				/* ����궨�������������ϵ��z=0��ƽ���� */
				//realPoint.x = i * square_size.width;
				//realPoint.y = j * square_size.height;
				realPoint.x = j * square_size.width;
				realPoint.y = i * square_size.height;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		result->object_points.push_back(tempPointSet);
	}
	return result;
}