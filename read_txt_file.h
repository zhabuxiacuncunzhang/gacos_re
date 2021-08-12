#ifndef READ_TXT_FILE_H
#define READ_TXT_FILE_H

#include<string>

using namespace std;

class read_txt_file
{
public:
    read_txt_file(string file_name);

    string get_name();
    int get_type();
    string get_path(int type);
    double get_lon_lat(int type);
    int get_is_new_add();
    double get_date();

    void set_name(string name);
    void set_type(int type);
    void set_path(string path,int type);
    void set_lon_lat(double value,int type);
    void set_is_new_add(int flag);
    void set_date(string date);
private:
    string name;
    int type;
    string xml1;
    string xml2;
    string in_txt;
    string out_txt;
    double minlon,minlat,maxlon,maxlat;
    int is_new_add;
    string out_add;
    int point_year;
    int point_month;
    int point_day;
    int point_hour;
    int point_minute;
};

#endif // READ_TXT_FILE_H
