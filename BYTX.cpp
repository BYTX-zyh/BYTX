/**
 * author:BYTX
 * 
 * 
 * */
#include <iostream>
#include <direct.h>
#include <string>
#include <cstring>
#define debug(x) cout << #x << '\t' << x << "\n";
#define Test cout << "this is a test line\n";
using namespace std;

string get_pwd();
void init();
bool create_file(string _file_name);

string get_pwd()
{
    char char_get_pwd[400];
    string s;
    _getcwd(char_get_pwd, sizeof(char_get_pwd));
    s = char_get_pwd;
    return s;
}

bool create_file(string _pwd, string _file_name)
{
    cout << "Create " << _file_name << " at " << _pwd << "\n";
    string make_new_file="echo >>"+_pwd+"\\"+_file_name;
    char * _make_new_file=strcpy(_make_new_file,make_new_file.c_str());
    int status = system(_make_new_file);
    if (status != 0)
    {
        cout << "Init Error\nPlease fix the problem and try again";
        return false;
    }
    cout << "Successfully create index.html\n";
    return true;
}

bool create_folder(string _pwd,string _folder_name){
    cout<<"Create "<<_folder_name<<" at "<<_pwd<<endl;
    
}

void init()
{
    /**操作解释：
     * _file文件夹保存原始markdown文件
     * _file_html文件夹保存渲染后的文件
     * index.html为主页
     * tag.html为标签索引页
     * 
     * var：
     * _pwd 路径
     * status 判定system返回值
     **/
    int status;
    string _pwd = get_pwd();

    cout << "Init at " << _pwd << endl;

    create_file(_pwd,"index.html");
    cout<<"dfsdfdfdfd"<<endl;
    Test
    const char *_make_file = "md _file";
    cout << "Create _file folder\n";
    status = system(_make_file);
    if (status != 0)
    {
        cout << "Init Error\nPlease fix the problem and try again";
        return;
    }
    cout << "Successfully create _file folder\n";

    const char *_make_file_html = "md _file_html";
    
    status = system(_make_file_html);
    if (status != 0)
    {
        cout << "Init Error\nPlease fix the problem and try again";
        return;
    }

    cout << "Create index.html\n";
    status = system("echo >>index.html");
    if (status != 0)
    {
        cout << "Init Error\nPlease fix the problem and try again";
        return;
    }
    cout << "Successfully create index.html\n";

    // string create_file = "echo >>" + _pwd + "\\_file\\d.txt";
    // char c[100];
    // strcpy(c, create_file.c_str());
    // status = system("echo >>index.html");
    //  if(status!=0){
    //     cout<<"Init Error\nPlease fix the problem and try again";
    //     return;
    // }
    // status =system(c);
    //  if(status!=0){
    //     cout<<"Init Error\nPlease fix the problem and try again";
    //     return ;
    // }
    cout << "Initialization completed successfully";
    return;
}
int main(int argc, char *argv[])
{
    string s = argv[1];
    //init 无参数指令 初始化
    // init();
    // system("pause");
    if (s == "init")
    {
        try
        {
            if (argc != 2)
                throw "Init Error,please input the right instructions.\nMaybe you want \"init\"?\n";
            init();
        }
        catch (char const *throw_string)
        {
            cout << throw_string << endl;
            exit(0);
        }
    }
    // cout<<"test"<<endl;
    return 0;
}