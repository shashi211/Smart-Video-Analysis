#include <opencv2/opencv.hpp>
#include <vector>
using namespace cv;
using namespace std;
int winx1 = 90, winy1 = 20, winx2 = 800, winy2 = 20, winx3 = 90, winy3 = 420, winx4 = 800, winy4 = 420, height = 650, width = 360;

void edgeDetect(float x1, float y1, float x2, float y2, int *le, int *re)
{
	float mx, x, temp;
	int i;
	if ((y2 - y1)<0)
	{
		temp = y1; y1 = y2; y2 = temp;
		temp = x1; x1 = x2; x2 = temp;
	}
	if ((y2 - y1) != 0)
		mx = (x2 - x1) / (y2 - y1);
	else
		mx = x2 - x1;
	x = x1;
	for (i = y1; i <= y2; i++)
	{
		if (x<(float)le[i])
			le[i] = (int)x;
		if (x>(float)re[i])
			re[i] = (int)x;
		x += mx;
	}
}

void HumanDetection()
{
	int a, b;
	float x1 = 50, y1 = 50, x2 = 640, y2 = 50, x3 = 640, y3 = 450, x4 = 300, y4 = 450, x5 = 300, y5 = 300, x6 = 50, y6 = 300;
	float X1 = 0, Y1 = 50, X2 = 350, Y2 = 50, X3 = 350, Y3 = 250, X4 = 500, Y4 = 250, X5 = 500, Y5 = 450, X6 = 0, Y6 = 450;

	VideoCapture cap1("Input 1a.mp4");
	VideoCapture cap2("Input 1b.mp4");

	cap1.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap1.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	cap2.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	cap2.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	VideoWriter video1("Output1.mp4", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(640, 480));
	VideoWriter video2("Output2.mp4", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(640, 480));

	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	namedWindow("-Background-");
	cvMoveWindow("-Background-", 0, -50);
	cvvResizeWindow("-Background-", 2400, 1000);
	Mat imgB = imread("PS1.jpg");
	imshow("-Background-", imgB);
	namedWindow("Vid1");
	namedWindow("Vid2");
	cvMoveWindow("Vid1", winx1 + 20, 150);
	cvvResizeWindow("Vid1", height, width);
	cvMoveWindow("Vid2", winx2 - 20, 150);
	cvvResizeWindow("Vid2", height, width);
	int le1[640], re1[640], le2[640], re2[640];
	int n, y;
	for (n = 0; n < 640; n++)
	{
		le1[n] = 640;
		re1[n] = 0;
	}
	for (n = 0; n < 640; n++)
	{
		le2[n] = 640;
		re2[n] = 0;
	}
	edgeDetect(x1, y1, x2, y2, le1, re1);
	edgeDetect(x2, y2, x3, y3, le1, re1);
	edgeDetect(x3, y3, x4, y4, le1, re1);
	edgeDetect(x4, y4, x5, y5, le1, re1);
	edgeDetect(x5, y5, x6, y6, le1, re1);
	edgeDetect(x6, y6, x1, y1, le1, re1);

	edgeDetect(X1, Y1, X2, Y2, le2, re2);
	edgeDetect(X2, Y2, X3, Y3, le2, re2);
	edgeDetect(X3, Y3, X4, Y4, le2, re2);
	edgeDetect(X4, Y4, X5, Y5, le2, re2);
	edgeDetect(X5, Y5, X1, Y1, le2, re2);
	edgeDetect(X5, Y5, X1, Y1, le2, re2);

	while (1)
	{
		Mat img1, img2;
		cap1 >> img1;
		cap2 >> img2;
		video1.write(img1);
		video2.write(img2);
		if (img1.empty() || img2.empty())
			break;

		vector<Rect> found1, found2, found_filtered1, found_filtered2, dups;
		hog.detectMultiScale(img1, found1, 0, Size(8, 8), Size(32, 32), 1.2, 2);
		hog.detectMultiScale(img2, found2, 0, Size(8, 8), Size(32, 32), 1.2, 2);
		size_t i, j;

		Scalar color = Scalar(0, 255, 0);

		line(img1, Point(x1, y1), Point(x2, y2), color, 2, 8, 0);
		line(img1, Point(x2, y2), Point(x3, y3), color, 2, 8, 0);
		line(img1, Point(x3, y3), Point(x4, y4), color, 2, 8, 0);
		line(img1, Point(x4, y4), Point(x5, y5), color, 2, 8, 0);
		line(img1, Point(x5, y5), Point(x6, y6), color, 2, 8, 0);
		line(img1, Point(x6, y6), Point(x1, y1), color, 2, 8, 0);

		line(img2, Point(X1, Y1), Point(X2, Y2), color, 2, 8, 0);
		line(img2, Point(X2, Y2), Point(X3, Y3), color, 2, 8, 0);
		line(img2, Point(X3, Y3), Point(X4, Y4), color, 2, 8, 0);
		line(img2, Point(X4, Y4), Point(X5, Y5), color, 2, 8, 0);
		line(img2, Point(X5, Y5), Point(X6, Y6), color, 2, 8, 0);
		line(img2, Point(X6, Y6), Point(X1, Y1), color, 2, 8, 0);

		Rect border1(500, 50, 140, 400);
		rectangle(img1, border1, Scalar(0, 0, 255), 2);
		Rect border2(0, 50, 140, 400);
		rectangle(img2, border2, Scalar(0, 0, 255), 2);

		for (i = 0; i < found1.size(); i++)
		{
			Rect r = found1[i];
			for (j = 0; j < found1.size(); j++)
				if (j != i && (r & found1[j]) == r)
					break;
			if (j == found1.size())
				found_filtered1.push_back(r);
		}
		for (i = 0; i < found_filtered1.size(); i++)
		{
			Rect r = found_filtered1[i];
			a = r.x;
			b = r.y;
			if (le1[b]<re1[b] && a > le1[b] && a < re1[b])
			{
				r.x += cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.06);
				r.height = cvRound(r.height*0.9);
				rectangle(img1, r.tl(), r.br(), cv::Scalar(255, 0, 0), 2);
			}
		}

		for (i = 0; i < found2.size(); i++)
		{
			Rect r = found2[i];
			for (j = 0; j < found2.size(); j++)
				if (j != i && (r & found2[j]) == r)
					break;
			if (j == found2.size())
				found_filtered2.push_back(r);
		}
		for (i = 0; i < found_filtered2.size(); i++)
		{
			Rect r = found_filtered2[i];
			a = r.x;
			b = r.y;
			if (le2[b]<re2[b] && a > (le2[b] + 140) && a < re2[b])
			{
				r.x += cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.06);
				r.height = cvRound(r.height*0.9);
				rectangle(img2, r.tl(), r.br(), cv::Scalar(255, 0, 0), 2);
			}
		}
		imshow("Vid1", img1);
		imshow("Vid2", img2);
		char ch = waitKey(20);
		if (ch == 27)
			break;
	}
	destroyWindow("Vid1");
	destroyWindow("Vid2");
	destroyWindow("-Background-");
}

