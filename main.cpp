#include <iostream>
#include <string>
#include <vector>
#include <format>
#include <utility>
using namespace std;

const string INT = "INT";
const string FLOAT = "FLOAT";
const string PLUS = "PLUS";
const string MINUS = "MINUS";
const string MUL = "MUL";
const string DIV = "DIV";
const string MOD = "MOD";
const string LPAREN = "LPAREN";
const string RPAREN = "RPAREN";

class Token {
    public:
        string type;
        bool hasValue;
        int intValue;
        float floatValue;

        Token(string tokenType) {
            type = tokenType;
            hasValue = false;
        }

        Token(string tokenType, int value) {
            type = tokenType;
            hasValue = true;
            intValue = value;
        }

        Token(string tokenType, float value) {
            type = tokenType;
            hasValue = true;
            floatValue = value;
        }

        string formatted() {
            string result = type;
            if (hasValue) {
                result += ":";
                if (type == INT) result += to_string(intValue);
                else result += to_string(floatValue);
            }
            return result;
        }
};

Token* EMPTY = new Token("NULL");

struct Result {

};

class Lexer {
    public:
        string text;

        int index;
        char currentChar;

        Lexer(string txt) {
            text = txt;
            index = -1;
            currentChar = NULL;
            advance();
        }

        void advance() {
            index += 1;
            currentChar = index < text.length() ? text[index] : NULL;
        }

        pair<vector<Token*>, bool> makeTokens() {
            vector<Token*> tokens = {};
            pair<Token*, bool> result;

            while (currentChar != NULL) {
                if (currentChar == ' ' || currentChar == '\n') {
                    advance();
                } else if (currentChar == '+') {
                    tokens.push_back(new Token(PLUS));
                    advance();
                } else if (currentChar == '-') {
                    tokens.push_back(new Token(MINUS));
                    advance();
                } else if (currentChar == '*') {
                    tokens.push_back(new Token(MUL));
                    advance();
                } else if (currentChar == '/') {
                    tokens.push_back(new Token(DIV));
                    advance();
                } else if (currentChar == '%') {
                    tokens.push_back(new Token(MOD));
                    advance();
                } else if (currentChar == '(') {
                    tokens.push_back(new Token(LPAREN));
                    advance();
                } else if (currentChar == ')') {
                    tokens.push_back(new Token(RPAREN));
                    advance();
                } else if (isdigit(currentChar)) {
                    result = makeNumber();
                    if (result.second) return {{}, true};
                    tokens.push_back(result.first);
                } else {
                    return {{}, true};
                }
            }
            return {tokens, false};
        }

        pair<Token*, bool> makeNumber() {
            pair<Token*, bool> p;
            string result = "";
            int decimals = 0;
            while (currentChar != NULL && (isdigit(currentChar) || currentChar == '.')) {
                if (isdigit(currentChar)) result += currentChar;
                else if (currentChar == '.') {
                    decimals += 1;
                    result += currentChar;
                }
                advance();
            }

            if (decimals == 0) {
                p.first = new Token(INT, stoi(result));
                p.second = false;
            } else if (decimals == 1) {
                p.first = new Token(FLOAT, stof(result));
                p.second = false;
            } else {
                p.first = EMPTY;
                p.second = true;
            }
            return p;
        }
};

template <class T>
class UnaryOpNode {
    public:
        T node;
        Token* opToken;

        UnaryOpNode(T n; Token* op) {
            node = n;
            opToken = op;
        }

        string formatted() {
            if (opToken->type == PLUS) {
                return node.formatted();
            }
            return "(-" + node.formatted() + ")";
        }
};

template <typename T>
class NumberNode {
    public:
        T value;

        NumberNode(T val) {
            value = val;
        }

        string formatted() {
            return to_string(value);
        }
};

template <typename T, typename U>
class BinaryOpNode {
    public:
        NumberNode<T> left;
        NumberNode<U> right;
        Token* opToken;

        BinaryOpNode(NumberNode<T> l, NumberNode<U> r, Token* op) {
            left = l;
            right = r;
            opToken = op;
        }

        string formatted() {
            string l = left.formatted();
            string r = right.formatted();
            string op;
            if (opToken == PLUS) op = "+";
            else if (opToken == MINUS) op = "-";
            else if (opToken == MUL) op = "*";
            else if (opToken == DIV) op = "/";
            else if (opToken == MOD) op = "%";

            return "(" + l + op + r + ")";
        }
};

class Parser {
    public:
        vector<Token*> tokens;
        int index;
        Token* token;

        Parser(vector<Token*> toks) {
            tokens = toks;
            index = -1;
            token = EMPTY;
            advance();
        }

        void advance() {
            index += 1;
            token = index < tokens.size() ? tokens[index] : EMPTY;
        }


};

void run() {
    string input;
    pair<vector<Token*>, bool> lexerRes;
    vector<Token*> tokens;
    bool error;

    while (true) {
        cout << ">>> ";
        getline(cin, input);
        if (input.empty()) continue;

        Lexer* lexer = new Lexer(input);
        lexerRes = lexer->makeTokens();

        tokens = lexerRes.first;
        error = lexerRes.second;

        if (error) {
            cout << "Error";
            continue;
        }
        
        for (Token* token : tokens) {
            cout << token->formatted() << ", ";
        }
        cout << endl;
    }
}

int main() {
    run();    
    return 0;
}