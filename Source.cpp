#include <iostream>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv\Labeling.h>

using namespace cv;
using namespace std;

int main(int argc, const char **argv) {
	VideoCapture cap(0);

	Mat frm, bg, dst, diff, gry;

	const char th = 10;

	cap >> frm;

	const int height = frm.rows;
	const int width = frm.cols;
	short *dstt = new short[width * height];//ラベリング出力先

	//ラベリング変数
	LabelingBS labeling;
	RegionInfoBS *ri;

	//背景画像を取得 スペースで背景画像取得
	while (waitKey(1) != 32) {
		cap >> frm;
		frm.copyTo(bg);
		imshow("bg", bg);
	}

	while (waitKey(1) != 27){
		cap >> frm;

		absdiff(frm, bg, diff);//背景画像との差分を取得
		cvtColor(diff, gry, CV_BGR2GRAY);
		threshold(gry, dst, th, 255, THRESH_BINARY);


		//ラベリングの実行
		labeling.Exec((uchar *)dst.data, dstt, width, height, true, 30);
		//最大の領域を四角で囲む
		ri = labeling.GetResultRegionInfo(0);
		int ltop_x, ltop_y, rbottom_x, rbottom_y;
		ri->GetMin(ltop_x, ltop_y);
		ri->GetMax(rbottom_x, rbottom_y);
		rectangle(frm, Point(ltop_x, ltop_y), Point(rbottom_x, rbottom_y), Scalar(0, 0, 255));

		imshow("dst", dst);
		imshow("frm", frm);
	}
}