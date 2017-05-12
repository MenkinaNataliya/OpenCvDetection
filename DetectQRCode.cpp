#include "stdafx.h"
#include "Header.h"

bool FindQrCode(Mat image)
{
	// Creation of Intermediate 'Image' Objects required later
	Mat gray(image.size(), CV_MAKETYPE(image.depth(), 1));			// To hold Grayscale Image
	Mat edges(image.size(), CV_MAKETYPE(image.depth(), 1));			// To hold Grayscale Image
	Mat qr, qr_raw, qr_gray, qr_thres;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	int mark, A = -1, B = -1, C = -1;

		qr_thres = Mat::zeros(100, 100, CV_8UC1);

		cvtColor(image, gray, CV_RGB2GRAY);		// Convert Image captured from Image Input to GrayScale	

		Canny(gray, edges, 100, 200, 3);		// Apply Canny edge detection on the gray image
		imshow("canny", edges);


		findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Find contours with hierarchy

		mark = 0;								// Reset all detected marker count for this frame

												// Get Moments for all Contours and the mass centers
		vector<Moments> mu(contours.size());
		vector<Point2f> mc(contours.size());

		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i], false);
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}

		for (int i = 0; i < contours.size(); i++)
		{
			int k = i;
			int c = 0;

			while (hierarchy[k][2] != -1)
			{
				k = hierarchy[k][2];
				c = c + 1;
			}
			if (hierarchy[k][2] != -1)
				c = c + 1;

			if (c >= 5)
			{
				if (mark == 0)		A = i;
				else if (mark == 1)	B = i;		// i.e., A is already found, assign current contour to B
				else if (mark == 2)	C = i;		// i.e., A and B are already found, assign current contour to C
				mark = mark + 1;
			}
		}


		if (mark >= 2 && A>=0&& B>=0&&C>=0)		// Ensure we have (atleast 3; namely A,B,C) 'Alignment Markers' discovered
		{
							// Now that we have the orientation of the line formed median1 & median2 and we also have the position of the outlier w.r.t. the line
			if (C < contours.size() && A < contours.size() && B < contours.size() && contourArea(contours[C]) > 10 && contourArea(contours[A]) > 10 && contourArea(contours[B]) > 10)
			{
				drawContours(image, contours, C, Scalar(255, 200, 0), 2, 8, hierarchy, 0);
				drawContours(image, contours, A, Scalar(0, 0, 255), 2, 8, hierarchy, 0);
				drawContours(image, contours, B, Scalar(255, 0, 100), 2, 8, hierarchy, 0);

			}														// Determine the 'right' and 'bottom' markers
			imshow("Image", image);
			imshow("QR code", qr_thres);
			return true;

		}

		return false;
}


