#include"input.cpp"

unordered_map<string, vector<bool>> values;
vector<int> minterms;

int binToDecimal(string bin)
{
    int decimal = 0;
    int power = 0;
    for (int i = bin.length() - 1; i >= 0; --i) 
    {
        if (bin[i] == '1') {
            decimal += static_cast<int>(pow(2, power));
        }
        power++;
    }
    return decimal;
}

void generateChanonical(vector<vector<bool>> table)
{
    string SoP="", PoS="";
    int decimal;
    for(int i=0; i<pow(2,vars.size()); i++)
    {
        string bin="";
        if(values["f"][i]==1)
        {
            if(SoP != ""){SoP += " + ";}
            for(auto j : vars)
            {
                if(values[string(1,j)][i] == 1)
                {
                    SoP += string(1,j);
                    bin += "1";
                }
                else
                {
                    SoP += string(1,j);
                    SoP += "'";
                    bin += "0";
                }
            }
            decimal = binToDecimal(bin);
            minterms.push_back(decimal);

        }
        else
        {
            PoS += "(";
            int count=0;
            for(auto j : vars)
            {
                count ++;
                if(values[string(1,j)][i] == 0)
                {
                    PoS += string(1,j);
                    if(count != vars.size()){PoS += " + ";}
                }
                else
                {
                    PoS += string(1,j);
                    PoS += "'";
                    if(count != vars.size()){PoS += " + ";}
                }
            }
            PoS += ")";
        }
    }
    cout << endl;
    cout << "Chanoninal SoP: " << SoP << endl;
    cout << "Chanonical PoS: " << PoS << endl;
}

void printTT(vector<vector<bool>> table)
{
    cout << "--------------Truth Table--------------" << endl;
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

void generateTT(string flag)
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
    if(flag == "SoP"){fillSoP(0);}
    else{fillPoS();}
    for(int i=0; i<pow(2,columns); i++)
    {
        table[columns][i]=values["f"][i];
    }
    printTT(table);
    generateChanonical(table);
}

// int main()
// {
//     string flag="0";
//     while(flag == "0")
//     {
//         cout << "Enter your function in SoP or PoS form: ";
//         getline(cin, f);
//         flag=checkValidity();
//     }
//     generateTT(flag);
// }