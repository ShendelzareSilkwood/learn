#include <opencv2\opencv.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\types_c.h>
#include <qstring.h>
#include <qstringlist.h>

using namespace cv;
using namespace std;

typedef struct cornor_data
{
    vector<vector<Point2f>> image_points_seq;			//��������
    vector<vector<Point3f>> object_points;
    Size imagesize;
    vector<Mat> image;
    int image_count;
}CorData;

class Pretreat
{
public:
	CorData* result;

public:
	Pretreat();
	~Pretreat();
	CorData* cornor_calculate(QStringList filename_list);
	CorData* cornor_calculate1(QStringList filename_list);
	
};

