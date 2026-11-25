#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

bool nuskaitytiDuomenis(string kategorijos[], string pavadinimai[],
                       double kainos[], int kiekiai[], int &kiekis) {
    ifstream failas("uzsakymas.txt");

    if (!failas.is_open()) {
        cout << "Ivyko klaida. Nepavyko atverti uzsakymas.txt." << endl;
        return false;
    }

    string eilute;
    getline(failas, eilute);
    istringstream pirmaEilute(eilute);

    if (!(pirmaEilute >> kiekis) || kiekis < 1 || kiekis > 100) {
        cout << "Ivyko klaida. Irasu skaicius turi buti nuo 1 iki 100." << endl;
        return false;
    }

    pirmaEilute >> ws;

    if (!pirmaEilute.eof()) {
        cout << "Ivyko klaida. Netinkama pirmoji failo eilute." << endl;
        return false;
    }

    for (int i = 0; i < kiekis; i++) {
        if (!getline(failas, eilute)) {
            cout << "Ivyko klaida. Faile truksta prekiu irasu." << endl;
            return false;
        }

        istringstream irasas(eilute);
        char skirtukas;

        if (!getline(irasas, kategorijos[i], ';') ||
            !getline(irasas, pavadinimai[i], ';') ||
            !(irasas >> kainos[i] >> skirtukas >> kiekiai[i])) {
            cout << "Ivyko klaida. epavyko nuskaityti iraso "
                 << i + 1 << "." << endl;
            return false;
        }

        irasas >> ws;

        if (skirtukas != ';' || !irasas.eof()) {
            cout << "Ivyko klaida. Netinkamas iraso "
                 << i + 1 << " formatas." << endl;
            return false;
        }

        if (kategorijos[i] != "Dviraciai" &&
            kategorijos[i] != "Dalys" &&
            kategorijos[i] != "Aksesuarai") {
            cout << "Ivyko klaida. Nezinoma kategorija irase." << endl;
            return false;
        }

        if (pavadinimai[i].find_first_not_of(" \t\r") == string::npos ||
            kainos[i] <= 0 || kiekiai[i] <= 0) {
            cout << "Ivyko klaida. Netinkamas pavadinimas, kaina arba kiekis irase."
                 << i + 1 << "." << endl;
            return false;
        }
    }

    return true;
}

void rodytiPrekes(const string kategorijos[], const string pavadinimai[],
                 const double kainos[], const int kiekiai[], int kiekis) {
    cout << fixed << setprecision(2);

    cout << left
         << setw(15) << "Kategorija"
         << setw(45) << "Pavadinimas"
         << right
         << setw(12) << "Kaina EUR"
         << setw(10) << "Kiekis" << endl;

    for (int i = 0; i < kiekis; i++) {
        cout << left
             << setw(15) << kategorijos[i]
             << setw(45) << pavadinimai[i]
             << right
             << setw(12) << kainos[i]
             << setw(10) << kiekiai[i] << endl;
    }
}

int main() {
    string kategorijos[100];
    string pavadinimai[100];
    double kainos[100];
    int kiekiai[100];
    int kiekis = 0;

    if (!nuskaitytiDuomenis(kategorijos, pavadinimai,
                           kainos, kiekiai, kiekis)) {
        return 1;
    }

    cout << "Duomenys nuskaityti. Prekiu irasu: "
         << kiekis << endl;

    while (true) {
        cout << endl;
        cout << "1. Perziureti visas prekes" << endl;
        cout << "2. Iseiti" << endl;
        cout << "Pasirinkite: ";

        string eilute;
        int pasirinkimas;

        if (!getline(cin, eilute)) {
            break;
        }

        istringstream ivestis(eilute);

        if (!(ivestis >> pasirinkimas)) {
            cout << "Ivyko klaida. Iveskite 1 arba 2." << endl;
            continue;
        }

        ivestis >> ws;

        if (!ivestis.eof()) {
            cout << "Ivyko klaida. Iveskite tik viena sveikaji skaiciu." << endl;
            continue;
        }

        if (pasirinkimas == 1) {
            rodytiPrekes(kategorijos, pavadinimai,
                        kainos, kiekiai, kiekis);
        } else if (pasirinkimas == 2) {
            break;
        } else {
            cout << "Ivyko klaida. Tokio pasirinkimo nera." << endl;
        }
    }

    return 0;
}