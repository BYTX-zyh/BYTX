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

//check head and save at map
bool check_head(map<string, string> &head_map);
//add <head>
void add_head(map<string, string> head_map);

//check if title
bool check_if_title(string s);
//get a title
void get_title(string s);

//check if task and return the first position of the task
bool check_if_task(string s, int &pos, bool &if_check);
//get a task and turn to HTML if_check mean if cheak
void get_task(string s, int pos, bool if_check);

//check if need print a <hr />
bool check_hr(string s);
//need a <hr/>
void get_hr(string s);

bool check_math( const char * markdown_file_name, int now_line, int &next_line);

//check if reserved word and some other like ==mark==
void turn_word(string s, int &pos);

//check if need <i> x:*/_  next_pos mean that the last pos of  *
bool check_if_i(char x, string s, int pos, int &next_pos);
//check if need <b> others like check_if_i pos->** code *<-next_pos*
bool check_if_b(string s, int pos, int &next_pos);
//check if need <b><i>
bool check_if_bi(string s, int pos, int &next_pos);

//check if need <Mark> for now_pos to next_pos now_pos:==code==:next_pos
bool check_if_need_mark(string s, int now_pos, int &next_pos);
//get a pos need mark for pos to end_pos pos:==code==:end_pos
void get_mark(string s, int pos, int end_pos);

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
    _file = fopen("_file\\file.txt", "r");
    if (_file == NULL)
    {
        cout << "Error._file\\file.txt does not exist.\n";
        return;
    }
    fclose(_file);
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
        if (check_if_title(s))
        {
            change_status("h");
            get_title(s);
            change_status("normal");
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
            write_file<<s;
            while(line_cnt!=next_line){
                read_file.getline(get_one_line, 1000);
                write_file<<get_one_line;
                line_cnt++;
            }
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
    write_file << "\t<h" << cnt << ">";
    for (int i = cnt; i < s.size();)
        turn_word(s, i);
    write_file << "</h" << cnt << ">\n";
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

bool check_math( const char * markdown_file_name, int now_line, int &next_line)
{
    fstream math_cnt;
    next_line=now_line;
    char get_one_line[1000];
    math_cnt.open(markdown_file_name);
    while(now_line--){
        math_cnt.getline(get_one_line,1000);
    }
    while(math_cnt.getline(get_one_line,1000)){
        string s=get_one_line;
        next_line++;
        if(s=="$$")
            return true;
    }
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
    change_word.insert('~');

    // transformation[' '] = "&#160;";
    transformation['<'] = "&#60;";
    transformation['>'] = "&#62;";
    transformation['&'] = "&#38;";
    transformation['\"'] = "&#34;";
    transformation['\''] = "&#39;";

    if (x == '\\' && s.size() - 1 >= pos + 1 && change_word.find(s[pos + 1]) != change_word.end())
    {
        //check if such as \'
        write_file << s[pos + 1];
        pos += 2;
    }
    else if (x == '*' && check_if_bi(s, pos, next_pos))
    {
        write_file << "<b><i>";
        int i;
        for (i = pos + 3; i < next_pos;)
            turn_word(s, i);
        write_file << "</i></b>";
        pos = next_pos + 3;
    }
    else if (x == '*' && pos + 1 <= s.size() - 1 && s[pos + 1] == '*' && check_if_b(s, pos, next_pos))
    {
        write_file << "<b>";
        int i;
        for (i = pos + 2; i < next_pos;)
            turn_word(s, i);
        write_file << "</b>";
        pos = next_pos + 2;
    }
    else if ((x == '*' || x == '_') && check_if_i(x, s, pos, next_pos))
    {
        write_file << "<i>";
        int i;
        for (i = pos + 1; i < next_pos;)
            turn_word(s, i);
        write_file << "</i>";
        pos = next_pos + 1;
    }
    else if (transformation.find(x) != transformation.end())
    {
        // check if need turn to &
        pos++;
        write_file << transformation[x];
    }
    else if (x == '=' && check_if_need_mark(s, pos, next_pos))
    {
        // check if mark
        get_mark(s, pos, next_pos);
        pos = next_pos + 1;
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

bool check_if_bi(string s, int pos, int &next_pos)
{
    next_pos = s.find("***", pos + 4);
    if (pos + 6 > s.size() - 1 ||
        s[pos + 1] != '*' || s[pos + 2] != '*' ||
        s[pos + 3] == ' ' ||
        next_pos == s.npos ||
        s[next_pos - 1] == ' ')
        return false;
    return true;
}

bool check_if_i(char x, string s, int pos, int &next_pos)
{
    if (pos + 1 > s.size() - 1 ||
        // pos + 1 <= s.size() - 1 && s[pos + 1] == x ||
        s[pos + 1] == ' ' ||
        s[pos - 1] == x ||
        s.find(x, pos + 1) == s.npos)
        return false;
    int pos_check = pos;
    while (pos_check <= s.size() - 1 && pos_check != s.npos)
    {
        pos_check = s.find(x, pos_check + 1);
        if (s[pos_check - 1] != '\\' && s[pos_check - 1] != ' ' && s[pos_check - 1] != x &&
            (pos_check == s.size() - 1 ||
             pos_check + 1 <= s.size() - 1 && s[pos_check + 1] != x))
        {
            next_pos = pos_check;
            return true;
        }
        if (pos_check == s.npos)
            break;
        pos_check++;
    }
    return false;
}

bool check_if_b(string s, int pos, int &next_pos)
{
    if (pos + 2 <= s.size() - 1 && s[pos + 2] == ' ' ||
        pos != 0 && s[pos - 1] == '\\' ||
        s.find("**", pos + 2) == s.npos)
        return false;
    next_pos = s.find("**", pos + 2);
    return true;
}

bool check_if_need_mark(string s, int now_pos, int &next_pos)
{
    if (now_pos != 0 && s[now_pos - 1] == '\\' || s[now_pos + 1] != '=')
        return false;
    next_pos = s.find('=', now_pos + 2);
    if (next_pos == s.npos)
        return false;
    if (next_pos + 1 < s.size() && s[next_pos + 1] != '=')
        return false;
    if (now_pos + 2 == next_pos)
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