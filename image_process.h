#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H

#include<string>
using namespace std;

class image_process
{
public:
    image_process();

	void do_diff(int master_date, int slave_date, string img_path, string out_path);
    void get_point_value(string image_file ,string point_file,string result_file);
	void m_do_diff(string inf_file,string img_path,string out_path);
    string in_path;
    string out_path;




};

#endif // IMAGE_PROCESS_H
