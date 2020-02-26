/**
 * author:BYTX
 * 
 * */
#include <bits/stdc++.h>
#include <algorithm>
#define ll long long
#define Test cout << "this is a test line" << endl
#define debug(x) cout << #x << '\t' << x << endl
using namespace std;
const int INF = 1e8;

// ------------------------------------------------------------------------------------------------------------
// head部分
void add_head()
{
    cout << "<head>\n";
    cout << "\t<title>\n";
    cout << "\t\t星河散尽故人来\n ";
    cout << "\t</title>\n";
    cout << "\t<meta charset=\"UTF-8\">\n";
    cout << "\t<!-- --------------------------------------------------------------------------- -->\n";
    cout << "\t<!-- 数学公式渲染 -->\n";
    cout << "\t<script type=\"text/x-mathjax-config\">\n";
    cout << "\t\t MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\(','\\)']]}});\n";
    cout << "\t</script>\n";
    cout << "\t<script type=\"text/javascript\"  \n";
    cout << "\t\tsrc=\"http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML\">\n";
    cout << "\t</script>\n";
    cout << "\t<!-- --------------------------------------------------------------------------- -->\n";
    cout << "\t<!-- highlight.js渲染 -->\n";
    cout << "\t<link rel=\"stylesheet\" href=\"highlight\\styles\\atom-one-light.css\">\n";
    cout << "\t<script src=\"highlight\\highlight.pack.js\"></script>\n";
    cout << "\t<script>hljs.initHighlightingOnLoad();</script>\n";
    cout << "\t<!-- --------------------------------------------------------------------------- -->\n";
    cout << "\t<link rel=\"stylesheet\" type=\"text/css\" href=\"css/index.css\">\n";
    cout << "</head>\n";
}

// ------------------------------------------------------------------------------------------------------------
// footer部分
void add_footer()
{
    cout << "\t<footer>\n";
    cout << "\t\t<p><a href=\"https://bytx-zyh.com/blog\">BYTX's blog</a> by BYTX</p>\n";
    cout << "\t\t<nav>\n";
    cout << "\t\t\t<ul>\n";
    cout << "\t\t\t\t<li>由 BYTX 弱力手打</li>\n";
    cout << "\t\t\t\t<li><a href=https://github.com/BYTX-zyh>BYTX</a></li>\n";
    cout << "\t\t\t</ul>\n";
    cout << "\t\t</nav>\n";
    cout << "\t</footer>\n";
}
// ------------------------------------------------------------------------------------------------------------
//  Navigation bar
void add_navigation_bar()
{
    cout << "\t<!-- --------------------------------------------------------------------------- -->\n";
    cout << "\t<!-- navigation bar -->\n";
    cout << "\t<ul class=\"navigation_bar_ul\">\n";
    cout << "\t\t<li class=\"navigation_bar_li\"><a class=\"navigation_bar_a\" href=\"https://github.com/BYTX-zyh\">白衣踏雪</a></li>\n";
    cout << "\t\t<li class=\"navigation_bar_li\"><a class=\"navigation_bar_a\" href=\"index.html\">Home</a></li>\n";
    cout << "\t\t<li class=\"navigation_bar_li\"><a class=\"navigation_bar_a\" href=\"tags.html\">Tags</a></li>\n";
    cout << "\t\t<li class=\"navigation_bar_li\"><a class=\"navigation_bar_a\" href=\"archives.html\">Archives</a></li>\n";
    cout << "\t\t<li class=\"navigation_bar_li\"><a class=\"navigation_bar_a\" href=\"about.html\">About</a></li>\n";
    cout << "\t</ul>\n";
    cout << "\t<br><br>\n";
}
string status = "normal";

void get_h(string s)
{
    int cnt_h = 1;
    int start_h = 1;
    for (int i = 1; i < s.size(); i++)
    {
        if (s[i] == s[i - 1])
            cnt_h++;
        else
            break;
    }
    cout << "\t<h" << cnt_h << ">";
    for (int i = cnt_h; i < s.size(); i++)
    {
        cout << s[i];
    }
    cout << "\t</h" << cnt_h << ">" << endl;
}

void get_code()
{
    char get_line[10000];
    string s;
    cout << "\t<pre>\n";
    cout << "\t\t<code>\n";
    while (gets(get_line))
    {
        s = get_line;
        if (s[0] == s[1] && s[1] == s[2] && s[2] == '`')
            break;
        else
        {
            for (int i = 0; i < s.size(); i++)
            {
                if (s[i] == '<')
                    cout << "&#60;";
                else if (s[i] == '>')
                    cout << "&#62;";
                else if (s[i] == '&')
                    cout << "&#38;";
                else if (s[i] == '"')
                    cout << "&#34;";
                else if (s[i] == '\'')
                    cout << "	&#39;";
                else
                    cout << s[i];
            }
            cout << endl;
        }
    }
    cout << "\t\t</code>\n";
    cout << "\t</pre>\n";
    status = "normal";
    return;
}
void get_math()
{
    char get_line[10000];
    cout<<"\t<p>$$\n";
    while(gets(get_line)){
        string s=get_line;
        if(s=="$$") break;
        cout<<s<<endl;
    }
    status = "normal";
    cout<<"\t$$</p>\n";
    return ;
}
void read_file()
{
    char get_line[10000];
    while (gets(get_line))
    {
        string s = get_line;
        if (s == "")
            continue;
        if (s[0] == '#' && status == "normal")
            get_h(s);
        else if (status == "normal" && s[0] == s[1] && s[1] == s[2] && s[2] == '`')
        {
            status = "code";
            get_code();
        }
        else if (s[0] == s[1] && s[1] == '$')
        {
            status = "math";
            get_math();
        }
        else
        {
            cout << "\t<p>";
            for (int i = 0; i < s.size(); i++)
            {
                if (s[i] != '`')
                    cout << s[i];
                else if (status == "normal")
                {
                    status = "code_in_line";
                    cout << "<code class =c++>";
                }
                else
                {
                    status = "normal";
                    cout << "</code>";
                }
            }
            cout << "</p>\n";
        }
        // debug(status);
    }
}

int main()
{

    freopen("_file\\3.md", "r", stdin);
    // freopen("test.html", "w", stdout);
    char s[1000];
    while(gets(s)){
        if(s[0]=='\t')cout<<"fdfdfdf"<<endl;
    }
    // cout << "<!DOCTYPE html>\n";
    // cout << "<html>\n";
    // add_head();

    // cout << "<body marginheight=\"0px\" marginwidth=\"0px\">\n";
    // add_navigation_bar();
    // cout << "\t<div style=\"margin-right:10%;margin-left:10%;\">";
    // read_file();
    // cout << "\t</div>\n";
    // add_footer();
    // cout << "</body>\n";
    system("pause");
    return 0;
}