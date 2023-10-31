#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;



pair<int, int> RowColumn(int literals){
    switch(literals){
    case (2):
        return {2,2};

    case (3):
        return {2,4};
    case (4):
        return {4,4};
    default:
        return {0,0};
    }

}


vector<vector<char>> CreateKmap(vector<int> minterm, int Noliterals){   
    auto dimensions= RowColumn(Noliterals);  
    const int val[] = {0,1,3,2};                      
    auto row= dimensions.first;
    auto col= dimensions.second;
    vector<vector<char>> kmap(row, vector<char>(col));
    for(int i = 0; i < row; ++i){
        for(int j = 0; j < col; ++j){
            int value = (val[i] << 2) + val[j];
            if (find(minterm.begin(), minterm.end(), value) != minterm.end()) {
              kmap[i][j] = '1';
            } else {
              kmap[i][j] = '0';
            }
        }
    }
    return kmap;
}

void DisplayKmap(vector<int> minterm, int Noliterals){
  auto kmap= CreateKmap(minterm, Noliterals);
    for(auto &row: kmap){
        for(auto &cell: row){
            cout << cell << '\t';
        }
        cout << '\n';
    }
}

