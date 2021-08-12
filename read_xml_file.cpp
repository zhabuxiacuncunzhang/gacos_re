#include "read_xml_file.h"
#include<string>
#include<iostream>
#include<sstream>
#include"./tinyxml2.h"
#include <stdlib.h>

using namespace tinyxml2;
using namespace std;
int stoe(int a);
double max(double a,double b);
double min(double a,double b);
read_xml_file::read_xml_file()
{

}

void read_xml_file::read_xml_lonlat(string file_path)
{
    XMLDocument doc;
    doc.LoadFile(file_path.c_str());
    XMLElement* root=doc.RootElement();
    XMLElement* edate = root->FirstChildElement("SARParametersofCommonModule")->FirstChildElement("acquisitionDate")->ToElement();

    if(!edate)
    {
        cout<<file_path<<" file open failed"<<endl;
        //return;
		exit(0);
    }else {
        //cout<<element->GetText()<<endl;
    }

    //date
    string date=edate->GetText();
    stringstream line(date);
    line>>this->year;
    line>>this->month;
    line>>this->day;
    line>>this->hour;
    line>>this->minute;

    //LeftTop
    XMLElement* elt = root->FirstChildElement("SARParametersofCommonModule")->FirstChildElement("sceneLeftTopCoord")->ToElement();
    XMLElement* eltlat = elt->FirstChildElement("lat")->ToElement();
    double ltlat=atof(eltlat->GetText());
    XMLElement* eltlon = elt->FirstChildElement("lon")->ToElement();
    double ltlon=atof(eltlon->GetText());

    //RightTop
    XMLElement* ert = root->FirstChildElement("SARParametersofCommonModule")->FirstChildElement("sceneRightTopCoord")->ToElement();
    XMLElement* ertlat = ert->FirstChildElement("lat")->ToElement();
    double rtlat=atof(ertlat->GetText());
    XMLElement* ertlon = ert->FirstChildElement("lon")->ToElement();
    double rtlon=atof(ertlon->GetText());

    //LeftBottom
    XMLElement* elb = root->FirstChildElement("SARParametersofCommonModule")->FirstChildElement("sceneLeftBottomCoord")->ToElement();
    XMLElement* elblat = elb->FirstChildElement("lat")->ToElement();
    double lblat=atof(elblat->GetText());
    XMLElement* elblon = elb->FirstChildElement("lon")->ToElement();
    double lblon=atof(elblon->GetText());

    //RightBottom
    XMLElement* erb = root->FirstChildElement("SARParametersofCommonModule")->FirstChildElement("sceneRightBottomCoord")->ToElement();
    XMLElement* erblat = erb->FirstChildElement("lat")->ToElement();
    double rblat=atof(erblat->GetText());
    XMLElement* erblon = erb->FirstChildElement("lon")->ToElement();
    double rblon=atof(erblon->GetText());

    this->maxlon=max(max(ltlon,lblon),max(rtlon,rblon));
    this->minlon=min(min(ltlon,lblon),min(rtlon,rblon));
    this->maxlat=max(max(ltlat,lblat),max(rtlat,rblat));
    this->minlat=min(min(ltlat,lblat),min(rtlat,rblat));
}

void read_xml_file::read_xml_inf(string file_path)
{
    XMLDocument doc;
    doc.LoadFile(file_path.c_str());
    XMLElement* root=doc.RootElement();
    XMLElement* element = root->FirstChildElement("MTInSARBasicModule")->FirstChildElement("ImageTotalNumber")->ToElement();

    if(!element)
    {
        cout<<file_path<<" file open failed"<<endl;
        //return;
		exit(0);
    }
    //image number
    this->image_number=atoi(element->GetText());
    this->image_date=new int[this->image_number];
    //get image date
    XMLElement* eimage_date = root->FirstChildElement("MTInSARBasicModule")->FirstChildElement("ImageNumber")->ToElement();
    for(int i=0;i<this->image_number;i++)
    {
        image_date[i]=stoe(atoi(eimage_date->FirstChildElement("ImageName")->GetText()));
        eimage_date=eimage_date->NextSiblingElement();
    }
    //xml path
    XMLElement* erslcxmlpath = root->FirstChildElement("MTInSARBasicModule")->FirstChildElement("ImageNumber")->FirstChildElement("RSLCXmlPath")->ToElement();
    this->RSLCXmlPath=erslcxmlpath->GetText();
    //inf number
    XMLElement* einfnumber = root->FirstChildElement("MTInSARIntfModule")->FirstChildElement("IntfTotalNumber")->ToElement();
    this->inf_number=atoi(einfnumber->GetText());
    this->master_date=new int[this->inf_number];
    this->slave_date=new int[this->inf_number];
    //get master-slave date
    XMLElement* einf_date = root->FirstChildElement("MTInSARIntfModule")->FirstChildElement("IntfNumber")->ToElement();
    for(int j=0;j<this->inf_number;j++)
    {
        master_date[j]=stoe(atoi(einf_date->FirstChildElement("MasterDate")->GetText()));
        slave_date[j]=stoe(atoi(einf_date->FirstChildElement("SlaveDate")->GetText()));
        einf_date=einf_date->NextSiblingElement();
    }
}

double max(double a,double b)
{
    if(a>b)
    {
        return a;
    }else{
        return b;
    }
}

double min(double a,double b)
{
    if(a<b)
    {
        return a;
    }else{
        return b;
    }
}


int get_length(int x)
{
    int leng=0;
    while(x)
    {
        x/=10;
        leng++;
    }
    return leng;
}

int stoe(int a)
{
    int b,c;
    if(get_length(a)==8)
    {
        return a;
    }else
    {
        b=a/10;
        c=a%10;
        b=b*100+c;
        return b;
    }
}
