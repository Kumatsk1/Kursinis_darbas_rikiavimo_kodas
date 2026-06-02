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

// --- Suliejimo rikiavimas (Merge Sort) ---

long long suliejimoPalyginimai = 0;
long long suliejimoSukeitimai  = 0;

// Sulieja du surikiuotus pomasyvius
void sulieti(vector<int>& masyvas, int kairysis, int vidurys, int desinis) {
    int kairioDydis  = vidurys - kairysis + 1;
    int desinioDydis = desinis - vidurys;

    vector<int> kairys(kairioDydis), desinas(desinioDydis);

    for (int i = 0; i < kairioDydis; i++)  kairys[i]  = masyvas[kairysis + i];
    for (int j = 0; j < desinioDydis; j++) desinas[j] = masyvas[vidurys + 1 + j];

    int i = 0, j = 0, k = kairysis;
    while (i < kairioDydis && j < desinioDydis) {
        suliejimoPalyginimai++;
        if (kairys[i] <= desinas[j]) {
            masyvas[k] = kairys[i];
            i++;
        } else {
            masyvas[k] = desinas[j];
            suliejimoSukeitimai++;
            j++;
        }
        k++;
    }
    while (i < kairioDydis)  { masyvas[k++] = kairys[i++]; }
    while (j < desinioDydis) { masyvas[k++] = desinas[j++]; }
}

// Rekursyviai dalina masyvą ir sulieja
void suliejimoRikiavimas(vector<int>& masyvas, int kairysis, int desinis) {
    if (kairysis < desinis) {
        int vidurys = kairysis + (desinis - kairysis) / 2;
        suliejimoRikiavimas(masyvas, kairysis, vidurys);
        suliejimoRikiavimas(masyvas, vidurys + 1, desinis);
        sulieti(masyvas, kairysis, vidurys, desinis);
    }
}

// Nustato skaitiklius ir paleidžia rekursiją
void suliejimoRikiavimoPradzia(vector<int>& masyvas) {
    suliejimoPalyginimai = 0;
    suliejimoSukeitimai  = 0;
    if (!masyvas.empty())
        suliejimoRikiavimas(masyvas, 0, masyvas.size() - 1);
}

// --- Duomenų generavimas ---

// Atsitiktiniai skaičiai (seed = 42, rezultatai atkartojami)
vector<int> generuotiAtsitiktinius(int kiekis) {
    vector<int> masyvas(kiekis);
    mt19937 generatorius(42);
    uniform_int_distribution<int> skirstinys(1, 1000000);
    for (auto& elementas : masyvas) elementas = skirstinys(generatorius);
    return masyvas;
}

// Surikiuoti didėjančia tvarka
vector<int> generuotiSurikiuotus(int kiekis) {
    vector<int> masyvas(kiekis);
    iota(masyvas.begin(), masyvas.end(), 1);
    return masyvas;
}

// Surikiuoti mažėjančia tvarka
vector<int> generuotiAtvirksciai(int kiekis) {
    vector<int> masyvas(kiekis);
    iota(masyvas.begin(), masyvas.end(), 1);
    reverse(masyvas.begin(), masyvas.end());
    return masyvas;
}

// --- Matavimas ---

struct MatavimoPabaiga {
    double vidutinisLaikasUs; // vidurkis mikrosekundėmis
    long long palyginimai;
    long long sukeitimai;
};

// Paleidžia įterpimo rikiavimą 5 kartus, grąžina vidurkį
MatavimoPabaiga matuotiIterpima(const vector<int>& pradinis, int bandymai = 5) {
    double bendrasLaikas = 0.0;
    long long paskPalyginimai = 0, paskSukeitimai = 0;
    for (int b = 0; b < bandymai; b++) {
        vector<int> masyvas = pradinis; // kopija – pradinis nesikeičia
        auto pradzios_laikas = high_resolution_clock::now();
        iterpimoPRikiavimas(masyvas);
        auto pabaigos_laikas = high_resolution_clock::now();
        bendrasLaikas  += duration<double, micro>(pabaigos_laikas - pradzios_laikas).count();
        paskPalyginimai = iterpimoPalyginimai;
        paskSukeitimai  = iterpimoSukeitimai;
    }
    return { bendrasLaikas / bandymai, paskPalyginimai, paskSukeitimai };
}

// Paleidžia suliejimo rikiavimą 5 kartus, grąžina vidurkį
MatavimoPabaiga matuotiSuliejima(const vector<int>& pradinis, int bandymai = 5) {
    double bendrasLaikas = 0.0;
    long long paskPalyginimai = 0, paskSukeitimai = 0;
    for (int b = 0; b < bandymai; b++) {
        vector<int> masyvas = pradinis; // kopija – pradinis nesikeičia
        auto pradzios_laikas = high_resolution_clock::now();
        suliejimoRikiavimoPradzia(masyvas);
        auto pabaigos_laikas = high_resolution_clock::now();
        bendrasLaikas  += duration<double, micro>(pabaigos_laikas - pradzios_laikas).count();
        paskPalyginimai = suliejimoPalyginimai;
        paskSukeitimai  = suliejimoSukeitimai;
    }
    return { bendrasLaikas / bandymai, paskPalyginimai, paskSukeitimai };
}

int main() {
    vykdytiEksperimenta();
    return 0;
}
