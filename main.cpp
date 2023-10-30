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


int countOnesInBinary(const string& binary) {
    return count(binary.begin(), binary.end(), '1');
}


pi selectCheapestImplicant(const pi_group& implicants) {
    pi cheapestImplicant;
    int maxMinterms = -1;
    int maxOnes = -1;

    for (const pi& implicant : implicants) {
        int minterms = implicant.first.size();
        int ones = countOnesInBinary(implicant.second);

        if (minterms > maxMinterms || (minterms == maxMinterms && ones > maxOnes)) {
            maxMinterms = minterms;
            maxOnes = ones;
            cheapestImplicant = implicant;
        }
    }

    return cheapestImplicant;
}



int calculateScore(const pi& implicant) {
    
    int mintermScore = implicant.first.size();

    return mintermScore;
}

pi_group findNonEssentialCoveringAllMinterms(const pi_group& nonEssential, const set<int>& uncoveredMinterms) {
    pi_group selectedImplicants;
    set<int> remainingUncoveredMinterms = uncoveredMinterms;
    int previousRemainingCount = remainingUncoveredMinterms.size(); // Initialize previous count

    while (!remainingUncoveredMinterms.empty()) {
        int maxScore = -1;
        pi_group maxScoreImplicants;

        // Iterate through non-essential prime implicants to calculate their scores
        for (const pi& nonEssentialPI : nonEssential) {
            int score = calculateScore(nonEssentialPI);

            if (score > maxScore) {
                maxScore = score;
                maxScoreImplicants.clear();
                maxScoreImplicants.insert(nonEssentialPI);
            }
            else if (score == maxScore) {
                maxScoreImplicants.insert(nonEssentialPI);
            }
        }

        if (maxScoreImplicants.empty()) {
            // If no implicants with the maximum score were found, exit the loop
            break;
        }

        pi selectedImplicant = selectCheapestImplicant(maxScoreImplicants);
        selectedImplicants.insert(selectedImplicant);

        for (int minterm : selectedImplicant.first) {
            remainingUncoveredMinterms.erase(minterm);
        }

        if (remainingUncoveredMinterms.size() == previousRemainingCount) {
            // If no progress is being made, exit the loop to avoid infinite loop
            break;
        }

        previousRemainingCount = remainingUncoveredMinterms.size();
    }

    return selectedImplicants;
}








//pi_group findNonEssentialCoveringAllMinterms(const pi_group& nonEssential, const set<int>& uncoveredMinterms) {
//
//    set<int> remainingUPI = uncoveredMinterms;
//    pi_group selectedImplicants;
//
//    while (!remainingUPI.empty()) {
//
//        int maxScore = -1;
//
//
//        // Iterate through non - essential prime implicants to calculate their scores
//            for (const pi& nonEssentialPI : nonEssential) {
//                int score = calculateScore(nonEssentialPI);
//
//                if (score > maxScore) {
//                    maxScore = score;
//                    maxScoreImplicants.clear();
//                    maxScoreImplicants.insert(nonEssentialPI);
//                }
//                else if (score == maxScore) {
//                    maxScoreImplicants.insert(nonEssentialPI);
//                }
//            }
//
//        //iterating through the non essential prime implicants:
//        for (const pi& nonEssentialPI : nonEssential) {
//            if (score > maxScore) {
//                //we also need to check before we clear that the elements we are deleting do not contain uncovered implicants that are not covered by nonEssential
//                selectedImplicants.clear();
//                selectedImplicants.insert(nonEssentialPI);
//            }
//        
//        }
//
//
//
//    }
//
//}

// Custom comparison function for the priority queue
struct ComparePI {
    bool operator()(const pi& lhs, const pi& rhs) const {
        // 1. Compare by the number of minterms covered (largest set first)
        if (lhs.first.size() != rhs.first.size()) {
            return lhs.first.size() < rhs.first.size();
        }

        // 2. If the number of minterms covered is the same, compare by the overall number of implicants
        if (lhs.first.size() == rhs.first.size()) {
            int lhsImplicantCount = count(lhs.second.begin(), lhs.second.end(), '1');
            int rhsImplicantCount = count(rhs.second.begin(), rhs.second.end(), '1');
            if (lhsImplicantCount != rhsImplicantCount) {
                return lhsImplicantCount < rhsImplicantCount;
            }
        }

        // 3. If both the set size and the number of implicants are the same, compare by the binary representation
        return lhs.second < rhs.second;
    }
};

// Helper function that returns a priority queue of prime implicants based on the conditions
priority_queue<pi, vector<pi>, ComparePI> prioritizePI(const pi_group& nonEssential, const set<int>& uncoveredMinterms) {
    // Initialize the priority queue with the custom comparison function
    priority_queue<pi, vector<pi>, ComparePI> pq;

    // Iterate through non-essential prime implicants
    for (const pi& nonEssentialPI : nonEssential) {
        // Calculate the count of uncovered minterms covered by this implicant
        int countCovered = 0;
        for (int minterm : nonEssentialPI.first) {
            if (uncoveredMinterms.find(minterm) != uncoveredMinterms.end()) {
                countCovered++;
            }
        }

        // Only insert implicants that cover at least one uncovered minterm
         if (countCovered > 0) {
            pq.push(nonEssentialPI);
        }
    }

    return pq;
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


    //// Find non-essential prime implicants covering all uncovered minterms
    //pi_group selectedImplicants = findNonEssentialCoveringAllMinterms(nonEssentialPrimeImplicants, uncoveredMintermsSet);


    //// Print the selected non-essential prime implicants represented by minterms
    //cout << "Selected Non-Essential Prime Implicants:" << endl;
    //for (const pi& selectedImplicant : selectedImplicants) {
    //    cout << "Minterms: ";
    //    for (int minterm : selectedImplicant.first) {
    //        cout << minterm << ' ';
    //    }
    //    cout << " (Binary: " << selectedImplicant.second << ")" << endl;
    //}


    priority_queue<pi, vector<pi>, ComparePI> pq = prioritizePI(nonEssentialPrimeImplicants, uncoveredMintermsSet);

    // Now, the priority queue `pq` contains prime implicants ordered according to the specified conditions
    while (!pq.empty()) {
        pi topPI = pq.top();
        cout << "Binary Representation: " << topPI.second << " | Minterms: ";
        for (int minterm : topPI.first) {
            cout << minterm << ' ';
        }
        cout << endl;
        pq.pop();
    }

    return 0;
}
    

