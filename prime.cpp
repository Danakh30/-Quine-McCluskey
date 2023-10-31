#include <iostream>
#include <unordered_set>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <map>
#include <algorithm>
#include <bitset>
using namespace std;


typedef pair<set<int>, string> pi; // minterms, binary representation
typedef set<pi> pi_group; // collection of pi

map<int, pair<string, int>> BinaryRepresentation(const vector<int> &minterms, int literals)
{   
    map<int, pair<string, int>> implicant;
    for(int i=0; i<minterms.size(); i++)
    {   
        string binarystring = bitset<32>(minterms[i]).to_string().substr(32 - literals, literals);
        int countone= count(binarystring.begin(), binarystring.end(), '1'); 
        implicant[minterms[i]] = {binarystring,countone};
        // printf("%d\t%s\n",countone,binarystring.c_str());
    }
    return implicant; 
}

bool compare_distance(string a, string b)
{
    int count=0;
    for(int i=0; i<a.size(); ++i)
    {
        if(a[i]!=b[i])
        {
        count++;
        }
    }
    return count==1;

}

string group(string a, string b)
{
    string modifiedBinary=a;
    for(int i=0; i<a.size(); ++i)
    {
        if(a[i]!=b[i])
            modifiedBinary[i]='-';
    }
    return modifiedBinary;
}

set<pi> GetAllImplicants(const vector<pi_group> &IGroup) {
    set<pi> PrimeImplicants;
    for (auto group : IGroup) {
        for (auto element : group) {
            PrimeImplicants.insert(element);
        }
    }
    return PrimeImplicants;
}

pi_group Generating_Groups(vector<pi_group> IGroup)
{
	vector<pi_group> groupedElements;
    set<pi> PrimeImplicants = GetAllImplicants(IGroup);

	//IGroup: Implicant Groups
	for (int i = 1; i < IGroup.size(); ++i) {
        // group i - 1, i
        // for each element in group i, we compare it to each element in i + 1
		set<pi> new_group;
        for(auto j: IGroup[i - 1])
        {
            for(auto k: IGroup[i])
            {
                if(compare_distance(j.second , k.second))
                {
                    // grouping the variables but we need to store them somewhere  
                    string groupedBinary= group(j.second,k.second);
					auto grouped_minterms = j.first;
					grouped_minterms.insert(k.first.begin(), k.first.end());
                    new_group.insert({grouped_minterms, groupedBinary});

                    // since the two elements are merged, we remove them from the prime implicants set
                    PrimeImplicants.erase(j);
                    PrimeImplicants.erase(k);
				}
				// I think we need to print them
			}
        }
		if (!new_group.empty()) {
			groupedElements.push_back(new_group);
		}
    }

    if (!groupedElements.empty()) {
        auto PI = Generating_Groups(groupedElements);
        PI.insert(PrimeImplicants.begin(), PrimeImplicants.end());
        return PI;
    }

	return pi_group(PrimeImplicants.begin(), PrimeImplicants.end());
}

pi_group generate_PI(vector<int> minterms, int literals)
{
    auto implicants = BinaryRepresentation(minterms, literals);
    vector<pi_group> PI_GROUPS(literals + 1);
    vector<pi> PI;
    // groupedElements;
   

    for (auto implicant : implicants) {
        // PI_GROUPS[count of ones].push_back(integer value of implicant, binary representation)
        PI_GROUPS[implicant.second.second].insert({{implicant.first}, implicant.second.first});
    }

	 return Generating_Groups(PI_GROUPS);



    /*
    -- split minterms to groups depending on countone
    -- for each two consecutive groups, we compare their binary representation
    -- If the distance between two elements is 1, we group them together in a map<
    */  
}

void printPIS(vector<pi> PrimeImplicants)
{
	pi_group PImplicants;
	for(auto p:PImplicants)
    {
    for(auto d: p.first) {
        cout << d << ' ';
    }
    cout << '\n';
}
}

