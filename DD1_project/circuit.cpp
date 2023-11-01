#include"PrimeManip.cpp"
#include<fstream>
using namespace std;

bool isNegated(int index, const string& str)
{
    if (index + 1 == str.size())
    {
        return false;
    }

    return str[index + 1] == '\'';
}

string create_script(vector<string> sop)
{
    string script = " { assign: [[\"out\",";

    if (sop.size() > 1) {
        script += "[\"|\"";
    }

    for (auto& product : sop)
    {
        script += ",\n[\"&\", ";
        for (int i = 0; i < product.size(); ++i)
        {
            if (isNegated(i, product))
            {
                script += "[\"~\", \"";
                script += product[i];
                script += string("\"]");
                ++i;
            }
            else
            {
                script += "\"";
                script += product[i];
                script += "\"";
            }
            if (i + 1 != product.size()) {
                script += ", ";
            }
        }
        script += "]\n";
    }

    if (sop.size() > 1) {
        script += "]";
    }

    script += "]]}";
    return script;
}

void circuit(vector<string> sop) 
{
    ofstream htmlFile("circuit.html");
    string script = create_script(sop);
    if (htmlFile.is_open()) 
    {
        htmlFile << "<!DOCTYPE html>\n";
        htmlFile << "<html>\n";
        htmlFile << "<head>\n";
        htmlFile << "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/wavedrom/3.1.0/skins/default.js\" type=\"text/javascript\"></script>\n";
        htmlFile << "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/wavedrom/3.1.0/wavedrom.min.js\" type=\"text/javascript\"></script>\n";
        htmlFile << "</head>\n";
        htmlFile << "<body onload=\"WaveDrom.ProcessAll()\">";
        htmlFile << "<script type=\"WaveDrom\">" << script;
        htmlFile << "</script>";
        htmlFile << "</body>\n";
        htmlFile << "</html>\n";

        htmlFile.close();
        cout << "HTML file created successfully!" << endl;
    } else {
        cerr << "Unable to create the file." << endl;
    }
}
