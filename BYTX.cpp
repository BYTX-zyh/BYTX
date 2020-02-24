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
void generate();
void get_title();
void watch();
void turn(string file_name);

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
    string now_pwd = get_pwd();
    if (_pwd != now_pwd)
        name = _pwd + "//" + name;
    FILE *file;
    char fileName[1000];
    strcpy(fileName, name.c_str());
    //使用“读入”方式打开文件
    file = fopen(fileName, "r");
    if (file == NULL)
        // 使用“写入”方式创建文件
        file = fopen(fileName, "w");
    //关闭文件
    file = fopen(fileName, "r");
    fclose(file);
    if (file != NULL)
        return true;
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
    status = create_file("_file", "file.txt");
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

    FILE *_file = freopen("configure.txt", "r", stdin);
    if (_file == NULL)
    {
        cout << "Error.Not in the initialized directory.\n";
        return;
    }
    freopen("CON", "r", stdin);
    if (name[name.size() - 1] != 'd' || name[name.size() - 2] != 'm' || name[name.size() - 3] != '.')
    {
        cout << "Error.Please create a markdown file\n";
        return;
    }
    bool status = create_file("_file", name);
    if (!status)
    {
        cout << "Create new file Error\n";
        return;
    }
    _file = freopen("_file\\file.txt", "r", stdin);
    freopen("CON", "r", stdin);
    if (_file == NULL)
    {
        cout << "Error._file\\file.txt does not exist.\n";
        return;
    }
    ofstream fout("_file\\file.txt", ios::app);
    fout << name << '\n';
    fout.close();
    cout << "add new file name in file.txt" << endl;
    return;
}

void generate()
{
}

void watch()
{
    FILE *_file = freopen("configure.txt", "r", stdin);
    freopen("CON", "r", stdin);
    if (_file == NULL)
    {
        cout << "Error.Not in the initialized directory.\n";
        return;
    }
    // _file = freopen("_file\\file.txt", "r", stdin);
    // if (_file == NULL)
    // {
    //     cout << "Error.file.txt does not exist.\n";
    //     return;
    // }
    // else {
    //     char xxxx[10000];
    //     while(gets(xxxx)){
    //          cout<<xxxx<<endl;
    //     }

    // }
    // freopen("CON", "r", stdin);
}

void turn(string file_name)
{
    /**
     *file_name:xxx.md 
     *保证此时文件存在且需要渲染
     * 
     **/

    string file_file_name="_file\\"+file_name;
    string html_file_name="_file_html\\"+file_name;
    char _file_file_name[100],_html_file_name[100];

    strcpy(_file_file_name,file_file_name.c_str());
    html_file_name=html_file_name.substr(0,html_file_name.size()-3)+".html";
    strcpy(_html_file_name,html_file_name.c_str());
    cout<<_html_file_name<<endl;

    freopen(_file_file_name, "r", stdin);
    freopen(_html_file_name, "w", stdout);
}

void get_title(string s)
{
    int cnt = 0;
    for (int i = 0; i < 6; i++)
        if (s[i] == '#')
            cnt++;
    cout << "\t<h" << cnt << ">";
    for (int i = cnt; i < s.size(); i++)
        cout << s[i];
    cout << "</h" << cnt << ">\n";
}

int main(int argc, char *argv[])
{
    //init 无参数指令 初始化
    string s = argv[1];
    if (s == "init")
    {

        if (argc != 2)
        {
            throw "Init Error,please input the right instructions.\nMaybe you want \"init\"?\n";
            return 0;
        }
        init();
    }
    else if (s == "new")
    {

        if (argc != 3)
        {
            cout << "Create new file Error,please input the right instructions.\n";
            return 0;
        }
        string new_file_name = argv[2];
        make_new_file(new_file_name);
    }
    else if (s == "generate")
    {

        if (argc != 2)
        {
            cout << "Error instruction\n";
            return 0;
        }
        generate();
    }
    else if (s == "watch")
    {
        watch();
    }
    else if(s=="turn"){
        turn("3.md");
    }
    // system("pause");
    return 0;
}