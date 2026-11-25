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

double skaiciuotiSuma(double kaina, int kiekis) {
    return kaina * kiekis;
}

double skaiciuotiNuolaida(double kaina, int kiekis) {
    if (kiekis >= 10) {
        return skaiciuotiSuma(kaina, kiekis) * 0.10;
    }

    return 0;
}

double skaiciuotiKategorijosSuma(const string kategorijos[],
                                const double kainos[],
                                const int kiekiai[],
                                int kiekis,
                                string kategorija) {
    double suma = 0;

    for (int i = 0; i < kiekis; i++) {
        if (kategorijos[i] == kategorija) {
            suma = suma + skaiciuotiSuma(kainos[i], kiekiai[i])
                   - skaiciuotiNuolaida(kainos[i], kiekiai[i]);
        }
    }

    return suma;
}

void rodytiPrekes(const string kategorijos[],
                 const string pavadinimai[],
                 const double kainos[],
                 const int kiekiai[],
                 int kiekis,
                 string kategorija) {
    bool rasta = false;

    cout << fixed << setprecision(2);

    cout << left
         << setw(13) << "Kategorija"
         << setw(43) << "Pavadinimas"
         << right
         << setw(10) << "Kaina"
         << setw(8) << "Kiekis"
         << setw(12) << "Suma"
         << setw(12) << "Nuolaida"
         << setw(12) << "Moketi" << endl;

    cout << "Visos pinigines reiksmes pateiktos eurais." << endl;

    for (int i = 0; i < kiekis; i++) {
        if (kategorija == "" || kategorijos[i] == kategorija) {
            double suma = skaiciuotiSuma(kainos[i], kiekiai[i]);
            double nuolaida = skaiciuotiNuolaida(kainos[i], kiekiai[i]);
            double moketi = suma - nuolaida;

            cout << left
                 << setw(13) << kategorijos[i]
                 << setw(43) << pavadinimai[i]
                 << right
                 << setw(10) << kainos[i]
                 << setw(8) << kiekiai[i]
                 << setw(12) << suma
                 << setw(12) << nuolaida
                 << setw(12) << moketi << endl;

            rasta = true;
        }
    }

    if (!rasta) {
        cout << "Pasirinktos kategorijos prekiu uzsakyme nera." << endl;
    }
}

void rodytiUzsakymoSumas(const double kainos[],
                        const int kiekiai[],
                        int kiekis) {
    double bendraSuma = 0;
    double bendraNuolaida = 0;

    for (int i = 0; i < kiekis; i++) {
        bendraSuma = bendraSuma
                    + skaiciuotiSuma(kainos[i], kiekiai[i]);

        bendraNuolaida = bendraNuolaida
                        + skaiciuotiNuolaida(kainos[i], kiekiai[i]);
    }

    cout << fixed << setprecision(2);
    cout << "Suma pries nuolaidas: " << bendraSuma << " EUR" << endl;
    cout << "Bendra nuolaida: " << bendraNuolaida << " EUR" << endl;
    cout << "Galutine moketina suma: "
         << bendraSuma - bendraNuolaida << " EUR" << endl;
}

void rodytiKategorijuSumas(const string kategorijos[],
                         const double kainos[],
                         const int kiekiai[],
                         int kiekis) {
    cout << fixed << setprecision(2);
    cout << "Kategoriju sumos po nuolaidu:" << endl;

    cout << "Dviraciai: "
         << skaiciuotiKategorijosSuma(
                kategorijos, kainos, kiekiai, kiekis, "Dviraciai")
         << " EUR" << endl;

    cout << "Dalys: "
         << skaiciuotiKategorijosSuma(
                kategorijos, kainos, kiekiai, kiekis, "Dalys")
         << " EUR" << endl;

    cout << "Aksesuarai: "
         << skaiciuotiKategorijosSuma(
                kategorijos, kainos, kiekiai, kiekis, "Aksesuarai")
         << " EUR" << endl;
}

int gautiPasirinkima(int maziausias, int didziausias) {
    while (true) {
        cout << "Pasirinkite: ";

        string eilute;
        int pasirinkimas;

        if (!getline(cin, eilute)) {
            return 0;
        }

        istringstream ivestis(eilute);

        if (ivestis >> pasirinkimas) {
            ivestis >> ws;

            if (ivestis.eof() &&
                pasirinkimas >= maziausias &&
                pasirinkimas <= didziausias) {
                return pasirinkimas;
            }
        }

        cout << "Klaida: iveskite sveikaji skaiciu nuo "
             << maziausias << " iki " << didziausias << "." << endl;
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
        cout << "2. Perziureti pasirinktos kategorijos prekes" << endl;
        cout << "3. Perziureti viso uzsakymo sumas" << endl;
        cout << "4. Perziureti sumas pagal kategorijas" << endl;
        cout << "5. Parodyti visa uzsakymo suvestine" << endl;
        cout << "6. Iseiti" << endl;

        int pasirinkimas = gautiPasirinkima(1, 6);

        if (pasirinkimas == 0 || pasirinkimas == 6) {
            break;
        }

        if (pasirinkimas == 1) {
            rodytiPrekes(kategorijos, pavadinimai,
                         kainos, kiekiai, kiekis, "");
        } else if (pasirinkimas == 2) {
            cout << endl;
            cout << "1. Dviraciai" << endl;
            cout << "2. Dalys" << endl;
            cout << "3. Aksesuarai" << endl;

            int kategorijosNumeris = gautiPasirinkima(1, 3);

            if (kategorijosNumeris == 0) {
                break;
            }

            string kategorija;

            if (kategorijosNumeris == 1) {
                kategorija = "Dviraciai";
            } else if (kategorijosNumeris == 2) {
                kategorija = "Dalys";
            } else {
                kategorija = "Aksesuarai";
            }

            rodytiPrekes(kategorijos, pavadinimai,
                         kainos, kiekiai, kiekis, kategorija);

            cout << fixed << setprecision(2);
            cout << "Kategorijos moketina suma: "
                 << skaiciuotiKategorijosSuma(
                        kategorijos, kainos, kiekiai, kiekis, kategorija)
                 << " EUR" << endl;

        } else if (pasirinkimas == 3) {
            rodytiUzsakymoSumas(kainos, kiekiai, kiekis);

        } else if (pasirinkimas == 4) {
            rodytiKategorijuSumas(kategorijos, kainos, kiekiai, kiekis);

        } else if (pasirinkimas == 5) {
            rodytiPrekes(kategorijos, pavadinimai,
                         kainos, kiekiai, kiekis, "");

            cout << endl;
            rodytiUzsakymoSumas(kainos, kiekiai, kiekis);

            cout << endl;
            rodytiKategorijuSumas(kategorijos, kainos, kiekiai, kiekis);
        }
    }

    return 0;
}