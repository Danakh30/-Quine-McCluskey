#include"input.cpp"
#include<vector>
#include<cmath>

void generateTT()
{
    vector<vector<bool>> table(vars.size(),vector<bool>(pow(2,vars.size())));
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
    for(char it : vars)
    {
        cout << it << "\t";
    }
    cout << endl;
    for(int i=0; i<pow(2,columns); i++)
    {
        for(int j=0; j<columns; j++)
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
