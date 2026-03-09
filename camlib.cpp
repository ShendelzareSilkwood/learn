#include "camlib.h"
#include <QImage>
#include <QThread>

using namespace cv;
using namespace std;

uint8_t showImageBuff[320 * 1280] = { 0 };

recvData showBuff1[320] = { 0 };
recvData showBuff2[320] = { 0 };
recvData showBuff3[320] = { 0 };

testRecvData testDataBuff[3];

SOCKET serSocket;
sockaddr_in serAddr;
sockaddr_in sinAddr;
void transmit()
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData) != 0)
		return;
	serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serSocket == INVALID_SOCKET)
		return;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8080);
	serAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.100");//pc de ip
	if (::bind(serSocket, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("bind error !");
		closesocket(serSocket);
		return;
	}
	sinAddr.sin_family = AF_INET;
	sinAddr.sin_port = htons(7);
	sinAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.10");//板子的ip
}

QRect m_roiRect = QRect(0, 0, 0, 0);				//绘制的ROI数据
int getedImageNum = 0;

camlib::camlib(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	data = NULL;
	cameraMatrix = Mat(3, 3, CV_64FC1, Scalar::all(0));
	distCoeffs = Mat(1, 5, CV_64FC1, Scalar::all(0));
	tvec = Mat(3, 1, CV_64FC1, Scalar::all(0));
	rvec = Mat(3, 1, CV_64FC1, Scalar::all(0));
	rotation_matrix = Mat(3, 3, CV_64FC1, Scalar::all(0));
	H = Mat(3, 3, CV_64FC1, Scalar::all(0));
	X.resize(1280);
	background_Y.resize(1280);



    pbtns = this->findChildren<QPushButton *>();
    for (QPushButton *btn : pbtns )
		connect(btn,&QPushButton::clicked,this,&camlib::ClickButton);
}


camlib::~camlib()
{

};


/*
 * @brief				链接界面按钮及功能函数
 *
 * @return				void
 */
void camlib::ClickButton()
{
    QPushButton* bt = (QPushButton *)sender();
    string name = bt->text().toStdString();
	if (name == "打开相机")
		ClickButton_OpenCamera();
	else if (name == "图像获取")
		ClickButton_GetPicture();
	else if (name == "内参计算")
		ClickButton_Internal();
	else if (name == "外参计算")
		ClickButton_External();
	else if (name == "关闭相机")
		ClickButton_CloseCamera();
	else if (name == "图像截取")
		ClickButton_Imagecut();
	else if (name == "点云计算")
		ClickButton_Calculate();
	else if (name == "图像矫正")
		ClickButton_Correct();
	else if (name == "中心提取")
		ClickButton_CentralExtact();
	else if (name == "加载内参")
		ClickButton_LoadInternal();
	else if (name == "畸变矫正")
		ClickButton_DistortionCorrect();
	else if (name == "映射表计算")
		GenerateDistortionCorrectBackwardMappingTable();
	else if (name == "点云显示")
		pointCloud();
	else if (name == "背景保存")
		backGround();
	else if (name == "测试模式")
		testMode();
	else if (name == "背景中心")
		getBackgroundCenterPoints();
}

/*
 * @brief				输出数据至界面文本框
 *
 * @return				void
 */
void camlib::showdata(Mat data,int order)
{
	stringstream stream;
	stream << data;
	if(order == 1)
		ui.plainTextEdit->appendPlainText(QString::fromStdString(stream.str()));
	else
		ui.plainTextEdit_2->appendPlainText(QString::fromStdString(stream.str()));
}

/*
 * @brief				保存数据至txt
 *
 * @return				void
 */
void camlib::savedata(Mat data, string filename)
{
	string filepath = "./internal_data/" + filename + ".txt";
	FILE* fp = fopen(filepath.c_str(), "wb");

	for (int i = 0; i < data.rows; i++)
	{
		for (int j = 0; j < data.cols; j++)
		{
			fwrite(&data.at<double>(i, j), 1, sizeof(double), fp);
		}
	}

	fclose(fp);
}



/*
 * @brief				驱动相机
 *
 * @return				void
 */
void camlib::ClickButton_OpenCamera()
{
	ui.btn_open->setText("关闭相机");
	
	imageShow = new imageshow(this);
	imageShow->imageShowFlag = true;
	connect(imageShow, &imageshow::imageReady, this, [=]() {
		saveImg = QImage((uchar*)showImageBuff, 1280, 320, 1280, QImage::Format_Grayscale8).copy(0, 0, 1280, 320);
		ui.picture->setPixmap(QPixmap::fromImage(saveImg));
		});
	connect(imageShow, &imageshow::finished, imageShow, &QObject::deleteLater);

	imageShow->start();
}

/*
 * @brief				关闭相机进程
 *
 * @return				void
 */
void camlib::ClickButton_CloseCamera()
{
	ui.btn_open->setText("打开相机");
	char sendData[] = "close camera";
	sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr*)&sinAddr, sizeof(sinAddr));
	imageShow->imageShowFlag = false;
	closesocket(serSocket);
}

/*
 * @brief				结合打开相机捕获图像
 *
 * @return				void
 */
void camlib::ClickButton_GetPicture()
{
	QString getedImageName = QString("./saveImage/%1.bmp").arg(getedImageNum);
	saveImg.save(getedImageName);
	QMessageBox::information(this, "information", "your image has saved", QMessageBox::Ok);
	getedImageNum++;
}




/*
 * @brief				计算相机内参
 *
 * @return				void
 */
