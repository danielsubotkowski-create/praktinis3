#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

// Nuskaito failą ir patikrina duomenis. Grąžina true, jei pavyko, arba false, jei įvyko klaida
// Keturi masyvai saugo tos pačios prekės duomenis tuo pačiu indeksu
// int &kiekis yra nuoroda. čia pakeitus kiekis, pasikeičia ir main funkcijos kintamasis
bool nuskaitytiDuomenis(string kategorijos[], string pavadinimai[],
                       double kainos[], int kiekiai[], int &kiekis) {
    // Failo ieškoma programos darbinio aplanko viduje. ifstream skirtas skaitymui.
    ifstream failas("uzsakymas.txt");

    if (!failas.is_open()) {
        cout << "Ivyko klaida. Nepavyko atverti uzsakymas.txt." << endl;
        return false;
    }

    string eilute;
    getline(failas, eilute);
    // Tekstinę eilutę paverčiame srautu, iš kurio galima nuskaityti skaičių su >>.
    istringstream pirmaEilute(eilute);

    // ! reiškia neigimą, || reiškia „arba“. Klaida, jei nepavyko nuskaityti arba netinka ribos.
    if (!(pirmaEilute >> kiekis) || kiekis < 1 || kiekis > 100) {
        cout << "Ivyko klaida. Irasu skaicius turi buti nuo 1 iki 100." << endl;
        return false;
    }

    // ws praleidžia likusius tarpus. eof tikrina, ar pasiekta eilutės srauto pabaiga

    pirmaEilute >> ws;

    if (!pirmaEilute.eof()) {
        cout << "Ivyko klaida. Netinkama pirmoji failo eilute." << endl;
        return false;
    }

    // Masyvo indeksai prasideda nuo 0; apdorojame tik kiekis įrašų.
    for (int i = 0; i < kiekis; i++) {
        if (!getline(failas, eilute)) {
            cout << "Ivyko klaida. Faile truksta prekiu irasu." << endl;
            return false;
        }

        // Kiekviena failo eilutė apdorojama kaip atskiras prekės įrašas
        istringstream irasas(eilute);
        char skirtukas;

        // Kainą ir kiekį skaitome kaip skaičius, o juos skiriantį simbolį saugome skirtukas
        if (!getline(irasas, kategorijos[i], ';') ||
            !getline(irasas, pavadinimai[i], ';') ||
            !(irasas >> kainos[i] >> skirtukas >> kiekiai[i])) {
            cout << "Ivyko klaida. epavyko nuskaityti iraso "
                 << i + 1 << "." << endl;
            return false;
        }

        irasas >> ws;

        // Tikriname skirtuką ir ar po kiekio neliko pašalinių simbolių, pvz., „2.5“ ar „2abc“.
        if (skirtukas != ';' || !irasas.eof()) {
            cout << "Ivyko klaida. Netinkamas iraso "
                 << i + 1 << " formatas." << endl;
            return false;
        }

        // klaida, kai kategorija nesutampa nė su viena leistina kategorija.
        if (kategorijos[i] != "Dviraciai" &&
            kategorijos[i] != "Dalys" &&
            kategorijos[i] != "Aksesuarai") {
            cout << "Ivyko klaida. Nezinoma kategorija irase." << endl;
            return false;
        }

        // Ieškome pirmo simbolio, kuris nėra tarpas, tabuliacija ar grįžimo į eilutės pradžią ženklas.
        // string::npos reiškia, kad tokio simbolio nėra: pavadinimas tuščias arba vien tarpai.
        if (pavadinimai[i].find_first_not_of(" \t\r") == string::npos ||
            kainos[i] <= 0 || kiekiai[i] <= 0) {
            cout << "Ivyko klaida. Netinkamas pavadinimas, kaina arba kiekis irase."
                 << i + 1 << "." << endl;
            return false;
        }
    }

    return true;
}

// Grąžina vienos prekės eilutės sumą prieš nuolaidą.
// Čia kiekis reiškia tos prekės vienetų skaičių, o ne failo įrašų skaičių.
double skaiciuotiSuma(double kaina, int kiekis) {
    return kaina * kiekis;
}

// Grąžina nuolaidos sumą eurais. Bent 10 tos pačios eilutės vienetų gauna 10 % nuolaidą.
double skaiciuotiNuolaida(double kaina, int kiekis) {
    if (kiekis >= 10) {
        return skaiciuotiSuma(kaina, kiekis) * 0.10;
    }

    return 0;
}

// Grąžina pasirinktos kategorijos mokėtiną sumą po nuolaidų.
// const neleidžia šioje funkcijoje keisti perduotų masyvų elementų.
// Jei kategorijos prekių nėra, suma lieka lygi 0.
double skaiciuotiKategorijosSuma(const string kategorijos[],
                                const double kainos[],
                                const int kiekiai[],
                                int kiekis,
                                string kategorija) {
    double suma = 0;

    // Masyvo indeksai prasideda nuo 0; apdorojame tik kiekis įrašų.
    for (int i = 0; i < kiekis; i++) {
        if (kategorijos[i] == kategorija) {
            suma = suma + skaiciuotiSuma(kainos[i], kiekiai[i])
                   - skaiciuotiNuolaida(kainos[i], kiekiai[i]);
        }
    }

    return suma;
}

