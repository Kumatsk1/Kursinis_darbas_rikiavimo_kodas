#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <string>
#include <numeric>

using namespace std;
using namespace chrono;

// --- Įterpimo rikiavimas (Insertion Sort) ---

long long iterpimoPalyginimai = 0;
long long iterpimoSukeitimai  = 0;

void iterpimoPRikiavimas(vector<int>& masyvas) {
    iterpimoPalyginimai = 0;
    iterpimoSukeitimai  = 0;
    int dydis = masyvas.size();
    for (int i = 1; i < dydis; i++) {
        int raktas = masyvas[i]; // įterpiamas elementas
        int j = i - 1;
        while (j >= 0) {
            iterpimoPalyginimai++;
            if (masyvas[j] > raktas) {
                masyvas[j + 1] = masyvas[j]; // perkeliame į dešinę
                iterpimoSukeitimai++;
                j--;
            } else {
                break;
            }
        }
        masyvas[j + 1] = raktas; // įterpiame į rastą vietą
    }
}


int main() {
    return 0;
}
