#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>

using namespace std;

enum class TokenType
{
    Keyword,
    Identifier,
    Integer,
    Float,
    Operator,
    Delimiter,
    Unknown
};

struct Token
{
    TokenType type;
    string lexeme;
};

string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::Keyword:
        return "KEYWORD";
    case TokenType::Identifier:
        return "IDENTIFIER";
    case TokenType::Integer:
        return "INTEGER";
    case TokenType::Float:
        return "FLOAT";
    case TokenType::Operator:
        return "OPERATOR";
    case TokenType::Delimiter:
        return "DELIMITER";
    case TokenType::Unknown:
        return "UNKNOWN";
    }

    return "UNKNOWN";
}

int main()
{

    ifstream file("source.nl");

    if (!file.is_open())
    {
        cout << "Error: Could not open source.nl\n";
        return 1;
    }

    unordered_set<string> keywords = {
        "let", "if", "else", "while",
        "func", "return", "print", "read",
        "int", "float"};

    unordered_set<char> delimiters = {
        '(', ')', '{', '}', '[', ']',
        ';', ',', ':'};

    unordered_set<string> operators = {
        "+", "-", "*", "/", "%",
        "=", "==", "!=", "<", ">",
        "<=", ">=", "&&", "||", "!"};

    vector<Token> tokens;

    string line;

    while (getline(file, line))
    {

        size_t i = 0;

        while (i < line.length())
        {

            // Ignore whitespace
            if (isspace(line[i]))
            {
                i++;
                continue;
            }

            // Comment
            if (line[i] == '#')
            {
                break;
            }

            // Identifier or Keyword
            if (isalpha(line[i]) || line[i] == '_')
            {

                string word;

                while (i < line.length() &&
                       (isalnum(line[i]) || line[i] == '_'))
                {

                    word += line[i];
                    i++;
                }

                if (keywords.count(word))
                {
                    tokens.push_back({TokenType::Keyword,
                                      word});
                }
                else
                {
                    tokens.push_back({TokenType::Identifier,
                                      word});
                }

                continue;
            }

            // Number
            if (isdigit(line[i]))
            {

                string number;
                bool isFloat = false;

                while (i < line.length() && isdigit(line[i]))
                {
                    number += line[i];
                    i++;
                }

                // Check decimal point
                if (i < line.length() &&
                    line[i] == '.' &&
                    i + 1 < line.length() &&
                    isdigit(line[i + 1]))
                {

                    isFloat = true;
                    number += line[i];
                    i++;

                    while (i < line.length() &&
                           isdigit(line[i]))
                    {

                        number += line[i];
                        i++;
                    }
                }

                if (isFloat)
                {
                    tokens.push_back({TokenType::Float,
                                      number});
                }
                else
                {
                    tokens.push_back({TokenType::Integer,
                                      number});
                }

                continue;
            }

            // Delimiter
            if (delimiters.count(line[i]))
            {

                string delimiter(1, line[i]);

                tokens.push_back({TokenType::Delimiter,
                                  delimiter});

                i++;
                continue;
            }

            // Operators
            string op;

            if (i + 1 < line.length())
            {
                string twoChars = line.substr(i, 2);

                if (operators.count(twoChars))
                {

                    tokens.push_back({TokenType::Operator,
                                      twoChars});

                    i += 2;
                    continue;
                }
            }

            // Single character operator
            op = string(1, line[i]);

            if (operators.count(op))
            {

                tokens.push_back({TokenType::Operator,
                                  op});

                i++;
                continue;
            }

            // Unknown characters
            tokens.push_back({TokenType::Unknown,
                              string(1, line[i])});

            i++;
        }
    }

    file.close();
    for (const Token &token : tokens)
    {
        cout << tokenTypeToString(token.type)
             << "\t" << token.lexeme << endl;
    }

    return 0;
}