void ObjectDetection()
{
	Mat frame, back, backz, fore, sub, imageT;
	int posX = -1, posY = -1;

	VideoCapture cap1("video1.avi");
	Ptr<BackgroundSubtractorMOG2> bg = createBackgroundSubtractorMOG2();
	bg->setNMixtures(10);
	vector < vector < Point > >contours;

	namedWindow("-Background-");
	cvMoveWindow("-Background-", 0, -50);
	cvvResizeWindow("-Background-", 2400, 1000);
	Mat imgB = imread("PS2.jpg");
	imshow("-Background-", imgB);
	namedWindow("Rectangle");
	namedWindow("Abandoned Objects");
	cvMoveWindow("Rectangle", winx1, 150);
	cvvResizeWindow("Rectangle", height, width);
	cvMoveWindow("Abandoned Objects", winx2, 150);
	cvvResizeWindow("Abandoned Objects", height, width);
	int flag = 0;
	double t = getTickCount() / getTickFrequency();//for abandoned object
	while (1)
	{
		cap1 >> frame;
		if (frame.empty())
			break;
		bg->apply(frame, fore);
		bg->getBackgroundImage(back);
		findContours(fore, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

		Mat out = frame.clone();

		int interval;
		if (flag == 0)
		{
			absdiff(back, back, backz);
			interval = 60;
			flag = 10;
		}

		if (flag == 10 && (getTickCount() / getTickFrequency() - t) >= 2)//this loop runs once
		{
			backz = back.clone();//extra loop run to ensure stable initial background
			flag = 20;
		}

		if ((getTickCount() / getTickFrequency() - t) >= interval)//interval can vary from 0 to 10 minutes; infinite loop
		{
			backz = back.clone();//not backz=back as they'll become pointers pointing to same address
			t = getTickCount() / (getTickFrequency());
		}
		if ((getTickCount() / getTickFrequency() - t) >= interval - 3)//interval can vary from 0 to 10 minutes; infinite loop
		{
			cout << "\a";//to produce a sound near new interval
		}
		absdiff(back, backz, sub);
		threshold(sub, sub, 35, 255, THRESH_BINARY);
		inRange(sub, Scalar(254, 254, 254), Scalar(255, 255, 255), imageT);
		Moments oMoments = moments(imageT);

		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;
		if (dArea > 100)
		{
			posX = dM10 / dArea;
			posY = dM01 / dArea;
		}
		Rect border(posX - sqrt(dArea)*0.05, posY - sqrt(dArea)*0.05, sqrt(dArea)*0.1, sqrt(dArea)*0.1);
		if (cv::waitKey(3) >= 10)
			break;

		Mat drawing = Mat::zeros(fore.size(), CV_8UC3);
		drawing = out.clone();
		Scalar color = Scalar(0, 255, 0);
		vector<Rect> boundRect(contours.size());

		for (int i = 0; i < contours.size(); i++)
		{
			boundRect[i] = boundingRect(contours[i]);
			//if (boundRect[i].width >= 70 || boundRect[i].height >= 70)
			//rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 0);	//For Human Detection
		}
		rectangle(imageT, border, Scalar(255, 0, 0), 2);
		rectangle(drawing, border, Scalar(255, 0, 0), 2);
		imshow("Abandoned Objects", sub);
		imshow("Rectangle", drawing);
		char c = (char)waitKey(15);
		if (c == 27)
			break;
	}
	destroyWindow("Abandoned Objects");
	destroyWindow("Rectangle");
	destroyWindow("-Background-");
}

void MaskDetection()
{
	CascadeClassifier face_cascade;
	VideoCapture cap(0);
	cap.set(CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CAP_PROP_FRAME_HEIGHT, 480);
	Mat img, imgT;
	face_cascade.load("cascade/cascade.xml");

	namedWindow("-Background-");
	cvMoveWindow("-Background-", 0, -50);
	cvvResizeWindow("-Background-", 2400, 1000);
	Mat imgB = imread("PS3.jpg");
	imshow("-Background-", imgB);
	namedWindow("Mask");
	cvMoveWindow("Mask", 400, 180);
	cvvResizeWindow("Mask", 640, 480);

	while (1)
	{
		cap >> img;
		if (img.empty())
			break;
		inRange(img, Scalar(0, 0, 0), Scalar(10, 10, 10), imgT);
		Moments oMoments = moments(imgT);
		double dM01 = oMoments.m01;
		double dM10 = oMoments.m10;
		double dArea = oMoments.m00;
		if (dArea > 30000000)
		{
			Rect border(20, 20, 600, 440);
			rectangle(img, border, Scalar(0, 255, 0), 2);
		}
		vector<Rect> faces;
		face_cascade.detectMultiScale(img, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
		for (size_t i = 0; i < faces.size(); i++)
		{
			Rect r = faces[i];
			if (r.height >150 && r.width >150)
			{
				cout << "\a";
				r.x -= cvRound(r.width*0.1);
				r.width = cvRound(r.width*1.2);
				r.height = cvRound(r.height*1.5);
				rectangle(img, r.tl(), r.br(), cv::Scalar(255, 0, 0), 2);
			}
		}
		imshow("Mask", img);
		char c = (char)waitKey(10);
		if (c == 27)
			break;
	}
	destroyWindow("Mask");
	destroyWindow("-Background-");
}

void Directions()
{
	namedWindow("-Background-");
	cvMoveWindow("-Background-", 0, -50);
	cvvResizeWindow("-Background-", 2400, 1000);
	Mat imgB = imread("Main.jpg");
	imshow("-Background-", imgB);
	Mat imgD = imread("imgD.jpg");
	namedWindow("Project");
	cvMoveWindow("Project", 240, 50);
	cvvResizeWindow("Project", 640, 380);

	while (1)
	{
		imshow("Project", imgD);
		char c = (char)waitKey(10);
		if (c == 27)
			break;
	}
	destroyWindow("Project");
	destroyWindow("-Background-");
}

void CallBackFunc1(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
		HumanDetection();
}

void CallBackFunc2(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
		ObjectDetection();
}

void CallBackFunc3(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
		MaskDetection();
}

void CallBackFunc4(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
		Directions();
}

void Windows()
{
	namedWindow("Background", CV_WINDOW_AUTOSIZE);
	Mat imgM = imread("Main.jpg");
	imshow("Background", imgM);
	namedWindow("Human Detection");
	Mat imgPS1 = imread("imgPS1.jpg");
	imshow("Human Detection", imgPS1);
	namedWindow("Object Detection");
	Mat imgPS2 = imread("imgPS2.jpg");
	imshow("Object Detection", imgPS2);
	namedWindow("Mask Detection");
	Mat imgPS3 = imread("imgPS3.jpg");
	imshow("Mask Detection", imgPS3);
	namedWindow("Directions");
	Mat imgPS4 = imread("Main.jpg");
	imshow("Directions", imgPS4);
	cvMoveWindow("Background", 0, -50);
	cvMoveWindow("Human Detection", winx1, winy1);
	cvMoveWindow("Object Detection", winx2, winy2);
	cvMoveWindow("Mask Detection", winx3, winy3);
	cvMoveWindow("Directions", winx4, winy4);
	cvvResizeWindow("Background", 2400, 1000);
	cvvResizeWindow("Human Detection", height, width);
	cvvResizeWindow("Object Detection", height, width);
	cvvResizeWindow("Mask Detection", height, width);
	cvvResizeWindow("Directions", height, width);
}

int main()
{
	while (1)
	{
		Windows();
		setMouseCallback("Human Detection", CallBackFunc1, NULL);
		setMouseCallback("Object Detection", CallBackFunc2, NULL);
		setMouseCallback("Mask Detection", CallBackFunc3, NULL);
		setMouseCallback("Directions", CallBackFunc4, NULL);
		char c = (char)waitKey(5);
		if (c == 27)
			break;
	}
}
















