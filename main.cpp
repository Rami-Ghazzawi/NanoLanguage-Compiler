#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cctype>

using namespace std;
// -----------------------------------------------
// Homework 2
enum class TokenType
{
    Keyword,
    Identifier,
    IntegerLiteral,
    FloatLiteral,
    ArithmeticOperator,
    AssignmentOperator,
    RelationalOperator,
    LogicalOperator,
    Delimiter,
    Unknown
};
// ---------------------------------
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
        return "ID";

    case TokenType::IntegerLiteral:
        return "INT_LITERAL";

    case TokenType::FloatLiteral:
        return "FLOAT_LITERAL";

    case TokenType::ArithmeticOperator:
        return "ARITH_OP";

    case TokenType::AssignmentOperator:
        return "ASSIGN_OP";

    case TokenType::RelationalOperator:
        return "REL_OP";

    case TokenType::LogicalOperator:
        return "LOGIC_OP";

    case TokenType::Delimiter:
        return "DELIM";

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
        "let", "func", "if", "else", "while",
        "return", "print", "read", "int", "float"};

    unordered_set<char> delimiters = {
        '(', ')', '{', '}', '[', ']',
        ';', ','};

    unordered_set<string> arithmeticOperators = {
        "+", "-", "*", "/", "%"};

    unordered_set<string> relationalOperators = {
        "==", "!=", "<", ">", "<=", ">="};

    unordered_set<string> logicalOperators = {
        "&&", "||", "!"};

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
                    tokens.push_back({TokenType::FloatLiteral,
                                      number});
                }
                else
                {
                    tokens.push_back({TokenType::IntegerLiteral,
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

            // Check two-character operators
            if (i + 1 < line.length())
            {
                string twoChars = line.substr(i, 2);

                if (relationalOperators.count(twoChars))
                {
                    tokens.push_back({TokenType::RelationalOperator,
                                      twoChars});

                    i += 2;
                    continue;
                }

                if (logicalOperators.count(twoChars))
                {
                    tokens.push_back({TokenType::LogicalOperator,
                                      twoChars});

                    i += 2;
                    continue;
                }
            }

            // Check single-character operators
            string op(1, line[i]);

            if (arithmeticOperators.count(op))
            {
                tokens.push_back({TokenType::ArithmeticOperator,
                                  op});

                i++;
                continue;
            }

            if (op == "=")
            {
                tokens.push_back({TokenType::AssignmentOperator,
                                  op});

                i++;
                continue;
            }

            if (relationalOperators.count(op))
            {
                tokens.push_back({TokenType::RelationalOperator,
                                  op});

                i++;
                continue;
            }

            if (logicalOperators.count(op))
            {
                tokens.push_back({TokenType::LogicalOperator,
                                  op});

                i++;
                continue;
            }

            // Unknown character
            tokens.push_back({TokenType::Unknown,
                              op});

            i++;
        }
    }

    file.close();

    // Print tokens
    for (const Token &token : tokens)
    {
        cout << tokenTypeToString(token.type)
             << "\t"
             << token.lexeme
             << endl;
    }

    return 0;
}