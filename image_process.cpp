#include "image_process.h"
#include<string>
#include<iostream>
#include<fstream>
#include<sstream>
#include <QtMath>
#include"read_point.h"
#include<gdal_priv.h>
using namespace std;
static double Mercator2Lon(double lon);
static double Mercator2Lat(double lat);
image_process::image_process()
{

}

void image_process::do_diff(int master_date, int slave_date, string img_path, string out_path)
{

	img_path.append("\\");
	out_path.append("\\");
	string filepath1 = img_path + to_string(master_date) + ".ztd.tif";
	string filepath2 = img_path + to_string(slave_date) + ".ztd.tif";

	GDALAllRegister(); //注册驱动
	GDALDataset* poSrcDS;//建立数据集读第一张图片
	poSrcDS = (GDALDataset*)GDALOpen(filepath1.c_str(), GA_ReadOnly);//指针指向文件路径
	if (poSrcDS == NULL)
	{
		cout << "open file error!" << endl;
	}

	int	iXSizeDS = poSrcDS->GetRasterXSize();//获取图像的宽
	int iYSizeDS = poSrcDS->GetRasterYSize();//获取图像的高
	int iBandCountDS = poSrcDS->GetRasterCount();//获取图像的波段数

	GDALDataset* poSrcDF;//建立数据集读第二张图片
	poSrcDF = (GDALDataset*)GDALOpen(filepath2.c_str(), GA_ReadOnly);//指针指向文件路径
	if (poSrcDF == NULL)
	{
		cout << "open file error!" << endl;
	}

	int	iXSizeDF = poSrcDF->GetRasterXSize();//获取图像的宽
	int iYSizeDF = poSrcDF->GetRasterYSize();//获取图像的高
	int iBandCountDF = poSrcDF->GetRasterCount();//获取图像的波段数

	GDALDriver* poDriver;
	GDALDataset* poDstDS;
	const char* pszFormat = "GTiff";
	//const char* pszDstFile = "NewPic.tiff";
	string pszDstFile = out_path + to_string(master_date) + "-" + to_string(slave_date) + ".ztd.tif";
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	poDstDS = poDriver->Create(pszDstFile.c_str(), iXSizeDS, iYSizeDS, iBandCountDS, GDT_Float32, NULL);//创建文件用来写入计算后的数据
	double value[6];
	poSrcDF->GetGeoTransform(value);
	poDstDS->SetGeoTransform(value);
	float* pSrcData = new float[iXSizeDS * iYSizeDS];//创建指针存第一张图片的数据
	float* pDstData = new float[iXSizeDS * iYSizeDS];//创建指针存结果图像的数据
	float* pFrcData = new float[iXSizeDS * iYSizeDS];//创建指针存第二张图片的数据


	if (iXSizeDS == iXSizeDF && iYSizeDS == iYSizeDF && iBandCountDS == iBandCountDF)//鲁棒
	{

		//循环波段,按波段来读写图像
		for (int iBand = 1; iBand <= iBandCountDS; iBand++)
		{
			GDALRasterBand* pSrcBandDS = poSrcDS->GetRasterBand(iBand);//获取第一张图片的第iBand个波段
			GDALRasterBand* pSrcBandDF = poSrcDF->GetRasterBand(iBand);//获取第二张图片的第iBand个波段
			GDALRasterBand* pDstBand = poDstDS->GetRasterBand(iBand);
			for (int i = 0; i < iYSizeDS; i++)//循环图像高
			{
				pSrcBandDS->RasterIO(GF_Read, 0, i, iXSizeDS, 1, pSrcData, iXSizeDS, 1, GDT_Float32, 0, 0);
				pSrcBandDF->RasterIO(GF_Read, 0, i, iXSizeDS, 1, pFrcData, iXSizeDS, 1, GDT_Float32, 0, 0);
				for (int j = 0; j < iXSizeDS; j++)//循环图像宽
				{
					pDstData[j] = pSrcData[j] - pFrcData[j];
					pDstBand->RasterIO(GF_Write, 0, i, iXSizeDS, 1, pDstData, iXSizeDS, 1, GDT_Float32, 0, 0);//把数据存入pDstData
				}
			}
		}
	}
	
	GDALClose((GDALDatasetH)poSrcDS);//关闭第一张图片
	GDALClose((GDALDatasetH)poSrcDF);//关闭第二张图片
	GDALClose((GDALDatasetH)poDstDS);//关闭结果图片
	delete[]pSrcData;
	delete[]pFrcData;
	delete[]pDstData;
}
void image_process::get_point_value(string image_file, string point_file, string result_file)
{
	read_point point;
	point.read_point_(point_file);

	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
	GDALDataset* pInDataset1 = (GDALDataset*)GDALOpen(image_file.c_str(), GA_ReadOnly);
	if (pInDataset1 == NULL)
	{
		cout << "无法打开图像！" << endl;
		GDALDestroyDriverManager();
	}
	int nImgWidth = pInDataset1->GetRasterXSize();
	int nImgHeight = pInDataset1->GetRasterYSize();

	double adfGeoTransform[6];
	pInDataset1->GetGeoTransform(adfGeoTransform);

	double lon = 0.0, lat = 0.0;//经度 纬度
	int dx, dy;
	GDALRasterBand* poBand = pInDataset1->GetRasterBand(1);
	float* pData = new float[point.number];

	for (int i = 0; i < point.number; i++)
	{
		lon = point.lon[i];
		lat = point.lat[i];
		dx = (lon - adfGeoTransform[0]) / adfGeoTransform[1];
		dy = (lat - adfGeoTransform[3]) / adfGeoTransform[5];
		if (dx > nImgWidth || dy > nImgHeight)
		{
			cout << "beyond the max" << endl;
			return;
		}
		cout << dx << "  " << dy << endl;
		poBand->RasterIO(GF_Read, dx, dy, 1, 1, (pData + i), 1, 1, GDT_Float32, 0, 0, 0);
	}

	for (int i = 0; i < point.number; i++)
	{
		cout << pData[i] << endl;
	}

	ofstream file(result_file, fstream::out);
	for (int i = 0; i < point.number; i++)
	{
		file << point.name[i] << " " << point.lon[i] << " " << point.lat[i] << " " << pData[i] << endl;
	}
	file.close();

	GDALClose(pInDataset1);
}

static double Mercator2Lon(double lon)//墨卡托转WGS84:经度
{
	return lon / 20037508.34 * 180.0;
}
static double Mercator2Lat(double lat)//墨卡托转WGS84:纬度
{
	double result = 0;
	double mid = lat / 20037508.34 * 180.0;
	result = 180.0 / M_PI * (2.0 * qAtan(qExp(mid*M_PI / 180.0)) - M_PI / 2.0);
	return result;
}
void image_process::m_do_diff(string inf_file, string img_path, string out_path)
{
	fstream file(inf_file);
	if (!file)
	{
		cout << "fail to open the file" << endl;
		return;
	}
	string line;
	int number = 0;
	while (getline(file, line))
	{
		number++;
	}
	file.close();
	int *master_date = new int[number];
	int *slave_date = new int[number];
	fstream file1(inf_file);
	for (int i = 0; i < number; i++)
	{
		getline(file1, line);
		stringstream line1(line);
		line1 >> master_date[i];
		line1 >> slave_date[i];
	}
	file1.close();
	image_process a;
	for (int i = 0; i < number; i++)
	{
		cout << "process " << i + 1 << " inf pair" << endl;
		a.do_diff(master_date[i], slave_date[i], img_path, out_path);
	}
}