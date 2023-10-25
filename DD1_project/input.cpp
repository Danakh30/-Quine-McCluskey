#include<iostream>
#include<regex>
#include<string>
using namespace std;

void SoP(string f)
{
    regex pattern("[a-zA-Z]('|[a-zA-Z]|\\s*\\+\\s*?[a-zA-Z]|\\s*)*");
    if(regex_match(f,pattern))
    {
        cout << "Match found!" << endl;
    }
    else
    {
        cout << "Match not found!" << endl;
    }
}

void PoS(string f)
{
    regex pattern("([a-zA-Z](\\s*\\+\\s*[a-zA-Z])*)|(\\([a-zA-Z](\\s*\\+\\s*[a-zA-Z])*\\))+");
    if(regex_match(f,pattern))
    {
        cout << "Match found!" << endl;
    }
    else
    {
        cout << "Match not found!" << endl;
    }
}

int main()
{
    string s="a+b'' '";
    SoP(s);

}