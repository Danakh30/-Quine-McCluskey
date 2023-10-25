#include<iostream>
#include<regex>
#include<string>
#include<sstream>
#include<set>
using namespace std;
set<char> vars;

void variables(string f, string flag)
{
    istringstream stream(f);
    string temp;
    regex pattern("[a-zA-Z]");
    if(flag == "SoP")
    {
        while(getline(stream, temp, '+'))
        {
            for(char index : temp)
            {
                string charAsString(1,index);
                if(regex_match(charAsString,pattern))
                {
                    vars.insert(index);
                }
            }
        }
    }
    else if(flag == "PoS")
    {
        
    }
    for(char i : vars)
    {
        cout << i << endl;
    }
}

void SoP(string f)
{
    regex pattern("[a-zA-Z]('|[a-zA-Z]|\\s*\\+\\s*?[a-zA-Z]|\\s*)*"); //I still need to consider the cases with brakets
    if(regex_match(f,pattern))
    {
        cout << "Match found!" << endl;
        variables(f,"SoP");
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
    string s="a";
    SoP(s);

}