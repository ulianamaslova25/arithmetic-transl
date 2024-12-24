#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <algorithm>
#include <stdexcept> 

using namespace std;

class ArithmeticTranslyator {
private:
    std::vector<std::string> data;
    std::stack<std::string> st;
    const std::string operators = "+-*/()";
    const std::string operat1 = "+-*/)";
    const std::string operat2 = "+-*/(";
    const std::string numbers = "0123456789.";

    bool isOperator(const std::string& str) {
        return operators.find(str) != std::string::npos;
    }

    bool isOperator1(const std::string& str) {
        return operat1.find(str) != std::string::npos;
    }

    bool isOperator2(const std::string& str) {
        return operat2.find(str) != std::string::npos;
    }

    int imp(const std::string& op) {
        if (op == "+" || op == "-") return 1;
        if (op == "*" || op == "/") return 2;
        return 0;
    }

public:
    ArithmeticTranslyator(std::string str = "") {
        std::string currentNumber;

        for (char c : str) {
            if (numbers.find(c) != std::string::npos) {
                currentNumber += c;
            }
            else {
                if (!currentNumber.empty()) {
                    data.push_back(currentNumber);
                    currentNumber.clear();
                }
                if (c != ' ') {
                    data.push_back(string(1, c));
                }
            }
        }
        if (!currentNumber.empty()) {
            data.push_back(currentNumber);
        }

        if (data.empty() || isOperator1(data.front()) || isOperator2(data.back())) {
            throw std::runtime_error("Wrong expression: Starts or ends with operator.");
        }

        for (size_t i = 0; i < data.size() - 1; ++i) {
            if (data[i] == "(" && data[i + 1] == ")") {
                throw std::runtime_error("Wrong expression: Empty parentheses.");
            }

        }

        int cnt_balance = 0;
        for (const string& s : data) {
            if (s == "(") cnt_balance++;
            else if (s == ")") cnt_balance--;
            if (cnt_balance < 0) throw std::runtime_error("Wrong expression: Mismatched parentheses.");
        }
        if (cnt_balance != 0) throw std::runtime_error("Wrong expression: Mismatched parentheses.");


        for (const std::string& s : data) {
            if (s.find('.') != string::npos) {
                if (s.front() == '.' || s.back() == '.' || std::count(s.begin(), s.end(), '.') > 1) {
                    throw std::runtime_error("Wrong expression: Invalid floating-point number.");
                }
            }
        }

    }


    double calculate() {
        std::stack<double> operands;
        std::stack<string> operators;

        auto counter = [&]() {
            double operand2 = operands.top(); operands.pop();
            double operand1 = operands.top(); operands.pop();
            string op = operators.top(); operators.pop();

            if (op == "+") operands.push(operand1 + operand2);
            else if (op == "-") operands.push(operand1 - operand2);
            else if (op == "*") operands.push(operand1 * operand2);
            else if (op == "/") {
                if (operand2 == 0) throw std::runtime_error("Division by zero");
                operands.push(operand1 / operand2);
            }
            };

        for (const string& i : data) {
            if (numbers.find(i[0]) != string::npos) {
                operands.push(stod(i));

            }
            else if (i == "(") {
                operators.push(i);
            }
            else if (i == ")") {
                while (!operators.empty() && operators.top() != "(") {
                    counter();
                }
                if (!operators.empty()) operators.pop();
            }
            else if (isOperator(i)) {
                while (!operators.empty() && imp(operators.top()) >= imp(i)) {
                    counter();
                }
                operators.push(i);
            }
        }


        while (!operators.empty()) {
            counter();
        }
        return operands.top();
    }





    friend std::ostream& operator<<(std::ostream& ostr, const ArithmeticTranslyator& other) {
        for (const auto& item : other.data) {
            ostr << item << " ";
        }
        return ostr << endl;
    }

    void printResult() {

        try {
            std::cout << calculate() << std::endl;
        }
        catch (const std::runtime_error& error) {
            std::cerr << "Error: " << error.what() << std::endl;
        }


    }

};




