#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

std::map<std::string, std::string> keywordCategories = {
    {"int", "DT"}, {"char", "DT"},{"bool", "DT"}, {"if", "if"}, {"else", "else"},
    {"return", "return"}, {"break", "break"}, {"continue", "continue"}, {"new", "new"}, {"for", "for"}, 
    {"void", "void"}, {"public", "Access Specifier"},{"private","Access Specifier"},{"protected","Access Specifier"},{"class","class"},
    {"this","this"},{"default","default"},{"delete","delete"},{"static","static"},{"virtual","virtual"},
    {"enum","enum"},{"sizeof","sizeof"},{"main","main"}
};

std::map<std::string, std::string> operatorCategories = {
    {"+", "PM"}, {"-", "PM"}, {"*", "MDM"}, {"/", "MDM"}, {"%", "MDM"},
    {"&&", "LogAnd"}, {"||", "LogOR"}, {"!", "NOT"}, {"==", "REL OP"}, {"!=", "REL OP"},
    {"<=", "REL OP"}, {">=", "REL OP"}, {"<", "REL OP"}, {">", "REL OP"},
    {"=", "="}, {"*=", "*="}, {"+=", "ASSIGNMENT OP"}, {"-=", "ASSIGNMENT OP"},
    {"/=", "ASSIGNMENT OP"}, {"++", "INC/DEC"}, {"--", "INC/DEC"}, {"?", "Ternary OP"}
};

std::map<std::string, std::string> punctuatorCategories = {
    {";", ";"}, {"(", "("}, {")", ")"}, {"[", "["}, {"]", "]"},
    {"{", "{"}, {"}", "}"}, {":", ":"},{",", ","}, {"::", "::"}, {"~", "~"},
    {".", "DA"}, {"->", "DA"}
};

using std::cout;
using std::vector;

int currentLine = 1; // Initialize the current line number


bool isID(const std::string &str);
bool isComment(const std::string &str);
bool isDigit(const std::string &str);    
bool isString(const std::string &str);    
bool isBool(const std::string &str);
bool isIntConst(const std::string &str);
bool isStrConst(const std::string &str);
bool isCharConst(const std::string &str);
bool isKeyword(const std::string &str);
bool isOperator(const std::string &str);
bool isPunctuator(const std::string &str);
bool isNotLegal(const std::string &str);
void printRoleOfToken(const vector<std::string>& tokens);
void lexicalAnalyze(const std::string &nameOfFile, const std::string &outputFileName);


bool isID(const std::string &str)
{
    if(std::isdigit(str[0]))
        return false;
    int counter = 0;
    if(str[0] == '_')
        counter++;

    for(; counter < str.size(); counter++)
        if (!(isalnum(str[counter]) || str[counter] == '_'))
            return false;

    return true;
}

bool isComment(const std::string &str)
{
    return str == "`@" || str == "``";
}

