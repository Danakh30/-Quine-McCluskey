#include"input.cpp"
#include<vector>
#include<cmath>

void generateTT()
{
    vector<vector<bool>> table(vars.size(),vector<bool>(pow(2,vars.size())));
    for(int i=0; i<vars.size(); i++) //just needs to be filled backward
    {
        int j = 0;
        bool bin = false;
        while(j<pow(2,vars.size()))
        {
            for(int k=0; k<pow(2,i); k++)
            {
                table[i][j]=bin;
                ++j;
            }
            bin=!bin;
        }
    }
    for(char it : vars)
    {
        cout << it << "\t";
    }
    cout << endl;
    for(int i=0; i<pow(2,vars.size()); i++)
    {
        for(int j=0; j<vars.size(); j++)
        {
            cout << table[j][i] << '\t';
        }
        cout << endl;
    }
}

int main()
{
    string f;
    bool flag=0;
    while(!flag)
    {
        cout << "Enter your function in SoP or PoS form: ";
        getline(cin, f);
        flag=checkValidity(f);
    }
    generateTT();
}