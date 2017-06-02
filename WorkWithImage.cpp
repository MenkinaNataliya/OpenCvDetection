#include "stdafx.h"
#include "Header.h"

void Contrast(Mat m_imgEdit, int step)
{
	try
	{
		std::vector<Mat> rgb;
		cv::split(m_imgEdit, rgb);
		Mat lut(1, 256, CV_8UC1);
		double contrastLevel = double(100 + step) / 100;
		uchar* p = lut.data;
		double d;
		for (int i = 0; i < 256; i++)
		{
			d = ((double(i) / 255 - 0.5)*contrastLevel + 0.5) * 255;
			if (d > 255)
				d = 255;
			if (d < 0)
				d = 0;
			p[i] = d;
		}
		if(rgb.size()==1)
			LUT(rgb[0], lut, rgb[0]);
		else if(rgb.size()==2)
		{
			LUT(rgb[0], lut, rgb[0]);
			LUT(rgb[1], lut, rgb[1]);
		}
		else if(rgb.size()==3)
		{
			LUT(rgb[0], lut, rgb[0]);
			LUT(rgb[1], lut, rgb[1]);
			LUT(rgb[2], lut, rgb[2]);
		}
		
		cv::merge(rgb, m_imgEdit);
		
		//return m_imgEdit;
	}
	catch (Exception ex)
	{
		
	}
}

Mat Clarity(Mat *m_imgEdit, int step)
{
	try
	{
		if (step < 0)
		{
			cv::blur(*m_imgEdit, *m_imgEdit, cv::Size(-step * 2 + 1, -step * 2 + 1));
		}
		else
		{
			Mat dst = m_imgEdit->clone();
			float matr[9]{
				-0.0375 - 0.05*step, -0.0375 - 0.05*step, -0.0375 - 0.05*step,
				-0.0375 - 0.05*step, 1.3 + 0.4*step, -0.0375 - 0.05*step,
				-0.0375 - 0.05*step, -0.0375 - 0.05*step, -0.0375 - 0.05*step
			};
			Mat kernel_matrix = Mat(3, 3, CV_32FC1, &matr);
			cv::filter2D(*m_imgEdit, dst, 32, kernel_matrix);
			//return m_imgEdit;
			//m_imgEdit = make_shared<Mat>(dst);
		}
		//cv::imwrite("D:/Учеба/Сервисный робот/OpenCvDetection/data/tmp1.jpg", *m_imgEdit);
		return *m_imgEdit;
	}
	catch (Exception ex)
	{

	}
}
