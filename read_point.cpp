#include "read_point.h"
#include<fstream>
#include<iostream>
#include<sstream>
void read_point::read_point_(string file_path)
{
    fstream file(file_path);
    if(!file)
    {
        cout << "fail to open the file" <<endl;
        //return ;
		exit(0);
    }
    string line;
    int number=0;
    while (getline(file, line))
    {
        number++;
    }
    this->number=number;
    file.close();
    this->name=new string[number];
    this->lat=new double[number];
    this->lon=new double[number];
    fstream file1(file_path);
    for(int i=0;i<number;i++)
    {
        getline(file1, line);
        stringstream line1(line);
        line1>>this->name[i];
        line1>>this->lon[i];
        line1>>this->lat[i];
    }
}
