#ifndef FTP_H
#define FTP_H
#include<string>
using namespace std;

class ftp
{
public:
    ftp();

    void do_ftp();
    void set_file(string file);
    void set_moudle(string type);

    string get_username();
    string get_pass();
    string get_file();
    string get_moudle();
private:
    string host       =     "";
    string username   =     "";
    string pass       =     "";
    string file       ;
    string moudle     ;
};

#endif // FTP_H
