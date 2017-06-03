#include "stdafx.h"
#include "Header.h"
#include "QRCode.h"
#include "DefineTarget.h"

QRCode FindQrCode(Mat image)
{
	// Creation of Intermediate 'Image' Objects required later
	Mat gray(image.size(), CV_MAKETYPE(image.depth(), 1));			// To hold Grayscale Image
	Mat edges(image.size(), CV_MAKETYPE(image.depth(), 1));			// To hold Grayscale Image
	Mat qr, qr_raw, qr_gray;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	int  top, right, bottom, median1, median2, outlier;
	float AB, BC, CA, dist, slope;
	int mark, A = -1, B = -1, C = -1;


	cvtColor(image, gray, CV_RGB2GRAY);		// Convert Image captured from Image Input to GrayScale	

	//cv::Canny(gray, edges, 100, 200, 3);		// Apply Canny edge detection on the gray image
	//cv::imshow("canny", edges);

	cv::findContours(edges, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE); // Find contours with hierarchy

	mark = 0;								// Reset all detected marker count for this frame

											// Get Moments for all Contours and the mass centers
	vector<Moments> mu(contours.size());
	vector<Point2f> mc(contours.size());
	int align, orientation;

	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}


	for (int i = 0; i < contours.size(); i++) {
		int k = i;
		int c = 0;

		while (hierarchy[k][2] != -1) {
			k = hierarchy[k][2];
			c = c + 1;
		}
		if (hierarchy[k][2] != -1)
			c = c + 1;

		if (c >= 5) {
			if (mark == 0)		A = i;
			else if (mark == 1)	B = i;		// i.e., A is already found, assign current contour to B
			else if (mark == 2)	C = i;		// i.e., A and B are already found, assign current contour to C
			mark = mark + 1;
		}
	}
	drawContours(image, contours, -1, Scalar(255, 200, 0), 2, 8, hierarchy, 0);

	if (mark >= 2 && (A>0 && A<mc.size())&& (B>0 && B<mc.size()) && (C>0 && C<mc.size()) ) {
		AB = cv_distance(mc[A], mc[B]);
		BC = cv_distance(mc[B], mc[C]);
		CA = cv_distance(mc[C], mc[A]);

		if (AB > BC && AB > CA) { outlier = C; median1 = A; median2 = B; }
		else if (CA > AB && CA > BC) { outlier = B;	median1 = A; median2 = C; }
		else if (BC > AB && BC > CA) { outlier = A;	median1 = B; median2 = C; }

		top = outlier;

		dist = cv_lineEquation(mc[median1], mc[median2], mc[outlier]);
		slope = cv_lineSlope(mc[median1], mc[median2], align);

		if (align == 0) { bottom = median1;	right = median2; }
		else if (slope < 0 && dist <= 0) { bottom = median1;	right = median2; orientation = NORTH; }
		else if (slope > 0 && dist < 0) { right = median1; bottom = median2; orientation = EAST; }
		else if (slope < 0 && dist > 0) { right = median1; bottom = median2; orientation = SOUTH; }
		else if (slope > 0 && dist > 0) { bottom = median1; right = median2; orientation = WEST; }

		if (top < contours.size() && right < contours.size() && bottom < contours.size() && contourArea(contours[top]) > 10 && contourArea(contours[right]) > 10 && contourArea(contours[bottom]) > 10)
		{

			vector<Point2f> L, M, O, tempL, tempM, tempO;
			Point2f N;

			vector<Point2f> src, dst;
			Mat warp_matrix;

			cv_getVertices(contours, top, slope, tempL);
			cv_getVertices(contours, right, slope, tempM);
			cv_getVertices(contours, bottom, slope, tempO);

			cv_updateCornerOr(orientation, tempL, L); 			// Re-arrange marker corners w.r.t orientation of the QR code
			cv_updateCornerOr(orientation, tempM, M); 			// Re-arrange marker corners w.r.t orientation of the QR code
			cv_updateCornerOr(orientation, tempO, O); 			// Re-arrange marker corners w.r.t orientation of the QR code

			int iflag = getIntersectionPoint(M[1], M[2], O[3], O[2], N);

			
			src.push_back(L[0]);
			src.push_back(M[1]);
			src.push_back(N);
			src.push_back(O[3]);

			Mat img2;
			image.copyTo(img2);

			Mat img3;
			Point2f pts1[] = { L[0],M[1],O[3], N };
			Point2f	pts2[4];
			if (orientation == NORTH) {
				pts2[0] = { 20,20 }; pts2[1] = { 620, 20 }; pts2[2] = { 20, 620 }; pts2[3] = { 620, 620 }; 
			}else
			if (orientation == EAST) {
				pts2[0] = { 620,20 };pts2[1] = { 620, 620 };pts2[2] = { 20, 20 };pts2[3] = { 20, 620 }; 
			}else
			if (orientation == WEST) {
				pts2[0] = { 20,620 };pts2[1] = {20, 20 };pts2[2] = { 620, 620 };pts2[3] = { 620,20 };	
			}else {
				pts2[0] = { 620,620 };pts2[1] = { 20,620 };	pts2[2] = { 620, 20 };pts2[3] = { 20, 20 };		
			}

			Mat m = getPerspectiveTransform(pts1, pts2);
			warpPerspective(img2, img3, m, Size(640, 640));

					
			QRCode qrcode = QRCode(img3);
			return qrcode;

		}
	}

	return QRCode();
}


