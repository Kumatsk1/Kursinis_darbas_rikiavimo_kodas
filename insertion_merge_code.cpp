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

// --- Rezultatų spausdinimas ---

void spausdintiAntraste() {
    cout << "\n";
    cout << "================================================================\n";
    cout << "   RIKIAVIMO ALGORITMŲ LYGINAMOJI ANALIZĖ\n";
    cout << "   Insertion Sort (Iterpimo) vs Merge Sort (Suliejimo)\n";
    cout << "================================================================\n\n";
}

void spausdintiLenteleAntraste() {
    cout << left
         << setw(12) << "Dydis"
         << setw(18) << "Duomenu tipas"
         << setw(22) << "Insertion Sort (us)"
         << setw(18) << "Merge Sort (us)"
         << setw(20) << "Ins. palyg."
         << setw(20) << "Mrg. palyg."
         << setw(18) << "Ins. sukeitimai"
         << setw(18) << "Mrg. sukeitimai"
         << "\n";
    cout << string(146, '-') << "\n";
}

void vykdytiEksperimenta() {
    spausdintiAntraste();

    vector<int> dydžiai = { 5000, 10000, 50000 };

    vector<pair<string, int>> duomenuTipai = {
        {"Atsitiktiniai", 0},
        {"Surikiuoti",    1},
        {"Atvirksciai",   2}
    };

    spausdintiLenteleAntraste();

    for (int n : dydžiai) {
        for (int t = 0; t < (int)duomenuTipai.size(); t++) {
            string tipoPavadinimas = duomenuTipai[t].first;
            int    tipoKodas       = duomenuTipai[t].second;
            vector<int> pradiniai;
            if      (tipoKodas == 0) pradiniai = generuotiAtsitiktinius(n);
            else if (tipoKodas == 1) pradiniai = generuotiSurikiuotus(n);
            else                     pradiniai = generuotiAtvirksciai(n);

            // Abu algoritmai testuojami su tais pačiais pradiniais duomenimis
            MatavimoPabaiga iterpimoRez  = matuotiIterpima(pradiniai);
            MatavimoPabaiga suliejimoRez = matuotiSuliejima(pradiniai);

            cout << left
                 << setw(12) << n
                 << setw(18) << tipoPavadinimas
                 << setw(22) << fixed << setprecision(2) << iterpimoRez.vidutinisLaikasUs
                 << setw(18) << suliejimoRez.vidutinisLaikasUs
                 << setw(20) << iterpimoRez.palyginimai
                 << setw(20) << suliejimoRez.palyginimai
                 << setw(18) << iterpimoRez.sukeitimai
                 << setw(18) << suliejimoRez.sukeitimai
                 << "\n";
        }
        cout << string(146, '-') << "\n";
    }

    cout << "\nPastaba: laikas mikrosekundemis (us), vidurkis is 5 bandymu.\n";
    cout << "Generavimo ir spausdinimo laikas netrauktas.\n";
}

int main() {
    vykdytiEksperimenta();
    return 0;
}
