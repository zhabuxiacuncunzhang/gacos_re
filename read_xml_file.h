#ifndef READ_XML_FILE_H
#define READ_XML_FILE_H

#include<string>
#include"tinyxml2.h"
using namespace std;
using namespace tinyxml2;

class read_xml_file
{
public:
    read_xml_file();
    void read_xml_lonlat(string file_path);
    void read_xml_inf(string file_path);

    int year;
    int month;
    int day;
    int hour;
    int minute;
    double minlon,minlat,maxlon,maxlat;


    int image_number;//mtinsar
    int *image_date;//mtinsar
    int inf_number;
    int *master_date;//mtinsar
    int *slave_date;//mtinsar
    string RSLCXmlPath;
};

#endif // READ_XML_FILE_H
