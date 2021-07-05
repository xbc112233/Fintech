#include <iostream>
#include <sstream>
#include <fstream>
#include <dirent.h>
#include "common_util.h"
using namespace std;

int CommonUtil::InsertByLineNumber(const std::string &filePath, const std::string &insertContent, int insertLine)
{
    if (filePath == "" || insertContent == "" || insertLine < 0)
    {
        return FAIL;
    }
    ifstream is(filePath);
    if (!is.is_open())
    {
        opendir(filePath.c_str());
        ////cout << "DEBUG opendir filePath: " << filePath <<endl;
        ofstream os(filePath);
        os << insertContent;
        os.close();
        ////cout << "DEBUG insertContent: " << insertContent << " insertLine: " <<  insertLine << endl; 
        return SUCC;
    }
    string tmp;
    string buffer;
    int count = 0;
    //cout << "DEBUG insertContent: " << insertContent << " insertLine: " <<  insertLine << endl; 
    bool f = false;
    while (getline(is,tmp))
    {
        count++;
        buffer += tmp;
        buffer += "\n";
        if (count == insertLine)
        {
            buffer += insertContent;
            buffer += "\n";
            f = true;
        }
        //cout << "DEBUG buffer: " << buffer<<endl;
    }
    if (!f) {
        buffer += insertContent;
        buffer += "\n";
    }
    is.close();
    string tmpPath = filePath + ".tmp";
    opendir(tmpPath.c_str());
    ofstream os(tmpPath);
    //cout <<"DEBUG ofstream tmpPath buffer: " << buffer <<endl;
    os << buffer;
    os.close();
    rename(tmpPath.c_str(), filePath.c_str());
    return SUCC;
}