bool isDigit(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

bool isString(const std::string &str)
{
    return str[0] == '"' && str[str.size()-1] == '"';
}

bool isBool(const std::string &str)
{
    return str == "true" || str == "false";
}

bool isIntConst(const std::string &str)
{
    return isDigit(str);
}

bool isStrConst(const std::string &str)
{
    return isString(str);
}

bool isCharConst(const std::string &str)
{
    // Check if the token is enclosed in single quotes
    if ((str.size() == 3 || str.size() > 4) && str.front() == '\'' && str.back() == '\'') {
        // Handle escaped characters if needed
        if (str[1] == '\\') {
            // Check for valid escaped character
            switch (str[2]) {
                case 'n': case 't': case 'r': case 'b':
                case 'f': case 'a': case 'v': case '?':
                case '\'': case '"': case '\\': case '0':
                    return true; // Valid escaped character
                default:
                    return false; // Invalid escaped character
            }
        } else {
            return true; // Non-escaped character
        }
    }
    
    return false;
}    



bool isKeyword(const std::string &str)
{
    return keywordCategories.find(str) != keywordCategories.end();
}

std::string getCategory(const std::string &token)
{
    auto it = keywordCategories.find(token);
    if (it != keywordCategories.end())
    {
        return it->second;
    }
    return "Unknown"; // Default category if not found
}



bool isOperator(const std::string &str)
{
    return operatorCategories.find(str) != operatorCategories.end();
}

std::string getOperatorCategory(const std::string &token)
{
    auto it = operatorCategories.find(token);
    if (it != operatorCategories.end())
    {
        return it->second;
    }
    return "Unknown"; // Default category if not found
}

bool isPunctuator(const std::string &str)
{
    return punctuatorCategories.find(str) != punctuatorCategories.end();
}

std::string getPunctuatorCategory(const std::string &token)
{
    auto it = punctuatorCategories.find(token);
    if (it != punctuatorCategories.end())
    {
        return it->second;
    }
    return "Unknown"; // Default category if not found
}

bool isNotLegal(const std::string &str)
{
    return str == " " || str == "\n";
}

void printRoleOfToken(const std::string& token)
{
    if (isOperator(token))
    {
        const std::string category = getOperatorCategory(token);
        std::cout << "(" << category << " , " << token << ", line " << currentLine << ")" << std::endl;
    }
    else if (isPunctuator(token))
    {
        const std::string category = getPunctuatorCategory(token);
        std::cout << "(" << category << " , " << token << ", line " << currentLine << ")" << std::endl;
    }
    else if (isKeyword(token))
    {
        const std::string category = getCategory(token);
        std::cout << "(" << category << " , " << token << ", line " << currentLine << ")" << std::endl;
    }

    
    else if(isIntConst(token))
        cout << "(Int Const, " << token << ", line " << currentLine << ")"<< std::endl;
    else if(isStrConst(token))
        cout << "(Str Const, " << token << ", line " << currentLine << ")"<< std::endl;
    else if(isCharConst(token))
        cout << "(Char Const, " << token << ", line " << currentLine << ")"<< std::endl;
    else if(isID(token))
        cout << "(identifier, " << token << ", line " << currentLine << ")"<< std::endl;
    else if(isComment(token))
        cout << "(comment, " << token << ", line " << currentLine << ")"<< std::endl;
    else
        cout << "(Invalid token, " << token << ", line " << currentLine << ")" << std::endl; // Handle invalid token
}
void lexicalAnalyze(const std::string &nameOfFile, const std::string &outputFileName) {
    std::fstream file(nameOfFile, std::fstream::in);

    if (!file.is_open())
    {
        cout << "error while opening the file\n";
        exit(0);
    }

    bool multiCm = false, singleCm = false;

    std::string buffer;
    char ch;

    while (file >> std::noskipws >> ch) {
        // Check for new line
        if (ch == '\n') {
            currentLine++;
        }
        if (singleCm || multiCm)
        {
            if (singleCm && ch == '\n')
                singleCm = false;

            if (multiCm && ch == '@')
            {
                file.get(ch);
                if (ch == EOF)
                    break;

                if (ch == '`')
                    multiCm = false;
            }
            continue;
        }

        if (ch == '`')
        {
            std::string comm(1, ch);
            file.get(ch);
            if (ch == EOF)
            {
                printRoleOfToken(comm);
                break;
            }

            if (ch == '@')
            {
                multiCm = true;
                comm += ch;
            }
            else if (ch == '`')
            {
                singleCm = true;
                comm += ch;
            }
            if (multiCm || singleCm)
            {
                printRoleOfToken(comm);
                continue;
            }
        }
        if (isNotLegal(std::string(1, ch)))
        {
            if (!buffer.empty())
            {
                printRoleOfToken(buffer);
                buffer = "";
            }
            continue;
        }
        // Check if the character is whitespace, operator, or punctuator
        if (std::isspace(ch) || isOperator(std::string(1, ch)) || isPunctuator(std::string(1, ch))) {
            if (!buffer.empty()) {
                printRoleOfToken(buffer);
                buffer.clear();
            }

            // Handle combined operators and punctuators
            std::string combinedOperator(1, ch);
            combinedOperator += file.peek(); // Peek at the next character
            if (isOperator(combinedOperator) || isPunctuator(combinedOperator)) {
                file.get(ch); // Consume the next character
                printRoleOfToken(combinedOperator);
            } else {
                printRoleOfToken(std::string(1, ch)); // Print the single operator or punctuator
            }
        }
        // Handle alphanumeric characters and underscores (part of identifiers)
        else if (std::isalnum(ch) || ch == '_') {
            buffer += ch;
        }
        // Handle string constants
        else if (ch == '"') {
            buffer += ch;
            while (file.get(ch) && ch != '"') {
                buffer += ch;
            }
            buffer += ch; // Include the closing double-quote

            printRoleOfToken(buffer);
            buffer.clear();
        }
        // Handle escaped characters in string constants
        else if (ch == '"') {
            buffer += ch;
            while (file.get(ch) && ch != '"') {
                buffer += ch;
                if (ch == '\\') {
                    buffer += '\\'; // Include the backslash
                    if (file.get(ch)) {
                        buffer += ch;
                    }
                }
            }
            buffer += ch; // Include the closing double-quote
            printRoleOfToken(buffer);
            buffer.clear();
        } 
        // Handle character constants
        else if (ch == '\'') {
        buffer += ch;
        bool escaped = false;
        
        while (file.get(ch) && (escaped || ch != '\'')) {
            buffer += ch;
            
            if (escaped) {
                // Interpret escape sequences
                switch (ch) {
                    case 'n': buffer += '\n'; break;
                    case 't': buffer += '\t'; break;
                    case 'r': buffer += '\r'; break;
                    case 'v': buffer += '\v'; break;
                    case 'f': buffer += '\f'; break;
                    case 'a': buffer += '\a'; break;
                    case '?': buffer += '\?'; break;
                    case '"': buffer += '\"'; break;
                    case '\\': buffer += '\\'; break;
                    // Add more cases for other escape sequences if needed
                    default: buffer += ch; break;
                }
                
                escaped = false;
            } else if (ch == '\\') {
                escaped = true;
            }
        }
        
        buffer += ch; // Include the closing single-quote
        
        printRoleOfToken(buffer);
        buffer.clear();
    }

        else {
            // Handle other cases (e.g., invalid characters)
            if (!buffer.empty()) {
                printRoleOfToken(buffer);
                buffer.clear();
            }
        }
    }
    if (!buffer.empty())
    {
        printRoleOfToken(buffer);
    }
    // Close the input and output files
    file.close();
   
}
int main()
{
  const std::string inputFileName = "program.txt";
    const std::string outputFileName = "output.txt";

    std::ofstream outputFile(outputFileName);
    if (!outputFile.is_open())
    {
        std::cout << "Error while opening the output file\n";
        return 1;
    }

    // Redirect the standard output to the output file
    std::streambuf* oldStdout = std::cout.rdbuf();
    std::cout.rdbuf(outputFile.rdbuf());

    // Perform lexical analysis
    lexicalAnalyze(inputFileName, outputFileName);

    // Restore the standard output
    std::cout.rdbuf(oldStdout);
    outputFile.close();

    return 0;
}
