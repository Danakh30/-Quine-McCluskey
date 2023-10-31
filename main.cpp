#include <iostream>
#include <unordered_set>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <map>
#include <algorithm>
#include <bitset>
#include <bitset>
#include <queue>
using namespace std;


typedef pair<set<int>, string> pi; // minterms, binary representation
typedef set<pi> pi_group; // collection of pi

map<int, pair<string, int>> BinaryRepresentation(const vector<int>& minterms, int literals)
{
    map<int, pair<string, int>> implicant;
    for (int i = 0; i < minterms.size(); i++)
    {
        string binarystring = bitset<32>(minterms[i]).to_string().substr(32 - literals, literals);
        int countone = count(binarystring.begin(), binarystring.end(), '1');
        implicant[minterms[i]] = { binarystring,countone };
        // printf("%d\t%s\n",countone,binarystring.c_str());
    }
    return implicant;
}

bool compare_distance(string a, string b)
{
    int count = 0;
    for (int i = 0; i < a.size(); ++i)
    {
        if (a[i] != b[i])
        {
            count++;
        }
    }
    return count == 1;

}

string group(string a, string b)
{
    string modifiedBinary = a;
    for (int i = 0; i < a.size(); ++i)
    {
        if (a[i] != b[i])
            modifiedBinary[i] = '-';
    }
    return modifiedBinary;
}

