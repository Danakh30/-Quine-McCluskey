#include<iostream>
#include<regex>
#include<string>
#include<sstream>
#include<set>
using namespace std;
set<char> vars;
string f;

void variables()
{
    f.erase(remove_if(f.begin(),f.end(),::isspace),f.end());
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

bool checkValidity()
{
    if(!SoP())
    {
        if(!PoS())
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

// int main()
// {
//     bool flag=0;
//     while(!flag)
//     {
//         cout << "Enter your function in SoP or PoS form: ";
//         getline(cin, f);
//         flag=checkValidity();
//     }
// }