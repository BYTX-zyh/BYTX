#include <iostream>
#include <direct.h>
#include <dirent.h>
#include <string>
#include <io.h>
#include <windows.h>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#define debug(x) cout << #x << '\t' << x << "\n";
#define Test cout << "this is a test line\n";
using namespace std;
/**
 * title means that <h1>
 * task means that the last line is a task
 * normal means that not need such as </ul>
 * */
string status = "normal";

fstream write_file, read_file;

//get the pwd
string get_pwd();

//init at the file
void init();
//
void generate();

//turn markdown to HTML
void turn(string file_name);
//create file and check if at the file inited
bool create_file(string _pwd, string name);
//create folder and check if at the file inited
bool create_folder(string _pwd, string name);
//get the time about the file
// bool GetFileTime(HANDLE hFile, LPSTR lpszCreationTime, LPSTR lpszLastAccessTime, LPSTR lpszLastWriteTime);
//check a foleder if exist
bool check_folder_if_exist(string folder_name);
// get all file name of pwd
void getFiles(string path, vector<string> &files);

//check head and save at map
bool check_head(map<string, string> &head_map);
//add <head>
void add_head(map<string, string> head_map);

//check if title title_style:atx,setext1(h1),setext2(h2)
bool check_if_title(const char *markdown_file_name, const string &s, const int line_cnt, string &title_style);
//get a title title_style:atx,setext1(h1),setext2(h2)
void get_title(const string &s, const string &title_style);

//check if task and return the first position of the task
bool check_if_task(string s, int &pos, bool &if_check);
//get a task and turn to HTML if_check mean if cheak
void get_task(string s, int pos, bool if_check);

//check if need print a <hr />
bool check_hr(string s);
//need a <hr/>
void get_hr(string s);

//check math
bool check_math(const char *markdown_file_name, int now_line, int &next_line);
//check code
bool check_code(const char *markdown_file_name, int now_line, int &next_line);
// turn code
void turn_code(string &s);

//check if reserved word and some other like ==mark==
void turn_word(string s, int &pos);

//check if need <em> x:*/_  next_pos mean that the last pos of  *
bool check_if_em(char x, string &s, int pos, int &next_pos);
//check if need <strong> others like check_if_i pos->** code *<-next_pos*
bool check_if_strong(char x, string &s, int pos, int &next_pos);
//check if a img only check like ![alt](path)
bool check_if_img(string &s, int pos, int &next_pos);
//check if inline math
bool check_if_inline_math(const string &s, int pos, int &next_pos);

//check if need <Mark> for now_pos to next_pos now_pos:==code==:next_pos
bool check_if_need_mark(string &s, int now_pos, int &next_pos);
//get a pos need mark for pos to end_pos pos:==code==:end_pos
void get_mark(string s, int pos, int end_pos);

//change status and print some code such as <\ul>
void change_status(string next_status);

//处理无用的空格
string skip_space(string &s);
//check h1-h6
bool check_html_h(string s, int &start_pos, int &next_pos);
//check <hr> or <hr/> 在检测到<的时候进行检测
bool check_html_hr(string s, int pos, int &next_pos);

bool GetFileTime(HANDLE hFile, LPSTR lpszCreationTime, LPSTR lpszLastAccessTime, LPSTR lpszLastWriteTime, SYSTEMTIME &stLocal3)
{
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC1, stLocal1, stUTC2, stLocal2, stUTC3;

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
    wsprintf(lpszCreationTime, "C:\t%02d/%02d/%d  %02d:%02d",
             stLocal1.wDay, stLocal1.wMonth, stLocal1.wYear,
             stLocal1.wHour, stLocal1.wMinute);
    wsprintf(lpszLastAccessTime, "last visit:\t%02d/%02d/%d  %02d:%02d",
             stLocal2.wDay, stLocal2.wMonth, stLocal2.wYear,
             stLocal2.wHour, stLocal2.wMinute);
    wsprintf(lpszLastWriteTime, "last change:\t%02d/%02d/%d  %02d:%02d",
             stLocal3.wDay, stLocal3.wMonth, stLocal3.wYear,
             stLocal3.wHour, stLocal3.wMinute);
    return true;
}

