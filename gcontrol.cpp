#include"ftp.h"
#include"image_process.h"
#include"read_point.h"
#include"read_txt_file.h"
#include"read_xml_file.h"
#include<iostream>
#include<string>
#include<fstream>
#include <iomanip>
#include <ctime>
#define random(a,b) (((double)rand()/RAND_MAX)*(b-a)+a)
using namespace std;
using namespace tinyxml2;
double max(double a, double b);
double min(double a, double b);
inline bool exists_file(const std::string& name);
string& replace_all_distinct(string& str, const string& old_value, const string& new_value);
void gacos_re(string txt_file_path)
{
	int type;
	read_txt_file txt_file(txt_file_path);
	type=txt_file.get_type();
	string running_path = _pgmptr;
	string winrar_path = "WinRAR.exe";
	winrar_path = running_path.substr(0, running_path.find_last_of("\\") + 1) + winrar_path;
	int idf = random(1000000, 99999999);
	string pro_time = to_string(time(0));
	//string afile_name = "control-file-" + to_string(idf) + "-" + ".txt";
	string afile_name = "API-gacos02-V"  + to_string(idf) + "-" + pro_time + ".txt";
	string afile_name_finish = afile_name + ".finish";
	ofstream afile(afile_name, fstream::out);
	string bfile_name = "inf-file-" + to_string(idf) + "-" + ".txt";
	//生成请求文件
	//point 
	double date;
	int yyyymmdd;
	//single
	read_xml_file single;
	//dinsar
	read_xml_file dinsar1, dinsar2;
	//mtinsar
	read_xml_file mtinsar, rslc;
	try
	{
		//
		txt_file.set_name(txt_file.get_name() + "-"+pro_time);
		afile << txt_file.get_name() << endl;
		if (type==0)
		{
			afile << txt_file.get_lon_lat(1) << " " << txt_file.get_lon_lat(2) << " " << txt_file.get_lon_lat(0) << " " << txt_file.get_lon_lat(3) << endl;
			date = txt_file.get_date();
			yyyymmdd=date/100;
			double time=date-yyyymmdd*100;
			afile << setiosflags(ios::fixed) << setprecision(2) << time << " " << 2 << endl;
			afile << yyyymmdd << endl;
		}
		else if (type==1)
		{
			
			single.read_xml_lonlat(txt_file.get_path(0));
			afile << single.minlat << " " << single.maxlat << " " << single.minlon << " " << single.maxlon << endl;
			afile << single.hour + (double)single.minute / 60 << " " << 2 << endl;
			afile << single.year * 10000 + single.month * 100 + single.day << endl;
		}
		else if (type==2)
		{
			dinsar1.read_xml_lonlat(txt_file.get_path(0));
			dinsar2.read_xml_lonlat(txt_file.get_path(1));
			double minlat, maxlat, minlon, maxlon;
			minlat = min(dinsar1.minlat, dinsar2.minlat);
			maxlat = max(dinsar1.maxlat, dinsar2.maxlat);
			minlon = min(dinsar1.minlon, dinsar2.minlon);
			maxlon = max(dinsar1.maxlon, dinsar2.maxlon);
			afile << minlat << " " << maxlat << " " << minlon << " " << maxlon << endl;
			afile << dinsar1.hour + (double)dinsar1.minute / 60 << " " << 2 << endl;
			afile << dinsar1.year * 10000 + dinsar1.month * 100 + dinsar1.day << endl;
			afile << dinsar2.year * 10000 + dinsar2.month * 100 + dinsar2.day << endl;
		}
		else if (type==3)
		{
			mtinsar.read_xml_inf(txt_file.get_path(0));
			rslc.read_xml_lonlat(mtinsar.RSLCXmlPath);
			afile << rslc.minlat << " " << rslc.maxlat << " " << " " << rslc.minlon << " " << rslc.maxlon << endl;
			afile << rslc.hour + (double)rslc.minute / 60 << " " << 2 << endl;
			for (int i = 0; i < mtinsar.image_number; i++)
			{
				afile << mtinsar.image_date[i] << endl;
			}
			
			ofstream bfile(bfile_name, fstream::out);
			for (int j = 0; j < mtinsar.inf_number; j++)
			{
				bfile << mtinsar.master_date[j] << " " << mtinsar.slave_date[j] << endl;
			}
			bfile.close();
		}
		else
		{
			throw "error input type";
		}
		afile.close();
		ofstream cfile(afile_name_finish, fstream::out);
		cfile << "finished!" << endl;
		cfile.close();
	}
	catch (char *str)
	{
		std::cout << str << endl;
	}
	//上传下载数据并解压
	try
	{
		ftp ftpp;
		ftpp.set_moudle("put");
		ftpp.set_file(afile_name);
		ftpp.do_ftp();

		ftp ftpg;
		ftpg.set_moudle("get");
		while (1)
		{
			if (exists_file(txt_file.get_name() + ".tar.gz.finish"))
			{
				ftpg.set_file(txt_file.get_name()+".tar.gz");
				ftpg.do_ftp();
				break;
			}
			_sleep(10000);
			ftpg.set_file(txt_file.get_name() + ".tar.gz.finish");
			ftpg.do_ftp();
		}
		remove(afile_name.c_str());
		remove(afile_name_finish.c_str());
		string mvfile1 = "md C:\\gacos_temp\\";
		cout << mvfile1 << endl;
		std::system(mvfile1.c_str());
		mvfile1 = "move .\\" + txt_file.get_name() + ".tar.gz C:\\gacos_temp\\";
		cout << mvfile1 << endl;
		std::system(mvfile1.c_str());
		mvfile1 = "del /F /S /Q .\\" + txt_file.get_name() + ".tar.gz.finish";
		cout << mvfile1 << endl;
		std::system(mvfile1.c_str());
		string unzip;
		unzip = "\""+winrar_path + "\"" + " x C:\\gacos_temp\\" + txt_file.get_name() + ".tar.gz C:\\gacos_temp\\" + txt_file.get_name() + "\\";
		std::cout << unzip << endl;
		std::system(unzip.c_str());

	}
	catch (const std::exception&)
	{

	}
	//运算
	try
	{
		image_process a;
		string out_path;
		/*if (type!=0)
		{
			if (txt_file.get_is_new_add())
			{
				out_path = txt_file.get_path(4);
			}
			else
			{
				out_path = txt_file.get_path(0).substr(0, txt_file.get_path(0).find_last_of("\\") + 1);
				//cout << out_path << endl;
			}
		}
		else
		{
			out_path = txt_file.get_path(2).substr(0, txt_file.get_path(2).find_last_of("\\") + 1);
		}
		
		if (type == 0)
		{
			string img_file;
			img_file = "C:\\gacos_temp\\" + txt_file.get_name() + "\\" + to_string(yyyymmdd) + ".ztd.tif";
			a.get_point_value(img_file, txt_file.get_path(2), txt_file.get_path(2).substr(0, txt_file.get_path(2).find_last_of(".")) + "_atmoscorrect.txt");
		}*/

		if (txt_file.get_is_new_add())
		{
			out_path = txt_file.get_path(4);
			out_path = replace_all_distinct(out_path, "/", "\\");
		}
		else
		{
			if (type==0)
			{
				out_path = txt_file.get_path(2);
			}
			else {
				out_path = txt_file.get_path(0);
			}
			out_path = replace_all_distinct(out_path, "/", "\\");
			out_path = out_path.substr(0, out_path.find_last_of("\\") + 1);
		}
		if (type == 0)
		{
			string img_file;
			img_file = "C:\\gacos_temp\\" + txt_file.get_name() + "\\" + to_string(yyyymmdd) + ".ztd.tif";
			string filename_2 = txt_file.get_path(2).substr(0, txt_file.get_path(2).find_last_of("."));
			filename_2 = replace_all_distinct(filename_2, "/", "\\");
			filename_2 = filename_2.substr(filename_2.find_last_of("\\") + 1, filename_2.size());
			a.get_point_value(img_file, txt_file.get_path(2), out_path + filename_2 + "_atmoscorrect.txt");
		}
		else if (type == 1)
		{
			string copyfile = "copy C:\\gacos_temp\\" + txt_file.get_name() + "\\" + to_string(single.year * 10000 + single.month * 100 + single.day) + ".ztd.tif " + out_path;
			cout << copyfile << endl;
			std::system(copyfile.c_str());
		}
		else if (type == 2)
		{
			a.do_diff(dinsar1.year * 10000 + dinsar1.month * 100 + dinsar1.day, dinsar2.year * 10000 + dinsar2.month * 100 + dinsar2.day, "C:\\gacos_temp\\" + txt_file.get_name() + "\\" ,out_path);
		}
		else if (type == 3)
		{
			a.m_do_diff(bfile_name, "C:\\gacos_temp\\" + txt_file.get_name() + "\\", out_path);
		}
		string mvfile = "move C:\\gacos_temp\\" + txt_file.get_name() + ".tar.gz \"" + out_path + "\"";
		cout << mvfile << endl;
		std::system(mvfile.c_str());
		/*mvfile = "move C:\\gacos_temp\\" + txt_file.get_name() + " " + "\"" + out_path + "\"";
		cout << mvfile << endl;
		std::system(mvfile.c_str());*/
		mvfile = "del /F /S /Q .\\" + afile_name_finish;
		std::system(mvfile.c_str());
		mvfile = "del /F /S /Q .\\" + afile_name;
		std::system(mvfile.c_str());
		mvfile = "del /F /S /Q .\\" + bfile_name;
		std::system(mvfile.c_str());
		mvfile = "rd /s /q C:\\gacos_temp\\";
		cout << mvfile << endl;
		std::system(mvfile.c_str());
	}
	catch (const std::exception&)
	{

	}
}
inline bool exists_file(const std::string& name) {
	struct stat buffer;
	return (stat((char *)name.c_str(), &buffer) == 0);
}

string& replace_all_distinct(string& str, const string& old_value, const string& new_value)
{
	string::size_type pos = 0;
	while ((pos = str.find(old_value, pos)) != string::npos)
	{
		str = str.replace(pos, old_value.length(), new_value);
		if (new_value.length() > 0)
		{
			pos += new_value.length();
		}
	}
	return str;

}