#ifndef READ_POINT_H
#define READ_POINT_H

#include<string>


using namespace std;

class read_point
{
public:
    void read_point_(string file_path);
    int number;
    string *name;
    double *lon;
    double *lat;
};

#endif // READ_POINT_H
