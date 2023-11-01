#include<iostream>
#include<regex>
#include<string>
#include<sstream>
#include<set>
#include <unordered_set>
#include <map>
#include <bitset>
#include<vector>
#include<cmath>
#include<unordered_map>
#include<algorithm>
using namespace std;
set<char> vars;
string f;

void variables()
{
    f.erase(remove_if(f.begin(),f.end(),::isspace),f.end());
    size_t found = f.find("\'\'");
    while(found != string::npos)
    {
        f.erase(found,2);
        found = f.find("\'\'");
    }
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
}

bool SoP()
{
    regex pattern("[a-zA-Z]('|[a-zA-Z]|\\s*\\+\\s*?[a-zA-Z]|\\s*)*"); //I still need to consider the cases with brakets
    if(regex_match(f,pattern))
    {
        variables();
        return 1;
    }
    else
    {
        return 0;
    }
}

bool PoS()
{
    regex pattern("([a-zA-Z]((\\s*\\+\\s*[a-zA-Z])|')*)|(\\([a-zA-Z]((\\s*\\+\\s*[a-zA-Z])|')*\\))+");
    if(regex_match(f,pattern))
    {
        variables();
        return 1;
    }
    else
    {
        return 0;
    }
}

string checkValidity()
{
    if(!SoP())
    {
        if(!PoS())
        {
            cout << "Invalid Input!" << endl;
            return "0";
        }
        else
        {
            cout << "The function you entered is in PoS form" << endl;
            return "PoS";
        }
    }
    else
    {
        cout << "The function you entered is in SoP form" << endl;
        return "SoP";
    }
}