#include "ftp.h"
#include<iostream>
#include<fstream>
using namespace std;
#define random(a,b) (((double)rand()/RAND_MAX)*(b-a)+a)
#if defined (WIN32)||defined (_WIN32)|| defined(WIN64)||defined(_WIN64)&& !defined(__CYGWIN__)
ftp::ftp()
{
    cout<<"this is windows!"<<endl;
}
void ftp::do_ftp()
{
    int idf=random(1000000, 99999999);
    string afile_name = "A_file-" + this->username + "-" + to_string(idf) + "-" +".txt" ;
    ofstream afile(afile_name, fstream::out);
    afile<<"open "<<this->host<<endl;
    afile<<"user "<<this->username<<" "<<this->pass<<endl;
    afile<<"binary"<<endl<<"prompt n"<<endl;
    if (this->moudle=="put")
    {
        afile << "cd " << this->username << "/inward" << endl;
        afile << "mput " << this->file << endl;
        afile << "mput " << this->file << ".finish" << endl;
    }else if(this->moudle=="get"){
        afile << "cd " << this->username << "/outward" << endl;
        afile << "mget " << this->file << endl;
    }
    afile<<"quit";
    afile.close();

    string bfile_name = "B_file-" + this->username + "-" + to_string(idf) + "-" +".bat" ;
    ofstream bfile(bfile_name, fstream::out);
    bfile<<"ftp.exe -n -s:"<<afile_name<<endl;
	bfile.close();
    string get_net;
    //get_net = "netsh advfirewall firewall add rule name = "+ to_string(idf) +" dir = out action = allow  program = "+ bfile_name +" enable = yes";
    //system(get_net.c_str());
    
    system(bfile_name.c_str());

	remove(afile_name.c_str());
	remove(bfile_name.c_str());
}

#else
ftp::ftp()
{
cout<<"this is linux!"<<endl;
}

void ftp::do_ftp(string username,string pass,string file,string moudle)
{

}
#endif


void ftp::set_file(string file)
{
    this->file=file;
}
void ftp::set_moudle(string type)
{
    this->moudle=type;
}

string ftp::get_username()
{
    return this->username;
}
string ftp::get_pass()
{
    return this->pass;
}
string ftp::get_file()
{
    return this->file;
}
string ftp::get_moudle()
{
    return this->moudle;
}
