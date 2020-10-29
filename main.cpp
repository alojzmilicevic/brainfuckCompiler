#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define MEMORY_SIZE 30000

void print_memory(vector<char> const &memory) {
    for (auto &t : memory) cout << (int) t << " ";

    cout << endl;
}

vector<char> compile(vector<char> const &program, vector<pair<int, int>> const &loopPointers) {
    vector<char> memory(MEMORY_SIZE, 0);

    unsigned int ptr{};
    char c{};

    for (int ip{0}; ip < program.size(); ++ip) {
        char instruction{program[ip]};

        switch (instruction) {
            case '+':
                ++memory[ptr];
                break;
            case '-':
                --memory[ptr];
                break;
            case '>':
                ++ptr;
                break;
            case '<':
                --ptr;
                break;
            case '.':
                cout << memory[ptr];
                break;
            case ',':
                cin >> c;
                memory[ptr] = c;
                break;
            case '[':
                if (memory[ptr] == 0) {
                    auto it = std::find_if(loopPointers.begin(), loopPointers.end(),
                                           [&ip](const pair<int, int> &element) {
                                               return element.first == ip || element.second == ip;
                                           });
                    ip = it->second;
                }
                break;
            case ']':
                if (memory[ptr] != 0) {
                    auto it = std::find_if(loopPointers.begin(), loopPointers.end(),
                                           [&ip](const pair<int, int> &element) {
                                               return element.first == ip || element.second == ip;
                                           });
                    ip = it->first;
                }
                break;
            default:
                break;
        }
    }

    return memory;
}

void print_pair(vector<pair<int, int>> const &v) {
    for (auto &e: v) {
        cout << "(" << e.first << ", " << e.second << ")" << endl;
    }
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        cerr << "Illegal number of parameters" << endl;
        return 1;
    }

    string filename{argv[1]};
    ifstream file{"../" + filename};

    if (!file) {
        cerr << "No such file" << endl;
        return 1;
    }

    char c{};
    vector<char> tokens{};
    vector<pair<int, int>> loopStack{};
    vector<int> openLoop{};
    int index{};

    while (file.get(c)) {
        if (c == '[') {
            openLoop.push_back(index);
        } else if (c == ']') {
            loopStack.emplace_back(pair(openLoop.back(), index));
            openLoop.pop_back();
        }
        tokens.push_back(c);
        ++index;
    }

    vector<char> output = compile(tokens, loopStack);
    //print_memory(output);



    return 0;
}
