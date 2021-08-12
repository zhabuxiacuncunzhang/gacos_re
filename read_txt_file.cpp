#include "read_txt_file.h"
#include<iostream>
#include<fstream>
#include <sstream>
#include<string>

//new object
read_txt_file::read_txt_file(string file_name)
{
    fstream txt_file(file_name);
    if(!txt_file)
    {
        cout << "fail to open the file" <<endl;
        //return ;
		exit(0);
    }
	
    string line;
    getline(txt_file, line);//line 1
    getline(txt_file, line);//line 2
        stringstream line2(line);
        string x;
        line2>>x;
        line2>>x;
        set_name(x);
    getline(txt_file, line);//line 3
        stringstream line3(line);
        line3>>x;
        line3>>x;
        set_type(stoi(x));
    getline(txt_file, line);//line 4
        stringstream line4(line);
        line4>>x;
        line4>>x;
        set_path(x,0);
    getline(txt_file, line);//line 5
        stringstream line5(line);
        line5>>x;
        line5>>x;
        set_path(x,1);
    getline(txt_file, line);//line 6
        stringstream line6(line);
        line6>>x;
        line6>>x;
        set_path(x,2); 
	if (this->get_type()==0)
	{
		getline(txt_file, line);//line 7
			stringstream line7(line);
			line7>>x;
			line7 >> x; 
			set_lon_lat(stod(x),3); 
		getline(txt_file, line);//line 8
			stringstream line8(line);
			line8>>x;
			line8>>x;
			set_lon_lat(stod(x),0);
		getline(txt_file, line);//line 9
			stringstream line9(line);
			line9>>x; 
			line9>>x;
			set_lon_lat(stod(x),1);
		getline(txt_file, line);//line 10
			stringstream line10(line);
			line10>>x;
			line10>>x;
			set_lon_lat(stod(x),2);
		getline(txt_file, line);//line 11
			stringstream line11(line);
			line11>>x;
			line11>>x;
			set_date(x);
	}
	else
	{
		getline(txt_file, line);//line 7
		stringstream line7(line);
		getline(txt_file, line);//line 8
		stringstream line8(line);
		getline(txt_file, line);//line 9
		stringstream line9(line);
		getline(txt_file, line);//line 10
		stringstream line10(line);
		getline(txt_file, line);//line 11
		stringstream line11(line);
	}

    /*getline(txt_file, line);//line 12
        stringstream line12(line);
        line12>>x;
        line12>>x;
        set_path(x,3);*/
    getline(txt_file, line);//line 13
        stringstream line13(line);
        line13>>x;
        line13>>x;
        set_is_new_add(stoi(x));
    getline(txt_file, line);//line 14
        stringstream line14(line);
        line14>>x;
        line14>>x;
        set_path(x,4);
}
//get function
string read_txt_file::get_name()
{
    return this->name;
}

int read_txt_file::get_type()
{
    return this->type;
}
string read_txt_file::get_path(int type)
{
    string path;
    if(type==0)
    {
        path=this->xml1;
    }
    else if(type==1)
    {
        path=this->xml2;
    }
    else if(type==2)
    {
        path=this->in_txt;
    }
    else if(type==3)
    {
        path=this->out_txt;
    }
    else if(type==4)
    {
        path=this->out_add;
    }
    return path;
}
double read_txt_file::get_lon_lat(int type)
{
    double value=0.0;
    if(type==0)
    {
        value=this->minlon;
    }
    else if(type==1)
    {
        value=this->minlat;
    }
    else if(type==2)
    {
        value=this->maxlat;
    }
    else if(type==3)
    {
        value=this->maxlon;
    }
    return value;
}
int read_txt_file::get_is_new_add()
{
    return this->is_new_add;
}
double read_txt_file::get_date()
{
    double time=this->point_hour+this->point_minute/60.0;
    int yyyymmdd=this->point_year*10000+this->point_month*100+this->point_day;
    double date=yyyymmdd*100.0+time;
    return date;
}

//set function
void read_txt_file::set_name(string name)
{
    this->name=name;
}
void read_txt_file::set_type(int type)
{
    this->type=type;
}
void read_txt_file::set_path(string path,int type)
{
    path=path.substr(1,path.length()-2);
    if(type==0)
    {
        this->xml1=path;
    }
    else if(type==1)
    {
        this->xml2=path;
    }
    else if(type==2)
    {
        this->in_txt=path;
    }
    else if(type==3)
    {
        this->out_txt=path;
    }
    else if(type==4)
    {
        this->out_add=path;
    }
}
void read_txt_file::set_lon_lat(double value,int type)
{
    if(type==0)
    {
        this->minlon=value;
    }
    else if(type==1)
    {
        this->minlat=value;
    }
    else if(type==2)
    {
        this->maxlat=value;
    }
    else if(type==3)
    {
        this->maxlon=value;
    }
}
void read_txt_file::set_is_new_add(int flag)
{
    this->is_new_add=flag;
}
void read_txt_file::set_date(string date)
{
    this->point_year=stoi(date.substr(0,4));
    this->point_month=stoi(date.substr(5,2));
    this->point_day=stoi(date.substr(8,2));
    this->point_hour=stoi(date.substr(11,2));
    this->point_minute=stoi(date.substr(14,2));
}
