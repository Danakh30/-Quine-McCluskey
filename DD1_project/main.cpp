#include "circuit.cpp"



int main() {

std::vector<std::string> logicalExpressions = {\
    "(A' + B)(A + B')",\
    "(P + Q)(P' + Q')",\
    "(A' + B + C')(A' + B' + C)",\
    "P'Q + PQ' + PQ",\
    "(A' + B + C)(A + B' + C' + D')",\
    "(P  Q'  R) + (P'  Q  R') + (P  Q  R  S')",\
    "(A  B'  C  D'  E') + (A'  B  C'  D  E)",\
    "P'  Q  R  S  + P  Q'  R'+ S T U",\
    "A  B + A'  B +  A  C'",\
    "(A' + B + C)(P' + Q')"};

    for (const std::string& expr : logicalExpressions){
        string flag = "0";
       // while (flag == "0")
        // {
        //     // cout << "Enter your function in SoP or PoS form: ";
        //     // getline(cin, f);
        //     f=expr;
        //     flag = checkValidity();
        // }
        f=expr;
        flag = checkValidity();
        generateTT(flag);


        auto pimyn = generate_PI(minterms, vars.size());
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

        set<string>epii = transformToBoolian(vars, essentialPrimeImplicants);
        cout << "Set of Terms:" << endl;
        for (const string& term : epii) {
            cout << term << " | "<<endl;
        }


        cout<<endl;
        //vector<pair<pi, int>> sortedPIs = sortPrimeImplicants(nonEssentialPrimeImplicants, uncoveredMintermsSet);

        vector<pi> selectedPIs = selectPrimeImplicants(nonEssentialPrimeImplicants, uncoveredMintermsSet);
        set<pi> selectedPIsSet(selectedPIs.begin(), selectedPIs.end());

        // Print the selected prime implicants
        cout << "Selected Prime Implicants:" << endl;
        for (const pi& selectedImplicant : selectedPIs) {
            for (int minterm : selectedImplicant.first) {
                cout << minterm << ' ';
            }
            cout << " (" << selectedImplicant.second << ")" << endl;
        }

            cout<<endl;


        //testing the printing of the final funtion
        cout << "final function will look like : ";
        cout << finalBoolianFunction(essentialPrimeImplicants, nonEssentialPrimeImplicants, selectedPIs, uncoveredMintermsSet, vars);



        //kmap representation
        cout << endl << endl;

        if(!(vars.size()>4)){
            for (auto imp : essentialPrimeImplicants) {

                cout << "kmap for each implicant" << endl;

                std::vector<std::vector<char>> karnaughMap = CreateKmap(imp.first, vars.size());

                // Print the Karnaugh Map
                for (int i = 0; i < karnaughMap.size(); ++i) {
                    for (int j = 0; j < karnaughMap[i].size(); ++j) {
                        std::cout << karnaughMap[i][j] << " ";
                    }
                    std::cout << std::endl;
            }

        }
    }
        
        if(!(vars.size()>4))
        {
        for (auto imp : selectedPIsSet) {

            cout << "kmap for each implicant" << endl;

            std::vector<std::vector<char>> karnaughMap = CreateKmap(imp.first, vars.size());

            // Print the Karnaugh Map
            for (int i = 0; i < karnaughMap.size(); ++i) {
                for (int j = 0; j < karnaughMap[i].size(); ++j) {
                    std::cout << karnaughMap[i][j] << " ";
                }
                std::cout << std::endl;
            }

        }
        }

        std::vector<std::string> terms = boolianTerms(essentialPrimeImplicants, nonEssentialPrimeImplicants, selectedPIs, vars);
        circuit(terms);
        }
    return 0;

}