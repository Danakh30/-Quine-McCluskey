#include"input.cpp"
#include<vector>
#include<cmath>
#include<unordered_map>
#include<algorithm>
unordered_map<string, vector<bool>> values;

void printTT(vector<vector<bool>> table)
{
    int columns = vars.size();
    for(char it : vars)
    {
        cout << it << "\t";
    }
    cout << "f" << endl;

    for(int i=0; i<pow(2,columns); i++)
    {
        for(int j=0; j<columns+1; j++)
        {
            cout << table[j][i] << '\t';
        }
        cout << endl;
    }
}

void fillSoP(bool flag)
{
    istringstream stream(f);
    string temp;
    bool result, temp1;
    set<string> new_columns;
    while(getline(stream, temp, '+'))
    {
        size_t found = temp.find("\'");
        while(found != string::npos)
        {
            new_columns.insert(temp.substr(found-1,2));
            found = temp.find("\'", found + 1);
        }
    }

    for(auto i : new_columns)
    {
        for(auto j : values[string(1,i[0])])
        {
            values[i].push_back(!j);
        }
    }
    vector<string> terms;
    istringstream stream2(f);
    while(getline(stream2 ,temp, '+'))
    {
        terms.clear();
        for(int i=0; i<temp.size(); i++)
        {
            if(i!=temp.size()-1)
            {
                if(temp[i+1]=='\'')
                {
                    terms.push_back(temp.substr(i,2));
                    i++;
                }
                else
                {
                    terms.push_back(string(1,temp[i]));
                }
            }
            else
            {
                terms.push_back(string(1,temp[i]));
            }
        }
        for(int i=0; i<pow(2,vars.size()); i++)
        {
            temp1 = 1;
            for(auto j : terms)
            {
                temp1 = temp1 && values[j][i];
            }
            values[temp].push_back(temp1);
        }
    }

    for(int i=0; i<pow(2,vars.size()); i++)
    {
        istringstream stream3(f);
        result=0;
        while(getline(stream3, temp, '+'))
        {
            result = result || values[temp][i];
        }
        if(!flag){values["f"].push_back(result);}
        else{values["f"].push_back(!result);}
    }

    // for(auto it = values.begin(); it!= values.end(); it++)
    // {
    //     cout << it-> first << '\t';
    //     for(int j=0; j<it->second.size(); j++)
    //     {
    //         cout << it->second[j] << ' ';
    //     }
    //     cout << endl;
    // }
}

void fillPoS()
{
    string temp;
    vector<string> terms;
    if(f[0] == '(')
    {
        f.erase(remove(f.begin(), f.end(), '('), f.end());
        istringstream stream(f);
        while(getline(stream, temp, ')'))
        {
            terms.push_back(temp);
        }
    }
    f.clear();
    for(int i=0; i<terms.size(); i++)
    {
        replace(terms[i].begin(), terms[i].end(), '+', '\'');
        f += terms[i];
        f += "'";
        if(i != terms.size()-1)
        {
            f+= "+";
        } 
    }
    size_t found = f.find("\'\'");
    while(found != string::npos)
    {
        f.erase(found,2);
        found = f.find("\'\'");
    }
    fillSoP(1);   
}

void generateTT()
{
    vector<vector<bool>> table(vars.size()+1,vector<bool>(pow(2,vars.size())+1));
    int columns = vars.size();
    for(int i=0; i<columns; i++) 
    {
        int j = 0;
        bool bin = 0;
        while(j<pow(2,columns))
        {
            for(int k=0; k<pow(2,columns-i-1); k++)
            {
                table[i][j]=bin;
                ++j;
            }
            bin=!bin;
        }
    }
    int i=0;
    for(char it : vars)
    {
        for(int j=0; j<pow(2,columns); j++)
        {
            values[string(1,it)].push_back(table[i][j]);
        }
        i++;
    }
    //fillSoP();
    fillPoS();
    for(int i=0; i<pow(2,columns); i++)
    {
        table[columns][i]=values["f"][i];
    }
    printTT(table);
}

int main()
{
    bool flag=0;
    while(!flag)
    {
        cout << "Enter your function in SoP or PoS form: ";
        getline(cin, f);
        flag=checkValidity();
    }
    generateTT();
    //fillPoS();
}