void camlib::ClickButton_Internal()
{
	filename_list = QFileDialog::getOpenFileNames(this, "choose file", "./internal/", "image (*.jpg *.png *.bmp)");
	if (filename_list.empty())
	{
		QMessageBox::information(this, "INFO", ("请选择内参图像"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return; 
	}

	QString str = ("正在计算内参，请耐心等待。");
	ui.plainTextEdit->appendPlainText(str);
	Pretreat p;
	data = p.cornor_calculate(filename_list);
	if (data == NULL)
	{
		QMessageBox::information(this, "INFO", ("角点检测失败"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}

	vector<Mat> rvecsMat;
	vector<Mat> tvecsMat;
	for (int i = 0; i < data->image.size(); i++)
	{
		QImage img(data->image[i].data, data->image[i].cols, data->image[i].rows, data->image[i].step, QImage::Format_RGB888);
		QPixmap pix = QPixmap::fromImage(img);
		pix.scaled(ui.picture->width(), ui.picture->height(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
		ui.picture->setPixmap(pix);
		waitKey(500);
	}
	calibrateCamera(data->object_points, data->image_points_seq, data->imagesize, cameraMatrix, distCoeffs, rvecsMat, tvecsMat, 0);

	ui.plainTextEdit->appendPlainText("相机内参：");
	showdata(cameraMatrix, 1);
	ui.plainTextEdit->appendPlainText("畸变系数：");
	showdata(distCoeffs, 1);

	savedata(cameraMatrix, "k");
	savedata(distCoeffs, "kc");

	filename_list.clear();
}


/*
 * @brief				加载内参
 *
 * @return				void
 */
void camlib::ClickButton_LoadInternal()
{

	FILE* fp = fopen("./internal_data/k.txt", "rb");
	FILE* fp1 = fopen("./internal_data/kc.txt", "rb");

	if ((!fp) || (!fp1))
	{
		QMessageBox::information(this, "INFO", ("请先计算内参或尝试重新计算内参"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			fread(&cameraMatrix.at<double>(i, j), 1, sizeof(double), fp);
		}
	}
	for (int i = 0; i < 5; i++)
	{
		fread(&distCoeffs.at<double>(0, i), 1, sizeof(double), fp1);
	}
	fclose(fp);
	fclose(fp1);
	ui.plainTextEdit->appendPlainText("相机内参：");
	showdata(cameraMatrix, 1);
	ui.plainTextEdit->appendPlainText("畸变系数：");
	showdata(distCoeffs, 1);
	QMessageBox::information(this, "INFO", ("内参已经加载成功"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
}



/*
 * @brief				计算相机外参
 *
 * @return				void
 */
void camlib::ClickButton_External()
{
	filename_list = QFileDialog::getOpenFileNames(this, "choose file", "./external", "image (*.jpg *.png *.bmp)");
	if (filename_list.empty())
	{
		QMessageBox::information(this, "INFO", ("请选择外参图像"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}
	if (filename_list.size() > 1)
	{
		QMessageBox::information(this, "INFO", ("外参图像只能选择一张哦"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}
	if (countNonZero(cameraMatrix) == 0)
	{
		QMessageBox::information(this, "INFO", ("请先得到相机内参"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}

	Pretreat p;
	data = p.cornor_calculate1(filename_list);
	if (data == NULL)
	{
		QMessageBox::information(this, "INFO", ("角点检测失败"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}
	string filename = filename_list.at(0).toStdString();
	filename = filename.substr(0, filename.find_last_of('.'));
	filename = filename + ".txt";
	int buff[4] = { 0 };
	QMessageBox::StandardButton rb = QMessageBox::question(this, "question", ("是否使用了截取图像"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	if (rb == QMessageBox::Yes)
	{
		if (m_roiRect.x() == 0 && m_roiRect.y() == 0)
		{
			ifstream f_roi;
			f_roi.open(filename);
			if (!f_roi.is_open())
			{
				QMessageBox::StandardButton rb = QMessageBox::question(this, "question", ("选择的图像不是截取的图像"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
				return;
			}
			for (int i = 0; i < 4; i++)
			{
				f_roi >> buff[i];
			}
			f_roi.close();
			m_roiRect = QRect(buff[0], buff[1], buff[2], buff[3]);
		}
		for (int i = 0; i < data->image_points_seq[0].size(); i++)
		{
			data->image_points_seq[0][i].x += m_roiRect.x();
			data->image_points_seq[0][i].y += m_roiRect.y();
		}
		QImage img(data->image[0].data, data->image[0].cols, data->image[0].rows, data->image[0].step, QImage::Format_RGB888);
		QPixmap pix = QPixmap::fromImage(img);
		pix.scaled(ui.picture->width(), ui.picture->height());
		ui.picture->setPixmap(pix);
		waitKey(10);
	}
	else
	{
		QImage img(data->image[0].data, data->image[0].cols, data->image[0].rows, data->image[0].step, QImage::Format_RGB888);
		ui.picture->setPixmap(QPixmap::fromImage(img));
		waitKey(10);
	}

	solvePnPRansac(data->object_points[0], data->image_points_seq[0], cameraMatrix, distCoeffs, rvec, tvec, false, 100, 2);
	Rodrigues(rvec, rotation_matrix);

	ui.plainTextEdit_2->appendPlainText("平移向量：");
	showdata(tvec, 2);
	ui.plainTextEdit_2->appendPlainText("旋转向量：");
	showdata(rvec, 2);
	ui.plainTextEdit_2->appendPlainText("旋转矩阵：");
	showdata(rotation_matrix, 2);

	tvec.copyTo(rotation_matrix.col(2));
	H = cameraMatrix * rotation_matrix;
	savedata(H.inv(), "h");
	filename_list.clear();
}


/*
 * @brief				矫正图像的功能函数
 *
 * @return				矫正后的Mat图像
 */
Mat camlib::ImageCorrect(Mat ori_image)
{
	Mat redress_image = ori_image.clone();
	Size imagesize;
	imagesize.width = ori_image.cols;
	imagesize.height = ori_image.rows;
	Mat mapx = Mat(imagesize, CV_64FC1);
	Mat mapy = Mat(imagesize, CV_64FC1);
	Mat R = Mat::eye(3, 3, CV_64FC1);
	initUndistortRectifyMap(cameraMatrix, distCoeffs, R, cameraMatrix, imagesize, CV_32FC1, mapx, mapy);
	remap(ori_image, redress_image, mapx, mapy, INTER_LINEAR);
	return redress_image;
}


/*
 * @brief				矫正图像
 *
 * @return				void
 */
void camlib::ClickButton_Correct()
{
	filename_list = QFileDialog::getOpenFileNames(this, "choose file", "./", "image (*.jpg *.png *.bmp)");
	if (filename_list.empty())
	{
		QMessageBox::information(NULL, "INFO", ("请选择矫正图像"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}
	if (countNonZero(cameraMatrix) == 0)
	{
		QMessageBox::information(this, "INFO", ("请先求得相机内参"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}
	string filename,temp_imagename, imagename, newimage_name, filepath;
	Mat ori_image, newimage;
	for (int i = 0; i < filename_list.size(); i++)
	{
		filename = filename_list.at(i).toStdString();
		ori_image = imread(filename);
		newimage = ImageCorrect(ori_image);
		filepath = "./correct_image/";
		temp_imagename = filename.substr(filename.find_last_of('/') + 1);
		imagename = temp_imagename.substr(0, temp_imagename.find_last_of("."));
		newimage_name = filepath + imagename + "_Correct.bmp";
		imwrite(newimage_name, newimage);
	}
	QMessageBox::information(NULL, "INFO", ("矫正图像已保存"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
}


/*
 * @brief				图像滤波(中值滤波、高斯滤波)
 *
 * @return				滤波后的Mat图像
 */
Mat camlib::ImageProcess(Mat I)
{
	Mat I_gray;
	cvtColor(I, I_gray, CV_BGR2GRAY);
	Mat I_gray_medfilt;
	medianBlur(I_gray, I_gray_medfilt, 3);
	Mat I_gray_Medfilt_Gaussfilt;
	GaussianBlur(I_gray_medfilt, I_gray_Medfilt_Gaussfilt, Size(5, 5), 1.6, 1.6);
	Mat I1;
	I_gray_Medfilt_Gaussfilt.convertTo(I1, CV_64F);
	return I1;
}


/*
 * @brief				中心提取功能函数
 *
 * @return				结构光中心点坐标
 */
vector<Point2d> camlib::grayCenter(Mat I1, vector<double> Max,vector<Point2d> L)
{
	Point2d p;
	vector<Point2d> P;
	double sum;
	double sum1;
	for (int i = 0; i < I1.cols; i++)
	{
		sum = 0;
		sum1 = 0;
		p.x = i;
		for (int j = L[i].y - 10; j <= L[i].y + 10; j++)
		{
			if (Max[i] < 20)
				p.y = 0;
			else
			{
				sum += j * I1.at<double>(j, i);
				sum1 += I1.at<double>(j, i);
			}
		}
		p.y = sum / sum1;
		P.push_back(p);
	}
	return P;
}


/*
 * @brief				提取光条纹中心数据
 *
 * @return				void
 */
void camlib::ClickButton_CentralExtact()
{
	filename_list = QFileDialog::getOpenFileNames(this, "choose file", "./", "image (*.jpg *.png *.bmp)");
	if (filename_list.empty())
	{
		QMessageBox::information(NULL, "INFO", ("请选择中心提取图像"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}

	string filename;
	Mat I,I1;
	double max;
	Point l;
	Point2d ll;
	vector<double> Max;
	vector<Point2d> L;
	vector<Point2d> temp_ImagePoints;
	vector<Point2d> ImagePoints;
	for (int i = 0; i < filename_list.size(); i++)
	{
		max = 0;
		l.x = 0;
		l.y = 0;
		ll.x = 0;
		ll.y = 0;
		temp_ImagePoints.clear();
		ImagePoints.clear();
		Max.clear();
		L.clear();
		filename = filename_list.at(i).toStdString();
		I = imread(filename);
		I1 = ImageProcess(I);
		for (int i = 0; i < I1.cols; i++)
		{
			minMaxLoc(I1.col(i), NULL, &max, NULL, &l);
			ll.x = l.x;
			ll.y = l.y;
			Max.push_back(max);
			L.push_back(ll);
		}
		temp_ImagePoints = grayCenter(I1, Max, L);
		ImagePoints = grayCenter(I1, Max, temp_ImagePoints);
		LPoints.push_back(ImagePoints);
	}

//#define DEBUG

#ifdef DEBUG
	
	unsigned short a;
	unsigned short b;
	FILE* fp = fopen("center_of_strip.txt", "wb");
	for (int j = 0; j < LPoints[0].size(); j++)
	{
		a = (unsigned short)round(LPoints[0][j].x * 32);
		b = (unsigned short)round(LPoints[0][j].y * 32);
		fwrite(&a, 1, sizeof(unsigned short), fp);
		fwrite(&b, 1, sizeof(unsigned short), fp);
	}
	fclose(fp);
	
	
	
#endif // DEBUG

	QMessageBox::information(NULL, "INFO", ("提取完成"), QMessageBox::Ok);
}


double LinearValue(vector<Point3d>& OPoints, double x)
{

	for (int i = 0; i < OPoints.size(); i++)
	{
		if (x < OPoints[i].x)
		{
			return OPoints[i - 1].y + (x - OPoints[i - 1].x) / (OPoints[i].x - OPoints[i - 1].x) * (OPoints[i].y - OPoints[i - 1].y);
		}
	}
	return 0;
}
double LinearValue(vector<double>& world_X, vector<double>& world_Y, double x )
{

	for (int i = 0; i < world_X.size(); i++)
	{
		if (x < world_X[i])
		{
			return world_Y[i-1] + (x - world_X[i - 1]) / (world_X[i] - world_X[i - 1]) * (world_Y[i] - world_Y[i - 1]);
		}
	}
	return 0;
}


/*
 * @brief				高度计算
 *
 * @return				void
 */
void camlib::ClickButton_Calculate()
{
	if (LPoints.empty())
	{
		QMessageBox::information(NULL, "INFO", ("请先提取中心点"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}
	int num = LPoints[0].size();
	Mat m = Mat(3, num, CV_64FC1,Scalar::all(0));
	Mat M = Mat(3, num, CV_64FC1, Scalar::all(0));
	vector<Point3d> object;
	Point3d temp;
	Point3d temp_result;
	vector<Point3d> result;
	for (int i = 0; i < LPoints.size(); i++)
	{
		object.clear();
		for (int j = 0; j < num; j++)
		{
			m.at<double>(0, j) = LPoints[i][j].x;
			m.at<double>(1, j) = LPoints[i][j].y;
			m.at<double>(2, j) = 1;
		}
		M = H.inv() * m;
		for (int k = 0; k < num; k++)
		{
			temp.x = M.at<double>(0, k) / M.at<double>(2, k);
			temp.y = M.at<double>(1, k) / M.at<double>(2, k);
			temp.z = M.at<double>(2, k);
			object.push_back(temp);
		}
		OPoints.push_back(object);
	}


	Point2d InvaluePoint;
	Point2d InvaluePointFirst;
	Point2d InvaluePointLast;
	vector<Point2d> InvaluePointBuff;
	for (int i = 0; i < OPoints.size(); i++)
	{
		InvaluePointBuff.clear();
		InvaluePointFirst.x = (int)OPoints[i][0].x;
		InvaluePointFirst.y = 0;
		InvaluePointLast.x = (int)OPoints[i][num - 1].x;
		InvaluePointLast.y = 0;
		double delta = (InvaluePointLast.x - InvaluePointFirst.x) / (num - 1);
		InvaluePointBuff.push_back(InvaluePointFirst);
		for (int j = 1; j < num-1; j++)
		{
			InvaluePoint.x = InvaluePointFirst.x + j * delta;
			InvaluePoint.y = 0;
			InvaluePointBuff.push_back(InvaluePoint);
		}
		InvaluePointBuff.push_back(InvaluePointLast);
		Invalue.push_back(InvaluePointBuff);
	}

	for (int i = 0; i < Invalue.size(); i++)
	{
		for (int j = 0; j < Invalue[0].size(); j++)
		{
			Invalue[i][j].y = LinearValue(OPoints[i], Invalue[i][j].x);
		}
	}

	for (int i = 1; i < OPoints.size(); i++)
	{
		result.clear();
		for (int j = 0; j < OPoints[0].size(); j++)
		{
			temp_result.x = std::abs(OPoints[i][j].x - OPoints[0][j].x);
			temp_result.y = std::abs(OPoints[i][j].y - OPoints[0][j].y);
			temp_result.z = std::abs(OPoints[i][j].z - OPoints[0][j].z);
			result.push_back(temp_result);
		}
		m_result.push_back(result);
	}
	
	LPoints.clear();
	OPoints.clear();
	dataPlot* d = new dataPlot();
	d->show();
	d->m_plot(m_result);
	m_result.clear();
	Invalue.clear();
}


/*
 * @brief				图像截取
 *
 * @return				void
 */
void camlib::ClickButton_Imagecut()
{
	CaptureScreen* c = new CaptureScreen();
	c->show();
	c->setAttribute(Qt::WA_DeleteOnClose);
}

void camlib::ClickButton_DistortionCorrect()
{
	if (LPoints.empty())
	{
		QMessageBox::information(NULL, "INFO", ("请先提取中心点"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}
	if (countNonZero(cameraMatrix) == 0)
	{
		QMessageBox::information(this, "INFO", ("请先求得相机内参"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}

	Mat _2 = Mat::zeros(2, 1280, CV_64FC1);				
	Mat r_2 = Mat::zeros(1, 1280, CV_64FC1);
	Mat r_4 = Mat::zeros(1, 1280, CV_64FC1);
	Mat r_6 = Mat::zeros(1, 1280, CV_64FC1);
	Mat k_radial = Mat::zeros(1, 1280, CV_64FC1);
	Mat x_2 = Mat::zeros(1, 1280, CV_64FC1);
	Mat y_2 = Mat::zeros(1, 1280, CV_64FC1);
	Mat m_LPoints_delta = Mat::zeros(2, 1280, CV_64FC1);
	Mat e = Mat::ones(2, 1, CV_64FC1);
	Mat E = Mat::ones(1, 1280, CV_64FC1);

	for (int i = 0; i < LPoints.size(); i++)
	{
		for (int j = 0; j < LPoints[0].size(); j++)
		{
			LPoints[i][j].y = (LPoints[i][j].y - cameraMatrix.at<double>(1, 2)) / cameraMatrix.at<double>(1, 1);
			LPoints[i][j].x = (LPoints[i][j].x - cameraMatrix.at<double>(0, 2) - LPoints[i][j].x * cameraMatrix.at<double>(0,1)) / cameraMatrix.at<double>(0, 0);
		}
	}

	vector<Mat>  m_LPoints;
	for (int i = 0; i < LPoints.size(); i++)
	{
		Mat temp = Mat::zeros(2, 1280, CV_64FC1);
		for (int j = 0; j < temp.cols; j++)
		{
			temp.at<double>(0, j) = LPoints[i][j].x;
			temp.at<double>(1, j) = LPoints[i][j].y;
		}
		m_LPoints.push_back(temp);
	}
	LPoints.clear();
	vector<Mat> m_Lpoints_copy;
	for (int i = 0; i < m_LPoints.size(); i++)
	{
		Mat new_temp = m_LPoints[i].clone();
		m_Lpoints_copy.push_back(new_temp);
	}
	for (int i = 0; i < m_LPoints.size(); i++)
	{
		int cnt = 0;
		while (cnt < 20)
		{
			pow(m_LPoints[i], 2, _2);
			r_2 = _2.row(0) + _2.row(1);
			pow(r_2, 2, r_4);
			pow(r_2, 3, r_6);
			pow(m_LPoints[i].row(0), 2, x_2);
			pow(m_LPoints[i].row(1), 2, y_2);
			k_radial = 1 + distCoeffs.at<double>(0) * r_2 + distCoeffs.at<double>(1) * r_4 + distCoeffs.at<double>(4) * r_6;
			m_LPoints_delta.row(0) = 2 * distCoeffs.at<double>(2) * m_LPoints[i].row(0).mul(m_LPoints[i].row(1)) + distCoeffs.at<double>(3) * (r_2 + 2 * x_2);
			m_LPoints_delta.row(1) = distCoeffs.at<double>(2) * (r_2 + 2 * y_2) + 2 * distCoeffs.at<double>(3) * m_LPoints[i].row(0).mul(m_LPoints[i].row(1));
			m_LPoints[i] = (m_Lpoints_copy[i] - m_LPoints_delta) / (e * k_radial);
			cnt++;
		}
	}

	for ( int i = 0; i < m_LPoints.size(); i++)
	{
		m_LPoints[i].push_back(E);
		m_LPoints[i] = cameraMatrix * m_LPoints[i];
	}
	vector<Point2d> Correct_imagepoints_buff;
	Point2d Correct_imagepoints;
	for (int i = 0; i < m_LPoints.size(); i++)
	{
		Correct_imagepoints_buff.clear();
		for (int j = 0; j < m_LPoints[0].cols; j++)
		{
			Correct_imagepoints.x = m_LPoints[i].at<double>(0, j);
			Correct_imagepoints.y = m_LPoints[i].at<double>(1, j);
			Correct_imagepoints_buff.push_back(Correct_imagepoints);
		}
		LPoints.push_back(Correct_imagepoints_buff);
	}
	QMessageBox::information(NULL, "INFO", ("光条中心校正完毕"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
}

void camlib::DistortionCorrect(vector<Point2f>& OneLine)
{

	Mat _2 = Mat::zeros(2, 1280, CV_32FC1);
	Mat r_2 = Mat::zeros(1, 1280, CV_32FC1);
	Mat r_4 = Mat::zeros(1, 1280, CV_32FC1);
	Mat r_6 = Mat::zeros(1, 1280, CV_32FC1);
	Mat k_radial = Mat::zeros(1, 1280, CV_32FC1);
	Mat x_2 = Mat::zeros(1, 1280, CV_32FC1);
	Mat y_2 = Mat::zeros(1, 1280, CV_32FC1);
	Mat m_LPoints_delta = Mat::zeros(2, 1280, CV_32FC1);
	Mat e = Mat::ones(2, 1, CV_32FC1);
	Mat E = Mat::ones(1, 1280, CV_32FC1);

	for (int j = 0; j < OneLine.size(); j++)
	{
		OneLine[j].y = (OneLine[j].y - cameraMatrix.at<double>(1, 2)) / cameraMatrix.at<double>(1, 1);
		OneLine[j].x = (OneLine[j].x - cameraMatrix.at<double>(0, 2) - OneLine[j].x * cameraMatrix.at<double>(0, 1)) / cameraMatrix.at<double>(0, 0);
	}

	Mat m_OneLine = Mat::zeros(2, 1280, CV_32FC1);
	for (int j = 0; j < m_OneLine.cols; j++)
	{
		m_OneLine.at<float>(0, j) = OneLine[j].x;
		m_OneLine.at<float>(1, j) = OneLine[j].y;
	}


	Mat m_OneLineCopy = m_OneLine.clone();

	int cnt = 0;
	while (cnt < 20)
	{
		pow(m_OneLine, 2, _2);
		r_2 = _2.row(0) + _2.row(1);
		pow(r_2, 2, r_4);
		pow(r_2, 3, r_6);
		pow(m_OneLine.row(0), 2, x_2);
		pow(m_OneLine.row(1), 2, y_2);
		k_radial = 1 + distCoeffs.at<double>(0) * r_2 + distCoeffs.at<double>(1) * r_4 + distCoeffs.at<double>(4) * r_6;
		m_LPoints_delta.row(0) = 2 * distCoeffs.at<double>(2) * m_OneLine.row(0).mul(m_OneLine.row(1)) + distCoeffs.at<double>(3) * (r_2 + 2 * x_2);
		m_LPoints_delta.row(1) = distCoeffs.at<double>(2) * (r_2 + 2 * y_2) + 2 * distCoeffs.at<double>(3) * m_OneLine.row(0).mul(m_OneLine.row(1));
		m_OneLine = (m_OneLineCopy - m_LPoints_delta) / (e * k_radial);
		cnt++;
	}

	m_OneLine.push_back(E);
	Mat m_cameraMatrix;
	cameraMatrix.convertTo(m_cameraMatrix, CV_32FC1);
	m_OneLine = m_cameraMatrix * m_OneLine;

	Point2f CorrectImagePoints;
	for (int j = 0; j < m_OneLine.cols; j++)
	{
		CorrectImagePoints.x = m_OneLine.at<float>(0, j);
		CorrectImagePoints.y = m_OneLine.at<float>(1, j);
		OneLine[j] = CorrectImagePoints;
	}

}
void camlib::GenerateDistortionCorrectBackwardMappingTable()
{
	ofstream out("UcVc.txt",ios::binary);
	
	vector<Point2f> OneLine;
	//vector<Point2f> text;
	OneLine.reserve(1280);
	//text.resize(1280);
	for (int i = 0; i < 319 * 32 + 1; i++)
	{
		OneLine.clear();
		for (int j = 0; j < 1280; j++)
		{
			Point2f p;
			p.x = j;
			p.y = (float)i / 32;
			OneLine.push_back(p);
		}
		DistortionCorrect(OneLine);
		//qDebug() << sizeof(Point2f);
		out.write((char*)&OneLine[0], 1280 * sizeof(Point2f));
		//out.close();
		//ifstream in("findchart.txt", ios::binary);
		//in.read((char*)&text[0], 1280 * sizeof(Point2f));
		//in.close();
	}
	out.close();
}



void camlib::backGround()
{
	 //判定前置条件
	if (LPoints.empty())
	{
		QMessageBox::information(NULL, "INFO", ("请先提取中心点"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}
	if (countNonZero(H) == 0)
	{
		QMessageBox::information(this, "INFO", ("请先求得相机外参"), QMessageBox::Yes | QMessageBox::Ok, QMessageBox::Yes);
		return;
	}

	//获取原始的条纹中心数据
	vector<Point2d> stripeCenter;			//背景的条纹中心数据(u,v).
	stripeCenter.resize(1280);
	stripeCenter = LPoints[0];

	//计算世界坐标
	Mat m = Mat(3, stripeCenter.size(), CV_64FC1, Scalar::all(0));
	Mat M = Mat(3, stripeCenter.size(), CV_64FC1, Scalar::all(0));
	vector<Point3d> worldPoints;			//(u,v)对应的世界坐标(x,y)
	Point3d oneOf_worldPoints;

	for (int i = 0; i < stripeCenter.size(); i++)
	{
		m.at<double>(0, i) = stripeCenter[i].x;
		m.at<double>(1, i) = stripeCenter[i].y;
		m.at<double>(2, i) = 1;
	}

	M = H.inv() * m;

	for (int i = 0; i < stripeCenter.size(); i++)
	{
		oneOf_worldPoints.x = M.at<double>(0, i) / M.at<double>(2, i);
		oneOf_worldPoints.y = M.at<double>(1, i) / M.at<double>(2, i);
		oneOf_worldPoints.z = M.at<double>(2, i);
		worldPoints.push_back(oneOf_worldPoints);
	}

// 确定 待插值X 的坐标范围

	Point2d oneOf_interedWorldPoints; 
	Point2d first_interedWorldPoints;
	Point2d last_interedWorldPoints;

	vector<Point2d> interedWorldPoints;
	
	//根据经验设定的的断电值
	first_interedWorldPoints.x = -330;
	first_interedWorldPoints.y = 0;
	last_interedWorldPoints.x = 1120;
	last_interedWorldPoints.y = 0;
	double delta = (last_interedWorldPoints.x - first_interedWorldPoints.x) / (stripeCenter.size() - 1);
	interedWorldPoints.push_back(first_interedWorldPoints);
	for (int i = 1; i < stripeCenter.size() - 1; i++)
	{
		oneOf_interedWorldPoints.x = first_interedWorldPoints.x + i * delta;
		oneOf_interedWorldPoints.y = 0;
		interedWorldPoints.push_back(oneOf_interedWorldPoints);
	}
	interedWorldPoints.push_back(last_interedWorldPoints);

	vector<double> WorldPoints_X;
	WorldPoints_X.resize(1280);

	vector<double> WorldPoints_Y;
	WorldPoints_Y.resize(1280);
	for (int i = 0; i < 1280; i++)
	{
		memcpy(&WorldPoints_X[i], &worldPoints[i], sizeof(double));
		memcpy(&WorldPoints_Y[i], (double*)&worldPoints[i] + 1, sizeof(double));
	}



	//线性插值
	for (int i = 0; i < stripeCenter.size(); i++)
	{
		interedWorldPoints[i].y = LinearValue(WorldPoints_X, WorldPoints_Y, interedWorldPoints[i].x);
	}
	
	FILE* fp = fopen("Xbackground.txt", "wb");
	FILE* fp1 = fopen("Ybackground.txt", "wb");
	for (int i = 0; i < 1280; i++)
	{
		fwrite(&interedWorldPoints[i], 1, 8, fp);
		fwrite((double*)&interedWorldPoints[i] + 1, 1, 8, fp1);
	}
	fclose(fp);
	fclose(fp1);

	QMessageBox::information(NULL, QString("Background Save"), QString::fromLocal8Bit("计算需要的背景已保存"), QMessageBox::Ok);

//#define DEBUG
#ifdef DEBUG

	vector<Point2d> interedWorldPointsDebug;
	interedWorldPointsDebug.resize(1280);
	FILE* m_fp = fopen("Xbackground.txt", "rb");
	FILE* m_fp1 = fopen("Ybackground.txt", "rb");
	for (int i = 0; i < 1280; i++)
	{
		fread(&interedWorldPointsDebug[i], 1, 8, m_fp);
		fread((double*)&interedWorldPointsDebug[i] + 1, 1, 8, m_fp1);
	}
	fclose(m_fp);
	fclose(m_fp1);

#endif // DEBUG
}


double m_sort(deque<double> d)
{
	sort(d.begin(), d.end());
	return d[d.size() / 2];
}
void myMidFilter(vector<double>& v, int size)
{
	
	deque<double> d(v.begin(), v.begin() + 1280);
	deque<double> d1(v.begin() + 1280, v.end());
	const double min_X = d[0];
	const double max_X = d[1279];
	const double min_Y = d[0];
	const double max_Y = d[1279];
	d.insert(d.begin(), (size - 1) / 2, min_X);
	d.insert(d.end(), (size - 1) / 2, max_X);
	d1.insert(d1.begin(), (size - 1) / 2, min_Y);
	d1.insert(d1.end(), (size - 1) / 2, max_Y);
	deque<double>::iterator it = d.begin();
	deque<double>::iterator it1 = d1.begin();
	for (int i = 0; i < 1280; i++, it++, it1++)
	{
		v[i] = m_sort(deque<double>(it, it + size));
		v[1280 + i] = m_sort(deque<double>(it1, it1 + size));
	}
}

class Aminus
{
public:
	uint8_t operator()(double val1, double val2)
	{
		return (uint8_t)(abs(val1 - val2) * 0.255);
	}
};
void LinearValue(vector<double>& world_X, vector<double>& world_Y, vector<double>& X, vector<double>& interedWorldPoints_Y)
{
	int  i = 0, j = 1;
	while (i < X.size())
	{
		if ((X[i] < world_X[j])&& (X[i] > world_X[j-1]))
		{
			interedWorldPoints_Y[i++] = world_Y[j - 1] + (X[i] - world_X[j - 1]) / (world_X[j] - world_X[j - 1]) * (world_Y[j] - world_Y[j - 1]);
			j = 1;
		}
		else
			j++;
	//	X[i] < world_X[j] ? interedWorldPoints_Y[i++] = world_Y[j - 1] + (X[i] - world_X[j - 1]) / (world_X[j] - world_X[j - 1]) * (world_Y[j] - world_Y[j - 1]) : j++;
	}
}


void camlib::pointCloudImageShow(int buffFlag)
{
	vector<double> worldPoints_X;
	worldPoints_X.resize(1280);
	vector<double> worldPoints_Y;
	worldPoints_Y.resize(1280);
	vector<double> interedWorldPoints_Y(1280, 0);
	unsigned char* buff = new unsigned char[320 * 1280];

	//myMidFilter(worldPoints, 57);
	if (buffFlag == 1)
	{
		for (int i = 0; i < 320; i++)
		{
			worldPoints_X.assign(showBuff1[i].X, showBuff1[i].X + 1280);
			worldPoints_Y.assign(showBuff1[i].Y, showBuff1[i].Y + 1280);
//			LinearValue(worldPoints_X, worldPoints_Y, X, interedWorldPoints_Y);
//			transform(interedWorldPoints_Y.begin(), interedWorldPoints_Y.end(), background_Y.begin(), &buff[i * 1280], Aminus());		//对数据做运算放至指定容器内
			transform(worldPoints_Y.begin(), worldPoints_Y.end(), background_Y.begin(), &buff[i * 1280], Aminus());		//对数据做运算放至指定容器内
		}
	}
	else if(buffFlag == 2)
	{
		for (int i = 0; i < 320; i++)
		{
			worldPoints_X.assign(showBuff2[i].X, showBuff2[i].X + 1280);
			worldPoints_Y.assign(showBuff2[i].Y, showBuff2[i].Y + 1280);
//			LinearValue(worldPoints_X, worldPoints_Y, X, interedWorldPoints_Y);
//			transform(interedWorldPoints_Y.begin(), interedWorldPoints_Y.end(), background_Y.begin(), &buff[i * 1280], Aminus());		//对数据做运算放至指定容器内
			transform(worldPoints_Y.begin(), worldPoints_Y.end(), background_Y.begin(), &buff[i * 1280], Aminus());		//对数据做运算放至指定容器内

		}
	}
	else if (buffFlag == 3)
	{
		for (int i = 0; i < 320; i++)
		{
			worldPoints_X.assign(showBuff3[i].X, showBuff3[i].X + 1280);
			worldPoints_Y.assign(showBuff3[i].Y, showBuff3[i].Y + 1280);
//			LinearValue(worldPoints_X, worldPoints_Y, X, interedWorldPoints_Y);
//			transform(interedWorldPoints_Y.begin(), interedWorldPoints_Y.end(), background_Y.begin(), &buff[i * 1280], Aminus());		//对数据做运算放至指定容器内
			transform(worldPoints_Y.begin(), worldPoints_Y.end(), background_Y.begin(), &buff[i * 1280], Aminus());		//对数据做运算放至指定容器内

		}
	}
	
	QImage img1 = QImage((uchar*)buff, 1280, 320, 1280, QImage::Format_Grayscale8).copy(0, 0, 1280, 320);
	delete[] buff;
	ui.picture->setPixmap(QPixmap::fromImage(img1));
}


void camlib::pointCloud()
{
	ui.pushButton_11->setText("关闭点云");
	dataprocess* dataProcessThread = new dataprocess(this);
	connect(dataProcessThread, &dataprocess::resultReady, this, &camlib::pointCloudImageShow);
	connect(dataProcessThread, &dataprocess::recvError, this, [=](int dataNum) {
			ui.plainTextEdit_3->setPlainText(QString::number(dataNum));
		});
	connect(dataProcessThread, &dataprocess::finished, dataProcessThread, &QObject::deleteLater);
	dataProcessThread->start();
	
	//读取背景数据
	FILE* m_fp = fopen("Xbackground.txt", "rb");
	FILE* m_fp1 = fopen("Ybackground.txt", "rb");
	for (int i = 0; i < 1280; i++)
	{
		fread(&X[i], 1, 8, m_fp);
		fread(&background_Y[i], 1, 8, m_fp1);
	}
	fclose(m_fp);
	fclose(m_fp1);
}

void camlib::testDrawinit()
{
		seriesV = new QLineSeries();
		seriesGrayscale = new QLineSeries();

		//设置线条
		seriesV->setName(QString("line: Fringe Center"));
		seriesGrayscale->setName(QString("line: Grayscale "));
//		seriesV->setColor(QColor(255, 0, 255));							//设置线条颜色
//		seriesV->setVisible(true);										//设置线条是否可视,默认为true
		seriesV->setPointLabelsVisible(false);							//数据显示标签是否可视
//		seriesV->setPointLabelsColor(QColor(255, 255, 255));			//标签颜色
//		seriesV->setPointLabelsFont(QFont("微软雅黑"));
		seriesV->setPointLabelsFormat("(@xPoint,@yPoint)");				//数据标签显示格式
//		seriesV->setPointLabelsFormat("@yPoint");						//数据标签显示格式
		seriesV->setPointLabelsClipping(false);							//数据标签边界是否截取
//		seriesV->setPointsVisible(true);								//设置"点"是否可见
		seriesGrayscale->setPointLabelsVisible(false);					//数据显示标签是否可视
		seriesGrayscale->setPointLabelsFormat("(@xPoint,@yPoint)");		//数据标签显示格式
		seriesGrayscale->setPointLabelsClipping(false);					//数据标签边界是否截取

		//设置坐标系
		QValueAxis* axisX = new QValueAxis();
		QValueAxis* axisY = new QValueAxis();
		
		axisX->setRange(0, 1280);
		axisX->setTitleText("u/pixel");								//设置坐标轴含义
//		axisX->setTitleBrush(QBrush(QColor(Qt::green)));
//		axisX->setTitleFont(QFont("Times New Roman"));
//		axisX->setTitleVisible(false);
		axisX->setMinorTickCount(8);								//设置每个子刻度之间有接个刻度
		axisX->setTickCount(9);										//设置总的刻度有几个
		axisX->setTickType(QValueAxis::TicksFixed);
//		axisX->setGridLineColor(Qt::white);							//设置总刻度线的颜色
//		axisX->setGridLineVisible(false);							//设置总刻度线是否可见，默认为true
//		axisX->setGridLinePen(QPen(QColor(Qt::white)));
//		axisX->setMinorGridLineColor(Qt::white);					//设置总刻度线之间刻度线的颜色(不太会设置）
//		axisX->setMinorGridLinePen(QPen(Qt::DotLine));
//		axisX->setMinorGridLineVisible(false);		
//		axisX->setReverse(true);									//设置坐标轴反转，默认为false
 		axisX->setLabelFormat("%d");								//设置下标以什么数据类型显示
//		axisX->setLabelsAngle(90);									//设置下标数字的角度
//		axisX->setLabelsColor(Qt::green);							//设置下标数字的颜色
//		axisX->setLabelsFont(QFont("宋体"));						//设置小标数字的字体
//		axisX->setLabelsVisible(false);
//		axisX->setLabelsBrush(QBrush(QColor(Qt::green)));
		axisY->setRange(-320, 320);
		axisY->setTitleText("v/pixel");								//设置坐标轴含义
//		axisY->setMinorTickCount(8);								//设置每个子刻度之间有接个刻度
		axisY->setTickCount(9);										//设置总的刻度有几个
		axisY->setTickType(QValueAxis::TicksFixed);
//		axisY->setGridLineColor(Qt::white);							//设置总刻度线的颜色
		axisY->setLabelFormat("%d");								//设置下标以什么数据类型显示
//		axisY->setMinorGridLineVisible(false);
//		axisY->setGridLineVisible(false);							//设置总刻度线是否可见，默认为true

		//设置图表
		chart = new QChart();

		chart->setTheme(QChart::ChartThemeDark);					//设置主题样式
		chart->addSeries(seriesV);									//添加线条
		chart->addSeries(seriesGrayscale);
		chart->setTitle("Fringe Center & Grayscale");				//设置标题
		chart->createDefaultAxes();
		chart->setAxisY(axisY,seriesV);
		chart->setAxisX(axisX, seriesV);
		chart->setAxisY(axisY, seriesGrayscale);
		chart->setAxisX(axisX, seriesGrayscale);
//		chart->setTitleBrush(QBrush(QColor(255, 170, 255)));		//设置标题Brush	
//		chart->setTitleFont(QFont("微软雅黑"));						//设置标题字体

		//设置图表中的图例
		chart->legend()->setVisible(true);
		chart->legend()->setAlignment(Qt::AlignTop);				//设置位置
//		chart->legend()->setBackgroundVisible(true);				//设置背景是否可视
//		chart->legend()->setLabelColor(QColor(255, 128, 255));		//设置标签颜色
//		chart->legend()->setBorderColor(QColor(255, 255, 170));		//设置边框颜色
//		chart->legend()->setFont(QFont("微软雅黑", 12, -1, true));


		//设置图表显示
		testDrawWidget = new TestDrawWidget(this);
		testDrawWidget->setGeometry(this->geometry());
		testDrawWidget->statShow->setFixedSize(QSize(testDrawWidget->x(), 4.2 * testDrawWidget->y()));
		testDrawWidget->chartView->setChart(chart);
		testDrawWidget->chartView->setRenderHint(QPainter::Antialiasing);

		connect(testDrawWidget, &TestDrawWidget::widgetClose, this, [=]() 
			{
				char sendData[] = "close debug";
				sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr*)&sinAddr, sizeof(sinAddr));
				testProcessThread->dataShowFlag = false;
				closesocket(serSocket);
			});
}

void camlib::testMode()
{
	testDrawinit();
	testDrawWidget->show();
	testProcessThread = new test(this);
	connect(testProcessThread, &test::resultReady, this, &camlib::testDataPlot);
	connect(testProcessThread, &test::finished, testProcessThread, &QObject::deleteLater);
	testProcessThread->start();
}

void camlib::testDataPlot(int num)
{
	testDrawWidget->statShow->clear();
	QVector<QPointF> vData(1280, QPointF(0, 0));
	QVector<QPointF> grayscaleData(1280, QPointF(0, 0));
	for (int i = 0; i < 1280; i++)
	{
		vData.push_back(QPointF(i, -testDataBuff[num].v[i]));
		grayscaleData.push_back(QPointF(i, testDataBuff[num].fringeMaxValue[i]));
	}

	testDrawWidget->statShow->append((QString::fromLocal8Bit("灰度大于门限50条纹列数:		%1").arg(testDataBuff[num].StatsData.t50_cnt)));
	testDrawWidget->statShow->append((QString::fromLocal8Bit("灰度大于门限100条纹列数:		%1").arg(testDataBuff[num].StatsData.t100_cnt)));
	testDrawWidget->statShow->append((QString::fromLocal8Bit("灰度大于门限150列数:		%1").arg(testDataBuff[num].StatsData.t1_cnt)));
	testDrawWidget->statShow->append((QString::fromLocal8Bit("灰度大于门限200列数:		%1").arg(testDataBuff[num].StatsData.t2_cnt)));
	testDrawWidget->statShow->append((QString::fromLocal8Bit("灰度大于门限250条纹列数:		%1").arg(testDataBuff[num].StatsData.t250_cnt)));
	testDrawWidget->statShow->append(QString(" "));

	testDrawWidget->statShow->append((QString::fromLocal8Bit("灰度大于门限150列宽度均值:		%1").arg(testDataBuff[num].StatsData.t1w_mean)));
	testDrawWidget->statShow->append((QString::fromLocal8Bit("灰度大于门限200列宽度均值:		%1").arg(testDataBuff[num].StatsData.t2w_mean)));
	testDrawWidget->statShow->append(QString(" "));

	testDrawWidget->statShow->append((QString::fromLocal8Bit("图像帧灰度最大值:		%1").arg(testDataBuff[num].StatsData.max)));
	testDrawWidget->statShow->append((QString::fromLocal8Bit("图像帧灰度平均值:		%1").arg(testDataBuff[num].StatsData.mean)));
	testDrawWidget->statShow->append((QString::fromLocal8Bit("每列平均值的最大值:		%1").arg(testDataBuff[num].StatsData.mean_max)));
	testDrawWidget->statShow->append((QString::fromLocal8Bit("每列最小值的均值:		%1").arg(testDataBuff[num].StatsData.min_mean)));
	testDrawWidget->statShow->append(QString(" "));

	testDrawWidget->statShow->append((QString::fromLocal8Bit("条纹列数:			%1").arg(testDataBuff[num].StatsData.max_cnt)));
	testDrawWidget->statShow->append((QString::fromLocal8Bit("条纹最大值均值:		%1").arg(testDataBuff[num].StatsData.maxf_mean)));
	testDrawWidget->statShow->append(QString(" "));

	testDrawWidget->statShow->append((QString::fromLocal8Bit("计算的条纹列门限:		%1\n").arg(testDataBuff[num].StatsData.thresh)));
	testDrawWidget->statShow->append(QString(" "));

	testDrawWidget->statShow->append((QString::fromLocal8Bit("编码器累加计数值:		%1").arg(testDataBuff[num].StatsData.encoder_cnt)));

//	testDrawWidget->statShow->append((QString::fromLocal8Bit("条纹列最大值灰度值的和: %1").arg(testDataBuff[num].StatsData.max_sum)));
//	testDrawWidget->statShow->append((QString::fromLocal8Bit("灰度大于门限200的条纹宽度和: %1").arg(testDataBuff[num].StatsData.t2_width)));
//	testDrawWidget->statShow->append((QString::fromLocal8Bit("灰度大于门限150的条纹宽度和: %1").arg(testDataBuff[num].StatsData.t1_width)));

	seriesV->replace(vData);
	seriesGrayscale->replace(grayscaleData);
	
}


void camlib::getBackgroundCenterPoints()
{
	transmit();
	char* sendData = "background request";
	sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr*)&sinAddr, sizeof(sinAddr));

	int optVal = 32 * 1024;		//32K
	setsockopt(serSocket, SOL_SOCKET, SO_RCVBUF, (char*)&optVal, sizeof(int));
	
	int fromlen = sizeof(sinAddr);
	testRecvData recvbuff;
	recvfrom(serSocket, (char*)&recvbuff, sizeof(recvbuff), 0, (sockaddr*)&sinAddr, &fromlen);

	sendData = "background saved";
	sendto(serSocket, sendData, strlen(sendData), 0, (sockaddr*)&sinAddr, sizeof(sinAddr));
	closesocket(serSocket);

	vector<Point2d> background;
	for (int i = 0; i < 1280; i++)
	{
		background.push_back(Point2d(recvbuff.u[i], recvbuff.v[i]));
	}

	FILE* fp = fopen("Xbackground.txt", "wb");
	FILE* fp1 = fopen("Ybackground.txt", "wb");
	for (int i = 0; i < 1280; i++)
	{
		fwrite(&background[i], 1, 8, fp);
		fwrite((double*)&background[i] + 1, 1, 8, fp1);
	}
	fclose(fp);
	fclose(fp1);

	QMessageBox::information(NULL, QString("Background Center"), QString::fromLocal8Bit("背景中心提取已完成"), QMessageBox::Ok);
}

