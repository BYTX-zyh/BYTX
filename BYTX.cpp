#include <iostream>
#include <direct.h>
#include <string>
#include <io.h>
#include <windows.h>
#include <stdio.h>
#include <cstring>
#include <fstream>
#define debug(x) cout << #x << '\t' << x << "\n";
#define Test cout << "this is a test line\n";
using namespace std;

string get_pwd();
void init();
bool create_file(string _pwd, string name);
bool create_folder(string _pwd, string name);
bool GetFileTime(HANDLE hFile, LPSTR lpszCreationTime, LPSTR lpszLastAccessTime, LPSTR lpszLastWriteTime);

bool GetFileTime(HANDLE hFile, LPSTR lpszCreationTime, LPSTR lpszLastAccessTime, LPSTR lpszLastWriteTime)
{
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC1, stLocal1, stUTC2, stLocal2, stUTC3, stLocal3;
    // -------->获取 FileTime
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
    {
        return false;
    }
    //---------> 转换: FileTime --> LocalTime
    FileTimeToSystemTime(&ftCreate, &stUTC1);
    FileTimeToSystemTime(&ftAccess, &stUTC2);
    FileTimeToSystemTime(&ftWrite, &stUTC3);

    SystemTimeToTzSpecificLocalTime(NULL, &stUTC1, &stLocal1);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC2, &stLocal2);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC3, &stLocal3);

    // ---------> Show the  date and time.
    // wsprintf(lpszCreationTime, "C:\t%02d/%02d/%d  %02d:%02d",
    //          stLocal1.wDay, stLocal1.wMonth, stLocal1.wYear,
    //          stLocal1.wHour, stLocal1.wMinute);
    // wsprintf(lpszLastAccessTime, "last visit:\t%02d/%02d/%d  %02d:%02d",
    //          stLocal2.wDay, stLocal2.wMonth, stLocal2.wYear,
    //          stLocal2.wHour, stLocal2.wMinute);
    wsprintf(lpszLastWriteTime, "last change time:\t%02d/%02d/%d  %02d:%02d\n",
             stLocal3.wDay, stLocal3.wMonth, stLocal3.wYear,
             stLocal3.wHour, stLocal3.wMinute);
    return true;
}

string get_pwd()
{
    char char_get_pwd[400];
    string s;
    _getcwd(char_get_pwd, sizeof(char_get_pwd));
    s = char_get_pwd;
    return s;
}

bool create_file(string _pwd, string name)
{
    cout << "Create " << name << " at " << _pwd << "\n";
    string _echo_ = "echo >>" + _pwd + "\\" + name;
    char x[10000];
    strcpy(x, _echo_.c_str());
    bool _if_ok = system(x);
    if (_if_ok == 0)
    {
        cout << "Successfully create " << name << " at " << _pwd << endl;
        return true;
    }
    else
        return false;
}

bool create_folder(string _pwd, string name)
{
    cout << "Create " << name << " at " << _pwd << "\n";
    string _make_folder = "md " + _pwd + "\\" + name;
    char x[10000];
    strcpy(x, _make_folder.c_str());
    bool _if_ok = system(x);
    if (_if_ok == 0)
    {
        cout << "Successfully create " << name << " at " << _pwd << endl;
        return true;
    }
    else
        return false;
}

void init()
{
    /**操作解释：
     * _file文件夹保存原始markdown文件
     * _file_html文件夹保存渲染后的文件
     * index.html为主页
     * tag.html为标签索引页
     * 
     * _pwd 路径
     * status 判定system返回
     **/
    bool status;
    string _pwd = get_pwd();

    cout << "Init at " << _pwd << endl;
    status = create_file(_pwd, "index.html");
    if (!status)
    {
        cout << "Init Error\nPlease fix the problem and try again";
        return;
    }
    status = create_file(_pwd, "tag.html");
    if (!status)
    {
        cout << "Init Error\nPlease fix the problem and try again";
        return;
    }
    status = create_folder(_pwd, "_file");
    if (!status)
    {
        cout << "Init Error\nPlease fix the problem and try again";
        return;
    }
    status = create_folder(_pwd, "_file_html");
    if (!status)
    {
        cout << "Init Error\nPlease fix the problem and try again";
        return;
    }
    status = create_file(_pwd, "configure.txt");
    if (!status)
    {
        cout << "Init Error\nPlease fix the problem and try again";
        return;
    }

    cout << "Initialization completed successfully";
    return;
}

void make_new_file(string name)
{
    try
    {
        FILE *_file = freopen("configure.txt", "r", stdin);
        if (_file == NULL)
        {
            throw "Error.Not in the initialized directory.\n";
        }
        freopen("CON", "r", stdin);
        string _pwd=get_pwd();
        _pwd+="\\_file";
        bool status=create_file(_pwd,name);
        if(!status){
            cout<<"Create "<<name<<" Error\n";
        }
    }
    catch (const char * throw_string)
    {
        cout<<throw_string;
        return;
    }
    // freopen("CON", "w", stdout);
}

int main(int argc, char *argv[])
{
    //init 无参数指令 初始化
    string s = argv[1];
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
            cout << throw_string;
            exit(0);
        }
    }
    else if (s == "new")
    {
        try
        {
            if (argc != 3)
            {
                throw "Create new file Error,please input the right instructions.\n";
            }
            string new_file_name = argv[2];
            make_new_file(new_file_name);
        }
        catch (char const *throw_string)
        {
            cout << throw_string;
            exit(0);
        }
    }

    return 0;
}