void generate()
{
    /**
     *x: a；all c:changed 
     * 
     **/
    vector<string> markdown_file;
    vector<string> html_file;
    getFiles("_file", markdown_file);
    getFiles("_file_html", html_file);

    for (int i = 0; i < markdown_file.size(); i++)
    {
        string filename = markdown_file[i];
        if (filename.substr(filename.size() - 3) != ".md")
        {
            cout << "Error.Some file not markdown file\n";
            return;
        }
    }
    for (int i = 0; i < html_file.size(); i++)
    {
        string filename = html_file[i];
        if (filename.substr(filename.size() - 5) != ".html")
        {
            cout << "Error.Some file not html file\n";
            return;
        }
    }

    for (int i = 0; i < markdown_file.size(); i++)
    {
        string markdown_file_name = markdown_file[i].substr(6, markdown_file[i].size() - 9);
        string html_file_name = "_file_html\\" + markdown_file_name + ".html";
        vector<string>::iterator f = find(html_file.begin(), html_file.end(), html_file_name);
        if (f != html_file.end())
        {
            char file[1000];
            strcpy(file, markdown_file[i].c_str());
            HANDLE hFile;
            TCHAR szCreationTime[30], szLastAccessTime[30], szLastWriteTime[30];
            SYSTEMTIME stLocal_md, stLocal_html;
            hFile = CreateFile(file, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            GetFileTime(hFile, szCreationTime, szLastAccessTime, szLastWriteTime, stLocal_md);
            if (hFile == INVALID_HANDLE_VALUE)
                return;
            CloseHandle(hFile);

            strcpy(file, html_file_name.c_str());
            hFile = CreateFile(file, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            GetFileTime(hFile, szCreationTime, szLastAccessTime, szLastWriteTime, stLocal_html);
            if (hFile == INVALID_HANDLE_VALUE)
                return;
            CloseHandle(hFile);

            if (stLocal_html.wYear > stLocal_md.wYear ||
                stLocal_html.wYear == stLocal_md.wYear && stLocal_html.wMonth > stLocal_md.wMonth ||
                stLocal_html.wYear == stLocal_md.wYear && stLocal_html.wMonth == stLocal_md.wMonth && stLocal_html.wDay > stLocal_md.wDay ||
                stLocal_html.wYear == stLocal_md.wYear && stLocal_html.wMonth == stLocal_md.wMonth && stLocal_html.wDay == stLocal_md.wDay && stLocal_html.wHour > stLocal_md.wHour ||
                stLocal_html.wYear == stLocal_md.wYear && stLocal_html.wMonth == stLocal_md.wMonth && stLocal_html.wDay == stLocal_md.wDay && stLocal_html.wHour == stLocal_md.wHour && stLocal_html.wMinute > stLocal_md.wMinute ||
                stLocal_html.wYear == stLocal_md.wYear && stLocal_html.wMonth == stLocal_md.wMonth && stLocal_html.wDay == stLocal_md.wDay && stLocal_html.wHour == stLocal_md.wHour && stLocal_html.wMinute == stLocal_md.wMinute && stLocal_html.wSecond > stLocal_md.wSecond)
            {
                cout << markdown_file_name << endl;
                continue;
            }
            else
            {
                cout << "turn" << endl;
                string s = markdown_file_name + ".md";
                turn(s);
            }
        }
        else
        {
            string s = markdown_file_name + ".md";
            turn(s);
        }
    }
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
    //create file
    if (file == NULL)
        file = fopen(fileName, "w");
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

    status = check_folder_if_exist(_pwd);
    if (status)
    {
        cout << "Error.This folder is not empty.\n";
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
    status = create_file("_file_html", "html.txt");
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
    strcpy(_folder_name, folder_name.c_str());
    DIR *dir = opendir(_folder_name);
    struct dirent *ent;
    if (dir == NULL)
    {
        return true;
    }
    while (true)
    {
        ent = readdir(dir);
        if (ent <= 0)
        {
            break;
        }
        if ((strcmp(".", ent->d_name) == 0) || (strcmp("..", ent->d_name) == 0))
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

    FILE *_file = fopen("configure.txt", "r");
    if (_file == NULL)
    {
        cout << "Error.Not in the initialized directory.\n";
        return;
    }
    fclose(_file);
    if (name[name.size() - 1] != 'd' ||
        name[name.size() - 2] != 'm' ||
        name[name.size() - 3] != '.')
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
    return;
}

void getFiles(string path, vector<string> &files)
{
    //文件句柄
    long hFile = 0;
    //文件信息
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
    {
        do
        {
            //如果是目录,迭代之
            //如果不是,加入列表
            if ((fileinfo.attrib & _A_SUBDIR))
            {
                if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
            }
            else
                files.push_back(p.assign(path).append("\\").append(fileinfo.name));
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
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
    cout << "The html file will at " << _html_file_name << endl;

    FILE *check_html_file_if_exist;
    char get_one_line[1000];

    //check if the html file exist ,if not exist then create it.
    check_html_file_if_exist = fopen(_html_file_name, "r");
    fopen(_html_file_name, "w");
    fclose(check_html_file_if_exist);

    //open the markdown file and the html file
    read_file.open(_file_file_name);
    write_file.open(_html_file_name);

    write_file << "<!doctype html>\n";
    write_file << "<html>\n";

    int parameter_cnt = 0;
    map<string, string> head_map;
    if (!check_head(head_map))
    {
        cout << "Error.Wrong title.\n";
        return;
    }

    add_head(head_map);
    write_file << "<body>\n";

    int line_cnt = 5;
    int next_line;

    while (read_file.getline(get_one_line, 1000))
    {
        line_cnt++;
        int pos = 0;
        string s = get_one_line;
        bool task_if_check;
        string title_style;
        int start_pos, end_pos;
        if (check_if_title(_file_file_name, s, line_cnt, title_style))
        {
            change_status("h");
            get_title(s, title_style);
            change_status("normal");
            if (title_style != "atx")
            {
                read_file.getline(get_one_line, 1000);
                line_cnt++;
            }
        }
        else if (check_if_task(s, pos, task_if_check))
        {
            change_status("task");
            get_task(s, pos, task_if_check);
        }
        else if (check_hr(s))
        {
            change_status("normal");
            get_hr(s);
        }
        else if (s == "$$" && check_math(_file_file_name, line_cnt, next_line))
        {
            write_file << s;
            while (line_cnt != next_line)
            {
                read_file.getline(get_one_line, 1000);
                write_file << get_one_line;
                line_cnt++;
            }
        }
        else if (s.find("```") == 0 && check_code(_file_file_name, line_cnt, next_line))
        {
            write_file << "<pre><code>\n";
            while (line_cnt != next_line - 1)
            {
                read_file.getline(get_one_line, 1000);
                string s = get_one_line;
                turn_code(s);
                line_cnt++;
            }
            read_file.getline(get_one_line, 1000);
            line_cnt++;
            write_file << "</code></pre>\n";
        }
        else if (check_html_h(s, start_pos, end_pos))
        {
            s = skip_space(s);
            for (int i = 0; i < start_pos;)
                turn_word(s, i);
            for (int i = start_pos; i <= end_pos + 4; i++)
                write_file << s[i];
            for (int i = end_pos + 5; i < s.size() - 1;)
                turn_word(s, i);
        }
        else
        {
            int s_lenth = s.length();
            for (int i = 0; i < s_lenth;)
                turn_word(s, i);
        }
    }
    write_file << "</body>\n";
    write_file << "</html>\n";
}

bool check_html_h(string s, int &start_pos, int &end_pos)
{
    s = skip_space(s);
    if (s.size() <= 8)
        return false;
    string h[] = {"h1", "h2", "h3", "h4", "h5", "h6"};
    for (int i = 0; i < 6; i++)
    {
        string x = "<" + h[i];
        int pos = s.find(x);
        start_pos = pos;
        if (pos == s.npos || pos != 0)
            continue;
        pos = s.find('>', pos);
        if (pos == s.npos)
            continue;
        x = "</" + h[i] + ">";
        pos = s.find(x, pos);
        if (pos != s.npos)
        {
            end_pos = pos;
            return true;
        }
    }
    return false;
}

string skip_space(string &s)
{
    string ss = "";
    int cnt = 0;
    for (int i = 0; i < s.size(); i++)
        if (s[i] != ' ')
            ss += s[i];
    return ss;
}

void turn_code(string &s)
{
    map<char, string> transformation;
    transformation[' '] = "&#160;";
    transformation['<'] = "&#60;";
    transformation['>'] = "&#62;";
    transformation['&'] = "&#38;";
    transformation['\"'] = "&#34;";
    transformation['\''] = "&#39;";
    for (int i = 0; i < s.size(); i++)
    {
        if (transformation.find(s[i]) != transformation.end())
            write_file << transformation[s[i]];
        else
            write_file << s[i];
    }
    write_file << endl;
}

void change_status(string next_status)
{
    if (status == next_status)
        return;
    else if (status == "task" && next_status == "normal")
    {
        write_file << "</ul>\n";
    }
    else if (status == "normal" && next_status == "task")
    {
        write_file << "<ul>\n";
    }
    status = next_status;
    return;
}

bool check_head(map<string, string> &head_map)
{
    char get_line[1000];
    string line;

    read_file.getline(get_line, 1000);
    line = get_line;
    if (line != "+++")
        return false;
    int cnt = 0;
    while (read_file.getline(get_line, 1000))
    {
        line = get_line;
        if (line == "+++")
            break;
        cnt++;
        if (cnt == 4)
            return false;
        if (line.find("title:") == 0)
            head_map["title"] = line.substr(6);
        else if (line.find("data:") == 0)
            head_map["data"] = line.substr(5);
        else if (line.find("tag:") == 0)
            head_map["tag"] = line.substr(4);
        else
            return false;
    }
    return true;
}

void add_head(map<string, string> head_map)
{
    write_file << "<head>";

    write_file << "\t<title>";
    write_file << "\t\t" << head_map["title"] << "\n";
    write_file << "\t</title>\n";

    write_file << "\t<meta charset=\"UTF-8\">\n";

    write_file << "\t<!-- 数学公式渲染 -->\n";
    write_file << "\t<script type=\"text/x-mathjax-config\">\n";
    write_file << "\t\t MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});\n";
    write_file << "\t</script>\n";
    write_file << "\t<script type=\"text/javascript\"  \n";
    write_file << "\t\tsrc=\"http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML\">\n";
    write_file << "\t</script>\n";

    write_file << "\t<script type=\"text/javascript\"\n";
    write_file << "\t\tsrc=\"http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML\">\n";
    write_file << "\t</script>\n";
    write_file << "\t<!-- highlight.js渲染 -->\n";
    write_file << "\t \n";
    write_file << "\t \n";
    write_file << "\t \n";
    write_file << "\t \n";
    write_file << "\t \n";
    write_file << "\t \n";
    write_file << "</head>\n";
}

bool check_if_title(const char *markdown_file_name, const string &s, const int line_cnt, string &title_style)
{
    if (s.size() >= 2 && s.substr(0, 2) == "# " ||
        s.size() >= 3 && s.substr(0, 3) == "## " ||
        s.size() >= 4 && s.substr(0, 4) == "### " ||
        s.size() >= 5 && s.substr(0, 5) == "#### " ||
        s.size() >= 6 && s.substr(0, 6) == "##### " ||
        s.size() >= 7 && s.substr(0, 7) == "###### ")
    {
        title_style = "atx";
        return true;
    }
    else
    {
        fstream title_check_file;
        char get_line[1000];
        title_check_file.open(markdown_file_name);
        int cnt = line_cnt + 1;
        while (cnt--)
        {
            title_check_file.getline(get_line, 1000);
        }
        title_check_file.close();
        string next_line = get_line;
        if (next_line[0] == '-')
        {
            for (int i = 0; i < next_line.size(); i++)
            {
                if (next_line[i] != '-')
                    return false;
            }
            title_style = "setext1";
            return true;
        }
        else if (next_line[0] == '=')
        {
            for (int i = 0; i < next_line.size(); i++)
            {
                if (next_line[i] != '=')
                    return false;
            }
            title_style = "setext2";
            return true;
        }
        else
            return false;
    }
}

void get_title(const string &s, const string &title_style)
{
    if (title_style == "atx")
    {
        int cnt = 0;
        for (int i = 0; i < 6; i++)
            if (s[i] == '#')
                cnt++;
        write_file << "\t<h" << cnt << ">";

        int end = s.size() - 1;
        while (s[end] == ' ')
            end--;
        for (; end; end--)
            if (s[end] != '#')
                break;
        end = (s[end] == ' ') ? end - 1 : s.size() - 1;
        //### dsfdf sdf d
        for (int i = cnt + 1; i <= end;)
            turn_word(s, i);
        write_file << "</h" << cnt << ">\n";
    }
    else if (title_style == "setext1")
    {
        write_file << "<h2>";
        for (int i = 0; i < s.size();)
            turn_word(s, i);
        write_file << "</h2>\n";
    }
    else
    {
        write_file << "<h1>";
        for (int i = 0; i < s.size();)
            turn_word(s, i);
        write_file << "</h1>\n";
    }
}

bool check_if_task(string s, int &pos, bool &if_check)
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
    if (s[pos2 + 1] == ' ')
        if_check = false;
    else
        if_check = true;
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

void get_task(string s, int pos, bool if_check)
{
    if (if_check)
        write_file << "<li style=\"list-style-type: none;position: relative;\">\n\t<input type = \'checkbox\' disabled = \'disabled\'checked/>";
    else
        write_file << "<li style=\"list-style-type: none;position: relative;\">\n\t<input type = \'checkbox\' disabled = \'disabled\' />";
    for (int i = pos; i < s.size();)
    {
        turn_word(s, i);
    }
    write_file << endl;
    write_file << "</li>\n";
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
            cnt_1++;
        else if (s[i] == '*')
            cnt_2++;
        else
            return false;
    }
    if (cnt_1 >= 3 && cnt_1 + cnt_skip == s_size || cnt_2 >= 3 && cnt_2 + cnt_skip == s_size)
        return true;
    else
        return false;
}

void get_hr(string s)
{
    write_file << "<hr />\n";
}

bool check_code(const char *markdown_file_name, int now_line, int &next_line)
{
    fstream code_cnt;
    next_line = now_line;
    char get_one_line[1000];
    code_cnt.open(markdown_file_name);
    while (now_line--)
    {
        code_cnt.getline(get_one_line, 1000);
    }
    while (code_cnt.getline(get_one_line, 1000))
    {
        next_line++;
        string s = get_one_line;
        if (s == "```")
        {
            code_cnt.close();
            return true;
        }
    }
    code_cnt.close();
    return false;
}

bool check_math(const char *markdown_file_name, int now_line, int &next_line)
{
    fstream math_cnt;
    next_line = now_line;
    char get_one_line[1000];
    math_cnt.open(markdown_file_name);
    while (now_line--)
    {
        math_cnt.getline(get_one_line, 1000);
    }
    while (math_cnt.getline(get_one_line, 1000))
    {
        string s = get_one_line;
        next_line++;
        if (s == "$$")
        {
            math_cnt.close();
            return true;
        }
    }
    math_cnt.close();
    return false;
}

void turn_word(string s, int &pos)
{
    if (pos == 0 && status == "normal")
        write_file << "<p>\n";
    char x = s[pos];
    int next_pos = pos;
    map<char, string> transformation;
    set<char> change_word;
    {
        change_word.insert('*');
        change_word.insert('\'');
        change_word.insert('\"');
        change_word.insert('(');
        change_word.insert(')');
        change_word.insert('[');
        change_word.insert(']');
        change_word.insert('{');
        change_word.insert('}');
        change_word.insert('#');
        change_word.insert('$');
        change_word.insert('^');
        change_word.insert('+');
        change_word.insert('-');
        change_word.insert('_');
        change_word.insert('=');
        change_word.insert('.');
        change_word.insert('!');
        change_word.insert('`');
        change_word.insert('\\');
    }
    // transformation[' '] = "&#160;";
    transformation['<'] = "&#60;";
    transformation['>'] = "&#62;";
    transformation['&'] = "&#38;";
    transformation['\"'] = "&#34;";
    transformation['\''] = "&#39;";
    if (x == '\\' && s.size() - 1 >= pos + 1 && change_word.find(s[pos + 1]) != change_word.end())
    {
        write_file << s[pos + 1];
        pos += 2;
    }
    else if (x == '=' && check_if_need_mark(s, pos, next_pos))
    {
        get_mark(s, pos, next_pos);
        pos = next_pos + 1;
    }
    else if (x == '!' && check_if_img(s, pos, next_pos))
    {
        write_file << "<img alt=\"";
        int i;
        for (i = pos + 2; s[i] != ']';)
            turn_word(s, i);
        write_file << "\"src=\"";
        for (i += 2; s[i] != ')';)
            turn_word(s, i);
        write_file << "\"/>";
        pos = next_pos + 1;
    }
    else if (check_if_strong(x, s, pos, next_pos))
    {
        write_file << "<strong>";
        int i;
        for (i = pos + 2; i <= next_pos - 1;)
            turn_word(s, i);
        write_file << "</strong>";
        pos = next_pos + 2;
    }
    else if (check_if_em(x, s, pos, next_pos))
    {
        write_file << "<em>";
        int i;
        for (i = pos + 1; i < next_pos;)
            turn_word(s, i);
        write_file << "</em>";
        pos = next_pos + 1;
    }
    else if (x == '$' && check_if_inline_math(s, pos, next_pos))
    {
        for (int i = pos; i <= next_pos; i++)
            write_file << s[i];
        pos = next_pos + 1;
    }
    else if (x == '<' && check_html_hr(s, pos, next_pos))
    {
        for(int i=pos;i<=next_pos;i++)
            write_file<<s[i];
        pos=next_pos+1;
    }
     else if (transformation.find(x) != transformation.end())
    {
        // check if need turn to &
        pos++;
        write_file << transformation[x];
    }
    else
    {
        //normal
        write_file << s[pos];
        pos++;
    }
    if (pos > s.size() - 1 && status == "normal")
        write_file << "</p>\n";
    return;
}

bool check_html_hr(string s, int pos, int &next_pos)
{
    debug(pos);
    debug(s.find("<hr",pos));
    if (s.find("<hr",pos)==pos)
    {
        int cnt=0;
        for(int i=pos;i<s.size()-1;i++)
        {
            if(s[i]=='\"')
            {
                while (true)
                {
                    i++;
                    if(s[i]=='\"'||i>=s.size()-1)
                        break;
                }
            }
            if(s[i]=='/'&&s[i+1]=='>')
            {
                next_pos=i+1;
                return true;
            }
        }
    }
    return false;
}

bool check_if_inline_math(const string &s, int pos, int &next_pos)
{
    next_pos = s.find('$', pos + 1);
    if (next_pos != pos + 1 && next_pos != s.npos && (s[next_pos - 1] != '\\' || next_pos == 0))
        return true;
    return false;
}

bool check_if_img(string &s, int pos, int &next_pos)
{
    int npos = s.npos;
    int find_m_r = s.find(']');

    if (pos + 1 > s.size() - 1 ||
        s[pos + 1] != '[' ||
        find_m_r == npos)
        return false;
    int find_s_l = s.find('(', find_m_r);
    if (find_s_l == npos || find_m_r + 1 != find_s_l)
        return false;
    int find_s_r = s.find(')', find_s_l + 1);
    if (find_s_r == npos)
        return false;
    next_pos = find_s_r;
    return true;
}

bool check_if_em(char x, string &s, int pos, int &next_pos)
{
    /**
     * 是否还有后续字符以及另一个强调符的位置
     * 第一个强调符前后都存在一个空格
     * 后续没有强调符
     * */
    if (x != '*' && x != '_')
        return false;
    next_pos = s.find(x, pos + 2);
    if (pos + 2 > s.size() - 1 ||
        (pos == 0 || pos != 0 && s[pos - 1] == ' ') && s[pos + 1] == ' ' ||
        next_pos == s.npos)
        return false;
    int pos_check = pos + 1;
    while (pos_check <= s.size() - 1)
    {
        pos_check = s.find(x, pos_check);
        if (pos_check == s.npos)
            break;
        if (s[pos_check - 1] != '\\' &&
            !(s[pos_check - 1] == ' ' && pos_check + 1 <= s.size() - 1 && s[pos_check + 1] == ' ') &&
            s[pos_check - 1] != x)
        {
            next_pos = pos_check;
            return true;
        }
        pos_check++;
    }
    return false;
}

bool check_if_strong(char x, string &s, int pos, int &next_pos)
{
    if (!(pos + 1 <= s.size() - 1 &&
          (x == '*' && s[pos + 1] == '*' || x == '_' && s[pos + 1] == '_')))
        return false;
    int s_size = s.size();
    if (pos + 2 > s_size - 1 ||
        pos + 2 <= s_size - 1 && s[pos + 2] == ' ' && (pos != 0 && s[pos - 1] == ' ') ||
        pos != 0 && s[pos - 1] == '\\')
        return false;
    int pos_check = pos + 2;
    while (s[pos] == x)
        pos++;
    string xx;
    if (x == '*')
        xx = "**";
    else
        xx = "__";
    bool flag = false;
    while (pos <= s_size - 1)
    {
        pos = s.find(xx, pos + 1);
        if (pos == s.npos)
            break;
        if (s[pos - 1] == ' ' && (pos + 1 != s_size - 1 || pos + 2 <= s_size - 1 && s[pos + 2] == ' ') ||
            s[pos - 1] == '\\')
        {
            pos++;
            continue;
        }
        else
        {
            pos++;
            flag = true;
            next_pos = pos;
        }
    }
    if (flag)
        return true;
    else
        return false;
}

bool check_if_need_mark(string &s, int now_pos, int &next_pos)
{
    if (now_pos != 0 && s[now_pos - 1] == '\\' || now_pos + 1 > s.size() - 1 || s[now_pos + 1] != '=')
        return false;
    next_pos = s.find("==", now_pos + 2);
    if (next_pos == s.npos || now_pos + 2 == next_pos)
        return false;
    next_pos++;
    return true;
}

void get_mark(string s, int pos, int end_pos)
{
    write_file << "<mark>";
    for (int i = pos + 2; i <= end_pos - 2;)
    {
        turn_word(s, i);
    }
    write_file << "</mark>";
}

int main(int argc, char *argv[])
{
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
    else if (s == "turn")
    {
        turn("3.md");
    }
    // system("pause");
    return 0;
}