set<pi> GetAllImplicants(const vector<pi_group>& IGroup) {
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
        for (auto j : IGroup[i - 1])
        {
            for (auto k : IGroup[i])
            {
                if (compare_distance(j.second, k.second))
                {
                    // grouping the variables but we need to store them somewhere  
                    string groupedBinary = group(j.second, k.second);
                    auto grouped_minterms = j.first;
                    grouped_minterms.insert(k.first.begin(), k.first.end());
                    new_group.insert({ grouped_minterms, groupedBinary });

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
        // PI_GROUPS[count of ones].push_bacnk(integer value of implicant, binary representation)
        PI_GROUPS[implicant.second.second].insert({ {implicant.first}, implicant.second.first });
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
    for (auto p : PImplicants)
    {
        for (auto d : p.first) {
            cout << d << ' ';
        }
        cout << '\n';
    }
}




///////////////////
//osswa
///////////////////


//finding the occurances of the minterms 

//takes in the pi_group of the prime implicants and creates a map where the key is the minterm and the values are pairs
//the first element = #occurances       second element = indicies of the prime implicants where it is found

map<int, pair<int, set<int>>> generateEPI(const pi_group& primeImplicants) {
    
    map<int, pair<int, set<int>>> mintermOccurrences;


    //Loop through each prime implicant in the pi_group
    for (int i = 0; i < primeImplicants.size(); i++) {
        const pi& primeImplicant = *next(primeImplicants.begin(), i);


        //Within each prime implicant, loop through the minterms in that prime implicant.
        for (int minterm : primeImplicant.first) {
            
            //Check if the minterm is already in the mintermOccurrences map.If it's not, 
            //create a new entry for the minterm with an overall count of 1 and a set containing the index of the current prime implicant.

            //If the minterm is already in the map(i.e., it has occurred in a previous prime implicant), 
            //increment its overall countand add the index of the current prime implicant to the set.
            if (mintermOccurrences.find(minterm) == mintermOccurrences.end()) {
                mintermOccurrences[minterm] = make_pair(1, set<int>{i});
            }
            else {
                mintermOccurrences[minterm].first++;
                mintermOccurrences[minterm].second.insert(i);
            }
        }
    }

    return mintermOccurrences;
}


pi_group EPI_generator(const pi_group& primeImplicants, const map<int, pair<int, set<int>>>& mintermOccurrences) {
    pi_group EPI;

    for (const auto& entry : mintermOccurrences) {
        int minterm = entry.first;
        int occurrenceCount = entry.second.first;

        if (occurrenceCount == 1) {
            const set<int>& primeImplicantIndices = entry.second.second;

            // Iterate through the prime implicant indices and extract corresponding prime implicants
            for (int primeImplicantIndex : primeImplicantIndices) {
                const pi& primeImplicant = *next(primeImplicants.begin(), primeImplicantIndex);
                EPI.insert(primeImplicant);
            }
        }
    }

    return EPI;
    ;
}


void epi_printer(pi_group &epis) {
    

    cout << "\nEssential Prime Implicants:" << endl;
    for (const pi& essential : epis) {
        for (int minterm : essential.first) {
            cout << minterm << ' ';
        }
        cout << " (" << essential.second << ")" << endl;
    }
}


pi_group nonEPI_extractor(const pi_group& allPI, const pi_group& epis) {
    pi_group nonEssentialPrimeImplicants;
    set<string> uniqueBinaryRepresentations;

    for (const pi& PI : allPI) {
        if (epis.find(PI) == epis.end()) {
            // Check if the binary representation is unique
            if (uniqueBinaryRepresentations.find(PI.second) == uniqueBinaryRepresentations.end()) {
                nonEssentialPrimeImplicants.insert(PI);
                uniqueBinaryRepresentations.insert(PI.second);
            }
        }
    }

    return nonEssentialPrimeImplicants;
}


set<int> uncoveredMinterms(const pi_group& nonEssential, const pi_group& essential) {
    set<int> essentialMinterms;

    // Extract all minterms covered by essential prime implicants
    for (const pi& essentialPI : essential) {
        for (int minterm : essentialPI.first) {
            essentialMinterms.insert(minterm);
        }
    }

    set<int> uncoveredMintermSet;

    // Iterate through non-essential prime implicants
    for (const pi& nonEssentialPI : nonEssential) {
        for (int minterm : nonEssentialPI.first) {
            if (essentialMinterms.find(minterm) == essentialMinterms.end()) {
                // Minterm is not covered by essential prime implicants
                uncoveredMintermSet.insert(minterm);
            }
        }
    }

    return uncoveredMintermSet;
}

set<string>  transformToBoolian(set<char> vars, pi_group pi) {

    set<string> epi;
    string result = "";

    for (const auto& p : pi) {
        string val = p.second;
        string term = "";

        for (char var : vars) {
            if (val.empty()) {
                break; // Stop if there are no more characters in val
            }

            if (val[0] == '1') {
                term = term + var;
            }
            else if (val[0] == '0') {
                term = term + var + "'";
            }

            val = val.substr(1);

            if (!val.empty() && val[0] == '-') {
                val = val.substr(1); // Skip '-' character
            }
        }

        if (!term.empty()) {
            epi.insert(term);
        }
    }

   

    return epi;
}



//the same function as above but takes in a vector (for the set of selected non prime implicants)
std::set<std::string> transformToBooleanVec(const std::set<char>& vars, const std::vector<pi>& piVector) {
    std::set<std::string> epi;

    for (const pi& p : piVector) {
        std::string val = p.second;
        std::string term = "";

        for (char var : vars) {
            if (val.empty()) {
                break; // Stop if there are no more characters in val
            }

            if (val[0] == '1') {
                term = term + var;
            }
            else if (val[0] == '0') {
                term = term + var + "'";
            }

            val = val.substr(1);

            if (!val.empty() && val[0] == '-') {
                val = val.substr(1); // Skip '-' character
            }
        }

        if (!term.empty()) {
            epi.insert(term);
        }
    }

    return epi;
}




// Custom comparison function for sorting by the number of covered minterms
// Custom comparison function for sorting by coverage, total implicants, and binary representation
struct ComparePrimeImplicants {
    bool operator()(const pair<pi, int>& lhs, const pair<pi, int>& rhs) const {
        // Compare by coverage
        if (lhs.second != rhs.second) {
            return lhs.second > rhs.second;
        }
        // If coverage is the same, compare by total implicants
        int lhsTotalImplicants = lhs.first.first.size();
        int rhsTotalImplicants = rhs.first.first.size();
        if (lhsTotalImplicants != rhsTotalImplicants) {
            return lhsTotalImplicants > rhsTotalImplicants;
        }
        // If total implicants are the same, compare by the number of 1's in binary representation
        int lhsOnesCount = count(lhs.first.second.begin(), lhs.first.second.end(), '1');
        int rhsOnesCount = count(rhs.first.second.begin(), rhs.first.second.end(), '1');
        return lhsOnesCount > rhsOnesCount;
    }
};

// Function to sort and return a vector of prime implicants based on coverage, total implicants, and binary representation
vector<pair<pi, int>> sortPrimeImplicants(const pi_group& nonEssential, const set<int>& uncoveredMinterms) {
    vector<pair<pi, int>> sortedPIs;

    // Iterate through non-essential prime implicants
    for (const pi& nonEssentialPI : nonEssential) {
        int countCovered = 0;

        // Calculate the count of uncovered minterms covered by this implicant
        for (int minterm : nonEssentialPI.first) {
            if (uncoveredMinterms.find(minterm) != uncoveredMinterms.end()) {
                countCovered++;
            }
        }

        // Only insert implicants that cover at least one uncovered minterm
        if (countCovered > 0) {
            sortedPIs.push_back(make_pair(nonEssentialPI, countCovered));
        }
    }

    // Sort the vector based on the custom comparison function
    sort(sortedPIs.begin(), sortedPIs.end(), ComparePrimeImplicants());

    return sortedPIs;
}



// Function to select prime implicants one by one, remove covered minterms, and create a vector of selected prime implicants
vector<pi> selectPrimeImplicants(const pi_group& nonEssential, set<int> uncoveredMinterms) {
    vector<pi> selectedPIs;
    vector<pair<pi, int>> sortedPIs = sortPrimeImplicants(nonEssential, uncoveredMinterms);

    while (!uncoveredMinterms.empty() && !sortedPIs.empty()) {
        // Select the prime implicant with the highest priority
        const pi& selectedImplicant = sortedPIs[0].first;
        int coverage = sortedPIs[0].second;

        // Add the selected implicant to the list of selected implicants
        selectedPIs.push_back(selectedImplicant);

        // Remove the minterms covered by the selected implicant from the set of uncovered minterms
        for (const auto& minterm : selectedImplicant.first) {
            auto it = uncoveredMinterms.find(minterm);
            if (it != uncoveredMinterms.end()) {
                uncoveredMinterms.erase(it);
            }

            
        }

        // Update the vector of sorted prime implicants
        sortedPIs = sortPrimeImplicants(nonEssential, uncoveredMinterms);
    }

    return selectedPIs;
}
//this function selects prime implicants until there are no uncovered minterms left.
//The function returns a vector of selected prime implicants based on the specified conditions.
//Each iteration selects the highest - priority prime implicant, removes the covered minterms, 
//and updates the list of sorted prime implicants for the next iteration.



//the final functio that will give you the optamized boolian function
string finalBoolianFunction(pi_group ePI, pi_group nEPI , vector<pi> neededPI, set<int> uncoveredMintermsSet, set<char> vars) {
    
    set<string>epii = transformToBoolian(vars, ePI);
    set<string>nepii = transformToBooleanVec(vars, neededPI);


    string bf = "";
    for (const auto& imp : epii) {
        bf = bf + imp + " + ";
    }

    vector<pi> selectedPIs = selectPrimeImplicants(nEPI, uncoveredMintermsSet);
    for (const auto& imp : nepii) {
        bf = bf + imp + " + ";
    }
    
    return bf.substr(0, bf.size() - 2);

}

int main() {

    set<char>vars = { 'A','B','C','D','E' };
    
    auto pimyn = generate_PI({ 0,2,3,5,8,10,16,17,18,19,21,24,30,31,32 }, 5);
    for (auto p : pimyn)
    {
        for (auto d : p.first) {
            cout << d << ' ';
        }
        cout << '\n';
    }
    map<int, pair<int, set<int>>> mintermOccurrences = generateEPI(pimyn);

    pi_group essentialPrimeImplicants = EPI_generator(pimyn, mintermOccurrences);
    cout << "\nEssential Prime Implicants:" << endl;
    for (const pi& essential : essentialPrimeImplicants) {
        for (int minterm : essential.first) {
            cout << minterm << ' ';
        }
        cout << " (" << essential.second << ")" << endl;
    }

    // Extract Non-Essential Prime Implicants
    pi_group nonEssentialPrimeImplicants = nonEPI_extractor(pimyn, essentialPrimeImplicants);
    cout << "\nNon-Essential Prime Implicants:" << endl;
    for (const pi& nonEssential : nonEssentialPrimeImplicants) {
        for (int minterm : nonEssential.first) {
            cout << minterm << ' ';
        }
        cout << " (" << nonEssential.second << ")" << endl;
    }
    
    // Find and print Uncovered Minterms
    set<int> uncoveredMintermsSet = uncoveredMinterms(nonEssentialPrimeImplicants, essentialPrimeImplicants);
    cout << "\nUncovered Minterms:" << endl;
    for (int minterm : uncoveredMintermsSet) {
        cout << minterm << ' ';
    }
    cout << endl;


    //testing the printing of the epis

    set<string>epii= transformToBoolian(vars, essentialPrimeImplicants);
    cout << "Set of Terms:" << endl;
    for (const string& term : epii) {
        cout << term << " | ";
    }


     //vector<pair<pi, int>> sortedPIs = sortPrimeImplicants(nonEssentialPrimeImplicants, uncoveredMintermsSet);

    vector<pi> selectedPIs = selectPrimeImplicants(nonEssentialPrimeImplicants, uncoveredMintermsSet);

    // Print the selected prime implicants
    cout << "Selected Prime Implicants:" << endl;
    for (const pi& selectedImplicant : selectedPIs) {
        for (int minterm : selectedImplicant.first) {
            cout << minterm << ' ';
        }
        cout << " (" << selectedImplicant.second << ")" << endl;
    }

        //testing the printing of the final funtion
    cout << "final function will look like : ";
    cout << finalBoolianFunction(essentialPrimeImplicants, nonEssentialPrimeImplicants, selectedPIs, uncoveredMintermsSet, vars);
    return 0;
}
    

