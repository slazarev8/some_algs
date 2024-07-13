#include <iostream>
#include <stack>
#include <string>

bool IsOpen(char bracket) {
    return bracket == '(' || bracket == '[' || bracket == '{';
}

bool IsMatch(char bracketCurr, char bracketPrev) {
    if (bracketPrev == '[')
        return bracketCurr == ']';
    if (bracketPrev == '{')
        return bracketCurr == '}';
    if (bracketPrev == '(')
        return bracketCurr == ')';
    return false;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string line;
    std::getline(std::cin, line);

    std::stack<char> bracketStack;

    for (size_t lineInd = 0; lineInd < line.size(); ++lineInd) {
        if (IsOpen(line[lineInd])) {
            bracketStack.push(line[lineInd]);
        } else {
            if (bracketStack.empty()) {
                std::cout << lineInd << std::endl;
                return 0;
            }
            if (!IsMatch(line[lineInd], bracketStack.top())) {
                std::cout << lineInd << std::endl;
                return 0;
            } else {
                bracketStack.pop();
            }
        }
    }

    if (bracketStack.empty())
        std::cout << "CORRECT" << std::endl;
    else
        std::cout << line.size() << std::endl;

    return 0;
}
