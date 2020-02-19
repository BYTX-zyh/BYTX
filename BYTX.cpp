/**
 * author:BYTX
 * 
 * 
 * */
#include <iostream>
#include <direct.h>

#define Test cout << "this is a test line\n"

using namespace std;

string get_pwd();


string get_pwd()
{
	char char_get_pwd[256];
    string s;
	_getcwd(char_get_pwd, sizeof(char_get_pwd));
	s=char_get_pwd;
    return s;
}

void init()
{
    string _pwd=get_pwd();
    cout<<_pwd<<endl;
    // system("md _file");
}
int main(int argc, char *argv[])
{
    string s = argv[1];
    cout<<argc<<endl;
    //init 无参数指令 初始化
    if (s == "init")
    {
        try
        {
            if(argc!=2) 
                throw "Init Error,please input the right instructions.\nMaybe you want \"init\"?\n";
            init();
        }
        catch (char const * throw_string)
        {
            cout<<throw_string<<endl;
            exit(0);
        }
    }
    // cout<<"test"<<endl;

    return 0;
}