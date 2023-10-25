#include<iostream>
#include<regex>
#include<string>
#include<sstream>
#include<set>
using namespace std;
set<char> vars;

void variables(string f)
{
    istringstream stream(f);
    string temp;
    regex pattern("[a-zA-Z]");
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
    // for(char i : vars)
    // {
    //     cout << i << endl;
    // }
}

bool SoP(string f)
{
    regex pattern("[a-zA-Z]('|[a-zA-Z]|\\s*\\+\\s*?[a-zA-Z]|\\s*)*"); //I still need to consider the cases with brakets
    if(regex_match(f,pattern))
    {
        variables(f);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool PoS(string f)
{
    regex pattern("([a-zA-Z]((\\s*\\+\\s*[a-zA-Z])|')*)|(\\([a-zA-Z]((\\s*\\+\\s*[a-zA-Z])|')*\\))+");
    if(regex_match(f,pattern))
    {
        variables(f);
        return 1;
    }
    else
    {
        return 0;
    }
}

bool checkValidity(string f)
{
    if(!SoP(f))
    {
        if(!PoS(f))
        {
            cout << "Invalid Input!" << endl;
            return 0;
        }
        else
        {
            cout << "The function you entered is in PoS form" << endl;
            return 1;
        }
    }
    else
    {
        cout << "The function you entered is in SoP form" << endl;
        return 1;
    }
}

int main()
{
    string f;
    bool flag=0;
    while(!flag)
    {
        cout << "Enter your function in SoP or PoS form: ";
        cin  >> f;
        flag=checkValidity(f);
    }
}