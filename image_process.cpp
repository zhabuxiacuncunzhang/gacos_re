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

	GDALAllRegister(); //ע������
	GDALDataset* poSrcDS;//�������ݼ�����һ��ͼƬ
	poSrcDS = (GDALDataset*)GDALOpen(filepath1.c_str(), GA_ReadOnly);//ָ��ָ���ļ�·��
	if (poSrcDS == NULL)
	{
		cout << "open file error!" << endl;
	}

	int	iXSizeDS = poSrcDS->GetRasterXSize();//��ȡͼ��Ŀ�
	int iYSizeDS = poSrcDS->GetRasterYSize();//��ȡͼ��ĸ�
	int iBandCountDS = poSrcDS->GetRasterCount();//��ȡͼ��Ĳ�����

	GDALDataset* poSrcDF;//�������ݼ����ڶ���ͼƬ
	poSrcDF = (GDALDataset*)GDALOpen(filepath2.c_str(), GA_ReadOnly);//ָ��ָ���ļ�·��
	if (poSrcDF == NULL)
	{
		cout << "open file error!" << endl;
	}

	int	iXSizeDF = poSrcDF->GetRasterXSize();//��ȡͼ��Ŀ�
	int iYSizeDF = poSrcDF->GetRasterYSize();//��ȡͼ��ĸ�
	int iBandCountDF = poSrcDF->GetRasterCount();//��ȡͼ��Ĳ�����

	GDALDriver* poDriver;
	GDALDataset* poDstDS;
	const char* pszFormat = "GTiff";
	//const char* pszDstFile = "NewPic.tiff";
	string pszDstFile = out_path + to_string(master_date) + "-" + to_string(slave_date) + ".ztd.tif";
	poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);
	poDstDS = poDriver->Create(pszDstFile.c_str(), iXSizeDS, iYSizeDS, iBandCountDS, GDT_Float32, NULL);//�����ļ�����д�����������
	double value[6];
	poSrcDF->GetGeoTransform(value);
	poDstDS->SetGeoTransform(value);
	float* pSrcData = new float[iXSizeDS * iYSizeDS];//����ָ����һ��ͼƬ������
	float* pDstData = new float[iXSizeDS * iYSizeDS];//����ָ�����ͼ�������
	float* pFrcData = new float[iXSizeDS * iYSizeDS];//����ָ���ڶ���ͼƬ������


	if (iXSizeDS == iXSizeDF && iYSizeDS == iYSizeDF && iBandCountDS == iBandCountDF)//³��
	{

		//ѭ������,����������дͼ��
		for (int iBand = 1; iBand <= iBandCountDS; iBand++)
		{
			GDALRasterBand* pSrcBandDS = poSrcDS->GetRasterBand(iBand);//��ȡ��һ��ͼƬ�ĵ�iBand������
			GDALRasterBand* pSrcBandDF = poSrcDF->GetRasterBand(iBand);//��ȡ�ڶ���ͼƬ�ĵ�iBand������
			GDALRasterBand* pDstBand = poDstDS->GetRasterBand(iBand);
			for (int i = 0; i < iYSizeDS; i++)//ѭ��ͼ���
			{
				pSrcBandDS->RasterIO(GF_Read, 0, i, iXSizeDS, 1, pSrcData, iXSizeDS, 1, GDT_Float32, 0, 0);
				pSrcBandDF->RasterIO(GF_Read, 0, i, iXSizeDS, 1, pFrcData, iXSizeDS, 1, GDT_Float32, 0, 0);
				for (int j = 0; j < iXSizeDS; j++)//ѭ��ͼ���
				{
					pDstData[j] = pSrcData[j] - pFrcData[j];
					pDstBand->RasterIO(GF_Write, 0, i, iXSizeDS, 1, pDstData, iXSizeDS, 1, GDT_Float32, 0, 0);//�����ݴ���pDstData
				}
			}
		}
	}
	
	GDALClose((GDALDatasetH)poSrcDS);//�رյ�һ��ͼƬ
	GDALClose((GDALDatasetH)poSrcDF);//�رյڶ���ͼƬ
	GDALClose((GDALDatasetH)poDstDS);//�رս��ͼƬ
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
		cout << "�޷���ͼ��" << endl;
		GDALDestroyDriverManager();
	}
	int nImgWidth = pInDataset1->GetRasterXSize();
	int nImgHeight = pInDataset1->GetRasterYSize();

	double adfGeoTransform[6];
	pInDataset1->GetGeoTransform(adfGeoTransform);

	double lon = 0.0, lat = 0.0;//���� γ��
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

static double Mercator2Lon(double lon)//ī����תWGS84:����
{
	return lon / 20037508.34 * 180.0;
}
static double Mercator2Lat(double lat)//ī����תWGS84:γ��
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