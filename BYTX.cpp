#include <iostream>

#define Test cout << "this is a test line\n"

using namespace std;
void init()
{
    cout<<"dddd\n";
}
int main(int argc, char *argv[])
{
    string s = argv[1];
    if (s == "init")
    {
        try
        {
            if(argc!=2) throw "Error\n";
            init();
        }
        catch (char const * throw_string)
        {
            cout<<throw_string<<endl;
            exit(0);
        }
    }
    cout<<"test"<<endl;

    return 0;
}