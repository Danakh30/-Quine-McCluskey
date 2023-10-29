#include"input.cpp"
#include<vector>
#include<cmath>
#include<unordered_map>
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
        for(int j=0; j<columns; j++)
        {
            cout << table[j][i] << '\t';
        }
        cout << endl;
    }
}

void fillSoP()
{
    istringstream stream(f);
    string temp, temp1;
    bool result;
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
        for(auto j : values[to_string(i[0])])
        {
            values[i].push_back(!j);
        }
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

void generateTT()
{
    vector<vector<bool>> table(vars.size()+1,vector<bool>(pow(2,vars.size())+1));
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
    int i=0;
    for(char it : vars)
    {
        for(int j=0; j<pow(2,columns); j++)
        {
            values[to_string(it)].push_back(table[i][j]);
        }
        i++;
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
        flag=checkValidity();
    }
    generateTT();
    fillSoP();
    fillSoP();
}
