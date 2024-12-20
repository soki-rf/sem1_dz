#include <iostream>
#include <stdexcept>
#include <sstream>
#include <map>
#include <cctype>
#include <string>
#include <stack>
#include <cmath>

using namespace std;


bool check(const string& s) {
    int b = 0;
    for (int i = 0; i < s.length(); ++i) {
        char x = s[i];
        if (isspace(x)) continue;
        if (isdigit(x) || x == 'x' || x == '.' || x == '(' || x == ')' || x == '+' || x == '-' || x == '*' || x == '/') {
            if (x == '(') b++;
            if (x == ')') b--;
            if (b < 0) return false;
        }
        else if (isalpha(x)) {
            string func;
            while (i < s.length() && isalpha(s[i])) {
                func += s[i++];
            }
            --i;
            if (func != "sin" && func != "cos" && func != "tg" && func != "ctg" && func != "exp") {
                return false;
            }
        }
        else {
            return false;
        }
    }
    return b == 0;
}


double applyOperation(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': //
        if (b == 0) throw runtime_error("Error: Division by zero");
        return a / b;
    default: throw runtime_error("Invalid operation");
    }
}


double applyFunction(const string& s, double x) {
    if (s == "sin") return sin(x);
    if (s == "cos") return cos(x);
    if (s == "tg") return tan(x);
    if (s == "ctg") return 1.0 / tan(x);
    if (s == "exp") return exp(x);
    throw runtime_error("Error: Invalid operation");
}


string toRPN(const string& s) {

    map<string, int> precedence = {
        {"+", 1}, {"-", 1}, {"*", 2}, {"/", 2},
        {"sin", 3}, {"cos", 3}, {"tg", 3}, {"ctg", 3}, {"exp", 3}
    };

    stack<string> opr;
    stringstream res;

    for (size_t i = 0; i < s.length(); ++i) {
        if (isspace(s[i])) continue;

        if (isdigit(s[i]) || s[i] == 'x' || s[i] == '.') {
            while (i < s.length() && (isdigit(s[i]) || s[i] == 'x' || s[i] == '.')) {
                res << s[i++];
            }
            res << ' ';
            --i;
        }
        else if (isalpha(s[i])) {
            string func;
            while (i < s.length() && isalpha(s[i])) {
                func += s[i++];
            }
            --i;
            opr.push(func);
        }
        else if (s[i] == '(') {
            opr.push("(");
        }
        else if (s[i] == ')') {
            while (!opr.empty() && opr.top() != "(") {
                res << opr.top() << ' ';
                opr.pop();
            }
            if (!opr.empty() && opr.top() == "(") opr.pop();
            if (!opr.empty() && precedence.count(opr.top())) {
                res << opr.top() << ' ';
                opr.pop();
            }
        }
        else {
            string op(1, s[i]);
            while (!opr.empty() && precedence[opr.top()] >= precedence[op]) {
                res << opr.top() << ' ';
                opr.pop();
            }
            opr.push(op);
        }
    }

    while (!opr.empty()) {
        res << opr.top() << ' ';
        opr.pop();
    }

    return res.str();
}





double evaluateRPN(const string& rpn, double x) {
    stringstream ss(rpn);
    stack<double> values;
    string tkn;
    while (ss >> tkn) {
        if (isdigit(tkn[0]) || (tkn.size() > 1 && tkn[0] == '-')) {
            values.push(stod(tkn));
        }
        else if (tkn == "x") {
            values.push(x);
        }
        else if (tkn == "+" || tkn == "-" || tkn == "*" || tkn == "/") {
            if (values.size() < 2) throw runtime_error("Insufficient values for operation");
            double b = values.top(); values.pop();
            double a = values.top(); values.pop();
            values.push(applyOperation(a, b, tkn[0]));
        }
        else {
            if (values.empty()) throw runtime_error("Function argument missing");
            double a = values.top(); values.pop();
            values.push(applyFunction(tkn, a));
        }
    }
    if (values.size() != 1) throw runtime_error("Invalid expression");
    return values.top();
}




int main() {

    string expr;
    getline(cin, expr);

    if (!check(expr)) {
        cerr << "Error" << endl;
        return 1;
    }

    string rpn = toRPN(expr);

    double x = 0;
    if (expr.find('x') != string::npos) {
        cout << "Enter x:";
        cin >> x;
    }

    try {
        double result = evaluateRPN(rpn, x);
        cout << "Result: " << result << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