// Tuščia kategorija "" leidžia rodyti visas prekes.
// ostream &isvestis yra nuoroda į išvesties srautą: ekraną arba failą.
// = cout yra numatytoji reikšmė: nepateikus šio argumento, rašoma į ekraną.
void rodytiPrekes(const string kategorijos[],
                 const string pavadinimai[],
                 const double kainos[],
                 const int kiekiai[],
                 int kiekis,
                 string kategorija,
                 ostream &isvestis = cout) {
    // Žymime, ar bent viena prekė atitiko pasirinktą kategoriją.
    bool rasta = false;

    // fixed kartu su setprecision(2) pinigines reikšmes rodo dviem skaitmenimis po taško.
    isvestis << fixed << setprecision(2);

    // setw nustato kito laukelio mažiausią plotį, left ir right – lygiavimą.
    isvestis << left
         << setw(13) << "Kategorija"
         << setw(43) << "Pavadinimas"
         << right
         << setw(10) << "Kaina"
         << setw(8) << "Kiekis"
         << setw(12) << "Suma"
         << setw(12) << "Nuolaida"
         << setw(12) << "Moketi" << endl;

    isvestis << "Visos pinigines reiksmes pateiktos eurais." << endl;

    // Masyvo indeksai prasideda nuo 0; apdorojame tik kiekis įrašų.
    for (int i = 0; i < kiekis; i++) {
        // Į lentelę įtraukiame visas prekes arba tik sutampančios kategorijos prekes.
        if (kategorija == "" || kategorijos[i] == kategorija) {
            double suma = skaiciuotiSuma(kainos[i], kiekiai[i]);
            double nuolaida = skaiciuotiNuolaida(kainos[i], kiekiai[i]);
            double moketi = suma - nuolaida;

            isvestis << left
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

    // Pranešimas reikalingas, kai pasirinktos kategorijos užsakyme nėra
    if (!rasta) {
        isvestis << "Pasirinktos kategorijos prekiu uzsakyme nera." << endl;
    }
}

// Susumuoja visų prekių sumas ir nuolaidas, tada išveda bendrus užsakymo rezultatus
void rodytiUzsakymoSumas(const double kainos[],
                        const int kiekiai[],
                        int kiekis,
                        ostream &isvestis = cout) {
    double bendraSuma = 0;
    double bendraNuolaida = 0;

    // Masyvo indeksai prasideda nuo 0; apdorojame tik kiekis įrašų
    for (int i = 0; i < kiekis; i++) {
        bendraSuma = bendraSuma
                    + skaiciuotiSuma(kainos[i], kiekiai[i]);

        bendraNuolaida = bendraNuolaida
                        + skaiciuotiNuolaida(kainos[i], kiekiai[i]);
    }

    // fixed kartu su setprecision(2) pinigines reikšmes rodo dviem skaitmenimis po taško.
    isvestis << fixed << setprecision(2);
    isvestis << "Suma pries nuolaidas: " << bendraSuma << " EUR" << endl;
    isvestis << "Bendra nuolaida: " << bendraNuolaida << " EUR" << endl;
    isvestis << "Galutine moketina suma: "
         << bendraSuma - bendraNuolaida << " EUR" << endl;
}

// Visoms trims kategorijoms naudojame tą pačią skaičiavimo funkciją
// Keičiasi tik paskutinis argumentas – kategorijos pavadinimas
void rodytiKategorijuSumas(const string kategorijos[],
                         const double kainos[],
                         const int kiekiai[],
                         int kiekis,
                         ostream &isvestis = cout) {
    // fixed kartu su setprecision(2) pinigines reikšmes rodo dviem skaitmenimis po taško.
    isvestis << fixed << setprecision(2);
    isvestis << "Kategoriju sumos po nuolaidu:" << endl;

    isvestis << "Dviraciai: "
         << skaiciuotiKategorijosSuma(
                kategorijos, kainos, kiekiai, kiekis, "Dviraciai")
         << " EUR" << endl;

    isvestis << "Dalys: "
         << skaiciuotiKategorijosSuma(
                kategorijos, kainos, kiekiai, kiekis, "Dalys")
         << " EUR" << endl;

    isvestis << "Aksesuarai: "
         << skaiciuotiKategorijosSuma(
                kategorijos, kainos, kiekiai, kiekis, "Aksesuarai")
         << " EUR" << endl;
}

// Grąžina sveikąjį skaičių nurodytame intervale. Neteisingai įvedus, prašo dar kartą.
// Nutraukus įvestį grąžina 0, kad main galėtų užbaigti programą.
int gautiPasirinkima(int maziausias, int didziausias) {
    // Ciklas kartojamas, kol jį užbaigia break arba iš funkcijos išeinama su return.
    while (true) {
        cout << "Pasirinkite: ";

        string eilute;
        int pasirinkimas;

        // Skaitome visą įvestą eilutę, todėl galime aptikti ir tekstą po skaičiaus.
        if (!getline(cin, eilute)) {
            return 0;
        }

        istringstream ivestis(eilute);

        if (ivestis >> pasirinkimas) {
            ivestis >> ws;

            // Priimame tik vieną sveikąjį skaičių, kuris patenka į leistinas ribas.
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

// Sudaro visą ataskaitą
// Visoms išvedimo funkcijoms perduodame tą patį srautą, kad ataskaita patektų į vieną vietą
void isvestiAtaskaita(const string kategorijos[],
                     const string pavadinimai[],
                     const double kainos[],
                     const int kiekiai[],
                     int kiekis,
                     ostream &isvestis) {
    isvestis << "DVIRACIU PARDUOTUVES UZSAKYMO ATASKAITA" << endl;
    isvestis << "Prekiu irasu: " << kiekis << endl;
    isvestis << endl;

    rodytiPrekes(kategorijos, pavadinimai, kainos,
                 kiekiai, kiekis, "", isvestis);

    isvestis << endl;
    rodytiUzsakymoSumas(kainos, kiekiai, kiekis, isvestis);

    isvestis << endl;
    rodytiKategorijuSumas(kategorijos, kainos,
                         kiekiai, kiekis, isvestis);
}

// Išsaugo ataskaitą ir grąžina, ar įrašymas pavyko
bool issaugotiAtaskaita(const string kategorijos[],
                       const string pavadinimai[],
                       const double kainos[],
                       const int kiekiai[],
                       int kiekis) {
    // ofstream skirtas rašymui. Taip atidarant failą, ankstesnis jo turinys perrašomas.
    ofstream failas("ataskaita.txt");

    if (!failas.is_open()) {
        cout << "Klaida: nepavyko atverti ataskaita.txt irasymui." << endl;
        return false;
    }

    isvestiAtaskaita(kategorijos, pavadinimai, kainos,
                    kiekiai, kiekis, failas);

    // Uždarymas užbaigia įrašymą. Po jo tikriname ir įrašymo, ir uždarymo klaidas.
    failas.close();

    if (failas.fail()) {
        cout << "Klaida: nepavyko sekmingai irasyti ataskaitos." << endl;
        return false;
    }

    return true;
}

int main() {
    // Talpa – 100 prekių. Visuose keturiuose masyvuose indeksas i nurodo tą pačią prekę.
    // Pavyzdžiui, kainos[0] ir kiekiai[0] priklauso pirmajai prekei.
    string kategorijos[100];
    string pavadinimai[100];
    double kainos[100];
    int kiekiai[100];
    // Faktinį įrašų skaičių gausime iš failo pirmosios eilutės.
    int kiekis = 0;

    // Jei nuskaitymas nepavyko, baigiame programą su klaidos kodu 1
    if (!nuskaitytiDuomenis(kategorijos, pavadinimai,
                           kainos, kiekiai, kiekis)) {
        return 1;
    }

    cout << "Duomenys nuskaityti. Prekiu irasu: "
         << kiekis << endl;

    // Ciklas kartojamas, kol jį užbaigia break arba iš funkcijos išeinama su return.
    while (true) {
        cout << endl;
        cout << "1. Perziureti visas prekes" << endl;
        cout << "2. Perziureti pasirinktos kategorijos prekes" << endl;
        cout << "3. Perziureti viso uzsakymo sumas" << endl;
        cout << "4. Perziureti sumas pagal kategorijas" << endl;
        cout << "5. Parodyti visa uzsakymo ataskaita" << endl;
        cout << "6. Issaugoti visa ataskaita i faila" << endl;
        cout << "7. Iseiti" << endl;

        // Funkcija priima tik meniu numerius nuo 1 iki 7.
        int pasirinkimas = gautiPasirinkima(1, 7);

        // break nutraukia meniu ciklą, kai pasirenkama išeiti arba nutrūksta įvestis.
        if (pasirinkimas == 0 || pasirinkimas == 7) {
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

            // Vartotojo pasirinktą numerį susiejame su faile naudojamu kategorijos pavadinimu.
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
            isvestiAtaskaita(kategorijos, pavadinimai, kainos,
                            kiekiai, kiekis, cout);

        } else if (pasirinkimas == 6) {

            if (issaugotiAtaskaita(kategorijos, pavadinimai,
                                  kainos, kiekiai, kiekis)) {
                cout << "Ataskaita sekmingai issaugota faile ataskaita.txt."
                     << endl;
            }
        }
    }
    return 0;
}