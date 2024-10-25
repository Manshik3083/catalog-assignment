#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

const long long MOD = 1000000007;

// Function prototypes
long long inv(long long a); // Declare the inv function prototype

// Function to decode y values with modular arithmetic to avoid out-of-range issues
long long decodeValue(int base, const string& value) {
    long long result = 0;
    for (char digit : value) {
        int num = (isdigit(digit)) ? digit - '0' : digit - 'a' + 10;
        result = (result * base + num) % MOD;
    }
    return result;
}

// Lagrange interpolation with modular arithmetic
long long lagrangeInterpolation(const vector<int>& x_values, const vector<long long>& y_values, int x) {
    int n = x_values.size();
    long long result = 0;

    for (int i = 0; i < n; ++i) {
        long long term = y_values[i] % MOD;
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                term = term * (x - x_values[j] + MOD) % MOD;
                term = term * inv(x_values[i] - x_values[j] + MOD) % MOD; // Modular inverse
            }
        }
        result = (result + term) % MOD;
    }

    return result;
}

// Extended Euclidean Algorithm for modular inverse
long long inv(long long a) {
    long long m = MOD, m0 = m, y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        long long q = a / m, t = m;
        m = a % m, a = t, t = y;
        y = x - q * y;
        x = t;
    }
    return (x + m0) % m0;
}

// Function to parse JSON and find the constant term c
long long findConstantTerm(const json& data) {
    int n = data["keys"]["n"];
    int k = data["keys"]["k"];

    vector<int> x_values;
    vector<long long> y_values;

    for (auto it = data.begin(); it != data.end(); ++it) {
        if (it.key() != "keys") {
            int x = stoi(it.key());
            int base = stoi(it.value()["base"].get<string>());
            string value = it.value()["value"];
            long long y = decodeValue(base, value);
            x_values.push_back(x);
            y_values.push_back(y);
        }
    }

    x_values.resize(k);
    y_values.resize(k);

    return lagrangeInterpolation(x_values, y_values, 0);
}

int main() {
    ifstream inputFile("test_case_1.json");
    json data1;
    if (inputFile) {
        inputFile >> data1;
        long long constant1 = findConstantTerm(data1);
        cout << "Constant term for test case 1: " << constant1 << endl;
    } else {
        cerr << "Error: Could not open test_case_1.json" << endl;
    }

    ifstream inputFile2("test_case_2.json");
    json data2;
    if (inputFile2) {
        inputFile2 >> data2;
        long long constant2 = findConstantTerm(data2);
        cout << "Constant term for test case 2: " << constant2 << endl;
    } else {
        cerr << "Error: Could not open test_case_2.json" << endl;
    }

    return 0;
}
