#include <iostream>
#include <direct.h>
#include <dirent.h>
#include <string>
#include <io.h>
#include <windows.h>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <vector>
#define debug(x) cout << #x << '\t' << x << "\n";
#define Test cout << "this is a test line\n";
using namespace std;

/**
 * task means that the last line is a task
 * normal means that not need such as </ul>
 * */
string status = "normal";

//get the pwd
string get_pwd();

//init at the file
void init();
//
void generate();
//check which need turn
void watch();
//turn markdown to HTML
void turn(string file_name);
//create file and check if at the file inited
bool create_file(string _pwd, string name);
//create folder and check if at the file inited
bool create_folder(string _pwd, string name);
//get the time about the file
bool GetFileTime(HANDLE hFile, LPSTR lpszCreationTime, LPSTR lpszLastAccessTime, LPSTR lpszLastWriteTime);
//check a foleder if exist
bool check_folder_if_exist(string folder_name);

//check if title
bool check_if_title(string s);
//get a title
void get_title(string s);

//check if task and return the first position of the task
bool check_if_task(string s, int &pos);
//get a task and turn to HTML
void get_task(string s, int pos);

//check if need print a <hr />
bool check_hr(string s);
//need a <hr/>
void get_hr(string s);

//change status and print some code such as <\ul>
void change_status(string next_status);

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
    if (file != NULL)
    {
        cout << "Error.this file already exist.\n";
        cout << "Would you want remake this file?\n";
        cout << "Please input your chose:[y/n]:";
        char chose;
        while (cin >> chose)
        {
            if (chose == 'y' || chose == 'Y')
            {
                break;
            }
            else if (chose == 'n' || chose == 'N')
            {
                return false;
            }
            else
            {
                cout << "Error.Please input your chose:[y/n]:";
            }
        }
    }
    if (file == NULL)
        // 使用“写入”方式创建文件
        file = fopen(fileName, "w");
    //关闭文件
    // file = fopen(fileName, "r");
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

    status=check_folder_if_exist(_pwd);
    if(status){
        cout<<"Error.This folder is not empty.\n";
        return;
    }
    cout << "Start init at " << _pwd << endl;

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

bool check_folder_if_exist(string folder_name)
{
    char _folder_name[1000];
    strcpy(_folder_name,folder_name.c_str());
    DIR *dir = opendir(_folder_name);
    struct dirent *ent;
    if (dir == NULL)
    {
        return true;
    }  
    while (true)
    {  
        ent = readdir (dir);
        if (ent <= 0)
        {
            break;
        }  
        if ((strcmp(".", ent->d_name)==0) || (strcmp("..", ent->d_name)==0))
        {  
            continue;
        }  
        /*判断是否有目录和文件*/
        if ((ent->d_type == DT_DIR) || (ent->d_type == DT_REG))
        {  
            return true;
        }  
    }
    return false;
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
    string file_file_name = "_file\\" + file_name;
    string html_file_name = "_file_html\\" + file_name;
    char _file_file_name[100], _html_file_name[100];

    strcpy(_file_file_name, file_file_name.c_str());
    html_file_name = html_file_name.substr(0, html_file_name.size() - 3) + ".html";
    strcpy(_html_file_name, html_file_name.c_str());
    cout << "The html file is " << _html_file_name << endl;

    freopen(_file_file_name, "r", stdin);
    freopen(_html_file_name, "w", stdout);
    char get_one_line[1000];

    cout << "<!doctype html>\n";
    cout << "<html>\n";
    while (gets(get_one_line))
    {
        int pos = 0;
        string s = get_one_line;
        if (check_if_title(s))
        {
            change_status("normal");
            get_title(s);
        }
        else if (check_if_task(s, pos))
        {
            change_status("task");
            get_task(s, pos);
        }
        else if (check_hr(s))
        {
            change_status("normal");
            cout << s << endl;
        }
        // cout << s << endl;
    }

    cout << "</html>\n";

    freopen("CON", "r", stdin);
    freopen("CON", "w", stdout);
}

void change_status(string next_status)
{
    if (status == next_status)
        return;
    else if (status == "task" && next_status == "normal")
    {
        cout << "<\\ul>\n";
    }
    else if (status == "normal" && next_status == "task")
    {
        cout << "<ul>\n";
    }
    status = next_status;
    return;
}

bool check_if_title(string s)
{
    if (s.size() >= 2 && s.substr(0, 2) == "# " ||
        s.size() >= 3 && s.substr(0, 3) == "## " ||
        s.size() >= 4 && s.substr(0, 4) == "### " ||
        s.size() >= 5 && s.substr(0, 5) == "#### " ||
        s.size() >= 6 && s.substr(0, 6) == "##### " ||
        s.size() >= 7 && s.substr(0, 7) == "###### ")
        return true;
    else
        return false;
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

bool check_if_task(string s, int &pos)
{
    int pos1, pos2, pos3;
    pos1 = s.find('-');
    pos2 = s.find('[');
    pos3 = s.find(']');
    if (pos1 == s.npos || pos2 == s.npos || pos3 == s.npos)
        return false;
    if (s[pos1 + 1] != ' ')
        return false;
    if (pos2 + 2 != pos3)
        return false;
    if (s[pos2 + 1] != ' ' && s[pos2 + 1] != 'x')
        return false;
    if (s.size() - 1 >= pos3 + 1 && s[pos3 + 1] != ' ')
        return false;
    for (int i = 0; i < pos1; i++)
        if (s[i] != ' ')
            return false;
    pos = pos3 + 1;
    for (int i = pos3 + 1; i < s.size(); i++)
    {
        if (s[i] != ' ')
        {
            pos = i;
            return true;
        }
    }
    return true;
}

void get_task(string s, int pos)
{
    cout << "<li style=\"list-style-type: none;position: relative;\">\n\t<input type = \'checkbox\' disabled = \'disabled\' />";
    for (int i = pos; i < s.size(); i++)
    {
        cout << s[i];
    }
    cout << endl;
    cout << "</li>\n";
}

bool check_hr(string s)
{
    //cnt_skip to count skip and cnt_ to count - or *
    int cnt_skip = 0, cnt_1 = 0, cnt_2 = 0;
    int s_size = s.size();
    for (int i = 0; i < s_size; i++)
    {
        if (s[i] == ' ')
            cnt_skip++;
        else if (s[i] == '-')
        {
            cnt_1++;
        }
        else if (s[i] == '*')
        {
            cnt_2++;
        }
        else
        {
            return false;
        }
    }
    if (cnt_1 + cnt_skip == s_size || cnt_2 + cnt_skip == s_size)
        return true;
    else
        return false;
}

void get_hr(string s)
{
    cout << "<hr //>\n";
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
    else if (s == "turn")
    {
        turn("3.md");
    }
    // system("pause");
    return 0;
}