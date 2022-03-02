/// Projekt 2 Chmielecki Damian Code::Blocks 17.12

#include <iostream>
#include <fstream>

using namespace std;

#define bajt_mask 0b0000000011111111    ///tzw maski ktore umozliwiaja z dwubajtowej zmiennej
#define klasa_mask 0b0000000011000000   ///wyodrebnic tylko wybrane bity
#define rodzaj_mask 0b0000000000100000
#define typ_mask 0b0000000000011111

const int max_nazwa = 100;
const int max_bajty = 1000;

void tworzenie_pliku(ofstream&, int&);
void odcz_plik(ifstream&);

void wyswietl_tabele_typow()
{
    /*cout << "| Lp.| Opis                 | Lp.| Opis             |" << endl;
    cout << "-----------------------------------------------------" << endl;
    cout << "|  0 | EOC (End-of-content) | 16 | SEQUENCE         |" << endl;
    cout << "|  1 | BOOLEAN              | 17 | SET              |" << endl;
    cout << "|  2 | INTEGER              | 18 | NUMERIC STRING   |" << endl;
    cout << "|  3 | BIT STRING           | 19 | PRINTABLE STRING |" << endl;
    cout << "|  4 | OCTET STRING         | 20 | T61 STRING       |" << endl;
    cout << "|  5 | NULL                 | 21 | VIDEOTEX STRING  |" << endl;
    cout << "|  6 | OBJECT IDENTIFIER    | 22 | IA5 STRING       |" << endl;
    cout << "|  7 | OBJECT DESCRIPTOR    | 23 | UTCTIME          |" << endl;
    cout << "|  8 | EXTERNAL             | 24 | GENERALIZED TIME |" << endl;
    cout << "|  9 | REAL                 | 25 | GRAPHIC STRING   |" << endl;
    cout << "| 10 | ENUMERATED           | 26 | VISABLE STRING   |" << endl;
    cout << "| 11 | EMBEDDED PDV         | 27 | GENERAL STRING   |" << endl;
    cout << "| 12 | UTF8String           | 28 | UNIVERSAL STRING |" << endl;
    cout << "| 13 | RELATIVE-OID         | 29 | CHARTER STRING   |" << endl;
    cout << "| 14 | zarezerwowana        | 30 | BMP STRING       |" << endl;
    cout << "| 15 | zarezerwowana        | 31 | specjalne uzycie |" << endl;*/

    cout << endl;
    cout << "| Lp.| Opis                 | Lp.| Opis             |" << endl;
    cout << "-----------------------------------------------------" << endl;
    cout << "|  1 | BOOLEAN              | 16 | SEQUENCE         |" << endl;
    cout << "|  2 | INTEGER              | 19 | PRINTABLE STRING |" << endl;
    cout << "|  6 | OBJECT IDENTIFIER    | 23 | UTCTIME          |" << endl;
}

void przesuniecie_bajta(char nazwa[max_nazwa], int o_ile) {       ///funkcja potrzebna przy typie SEQUENCE
    char kopia_bajtow[max_bajty], przesuniety_bajt[1];            ///poniewaz na poczatku nie wiadomo jaka bedzie dlugosc danych (dopiero na koncu jest dodawany bajt z dlugoscia)
    int rozmiar;                                                  ///to trzeba go przesunac na odpowiednia pozycje
    ifstream plik_odczyt(nazwa, ios::ate | ios::binary);
    rozmiar = plik_odczyt.tellg();
    plik_odczyt.clear();
    plik_odczyt.seekg(0, ios_base::beg);                    ///wczytanie zawartosci pliku do tablicy
    plik_odczyt.read(kopia_bajtow, rozmiar - 1);
    plik_odczyt.read(przesuniety_bajt, 1);
    plik_odczyt.close();

    ofstream plik_zapis(nazwa, ios::binary);                ///wpisanie danych do pliku juz w odpowiedniej kolejnosci
    for (int i = 0; i < rozmiar - o_ile - 1; i++) {
        plik_zapis << kopia_bajtow[i];
    }
    plik_zapis << przesuniety_bajt[0];
    for (int i = rozmiar - o_ile - 1; i < rozmiar - 1; i++) {
        plik_zapis << kopia_bajtow[i];
    }
    plik_zapis.close();
}

void wyswietl_typ(ifstream& plik, int typ_danych, int rozmiar)      ///funkcja wyswietla odpowiedni typ na podstawie danych z pliku
{
    char buffor;
    int bajt, suma = 0;
    switch (typ_danych) {
    case 1:
        cout << "Typ: BOOLEAN" << endl;
        plik.get(buffor);
        bajt = static_cast<int>(buffor);
        bajt = bajt & bajt_mask;
        cout << "Wartosc: ";
        if (bajt == 0) cout << "falsz" << endl;
        else if (bajt == 1) cout << "prawda" << endl;
        break;
    case 2:
        cout << "Typ: INTEGER" << endl;
        for (int i = rozmiar - 1; i >= 0; i--) {
            plik.get(buffor);
            bajt = static_cast<int>(buffor);
            bajt = bajt & bajt_mask;
            suma += bajt << (i * 8);
        }
        cout << "Wartosc: " << suma << endl;
        break;
    case 6:
        cout << "Typ: OBJECT IDENTIFIER" << endl;
        cout << "Wartosc: ";
        for (int i = 0; i < rozmiar; i++) {
            plik.get(buffor);
            cout << buffor;
        }
        break;
    case 16:
        cout << "Typ: SEQUENCE" << endl;                        ///typ SEQUENCE zawiera w sobie kolejne typy dlatego jest ponownie wywolywana
        cout << "Zawiera nastepujace typy:" << endl << endl;    ///funkcja odczytujaca zawartosc pliku
        odcz_plik(plik);
        break;
    case 19:
        cout << "Typ: PRINTABLE STRING" << endl;
        cout << "Wartosc:" << endl;
        for (int i = 0; i < rozmiar; i++) {
            plik.get(buffor);
            cout << buffor;
        }
        break;
    case 23:
        cout << "Typ: UTCTIME" << endl;
        cout << "Podana godzina: ";
        plik.get(buffor);
        bajt = static_cast<int>(buffor);
        bajt = bajt & bajt_mask;
        cout << bajt << ":";
        plik.get(buffor);
        bajt = static_cast<int>(buffor);
        bajt = bajt & bajt_mask;
        cout << bajt << endl;
        break;
    }
    cout << endl;
    if (typ_danych != 16) odcz_plik(plik);
}

void odcz_plik(ifstream& plik)                      ///funkcja odczytuje zawartosc pliku i wyswietla informacje na temat danego typu
{
    char buffor;
    int bajt;
    unsigned short int typ;
    int klasa_danych, rodzaj_danych, typ_danych, rozmiar;
    long int dlugosc = 0;
    plik.get(buffor);
    typ = static_cast<unsigned short int>(buffor);
    typ = typ & bajt_mask;                             ///wyodrebnienie bajtu zawierajacego informacje o typie przy pomocy maski uzyskujemy informacje na temat danych
    klasa_danych = (typ & klasa_mask) >> 6;
    rodzaj_danych = (typ & rodzaj_mask) >> 5;
    typ_danych = typ & typ_mask;


    if (typ_danych == 0) return;
    cout << "Klasa danych: " << klasa_danych << endl;
    cout << "Rodzaj danych: ";
    if (rodzaj_danych == 0) cout << "prymitywny" << endl;
    else if (rodzaj_danych == 1) cout << "zlozony" << endl;
    cout << "Typ danych: " << typ_danych << endl;

    plik.get(buffor);
    bajt = static_cast<int>(buffor);
    rozmiar = bajt & bajt_mask;
    if (rozmiar > 127) {                                 ///jezeli pierwszy bajt rozmiaru jest wiekszy niz 127 oznacza to ze jest on zawarty w kolejnych bajtach
        rozmiar = rozmiar - 128;
        dlugosc = rozmiar;
        for (int i = dlugosc - 1; i >= 0; i--) {
            plik.get(buffor);
            bajt = static_cast<int>(buffor);
            rozmiar += (bajt & bajt_mask) << (i * 8);
        }
    }

    cout << "Rozmiar: " << rozmiar << " (bajty)" << endl;

    if (rodzaj_danych == 0) {
        wyswietl_typ(plik, typ_danych, rozmiar);
    }
    else {
        odcz_plik(plik);
    }
}

void wpisz_do_pliku(ofstream& plik, int rozmiar, int wartosc, string& text, int& seq_rozmiar)   ///funkcja wpisuje do pliku informacje o danych
{                                                                                               ///ich typ, rozmiar, wartosc w postacji binarnej
    char buffor;
    int bajt;
    if (rozmiar > 127) {                                            ///jezeli romiar wartosci danych jest wiekszy niz 127(czyli nie zmiesci sie na jednym bajcie)
        int bajty_roz = rozmiar / 255;                              ///to nalezy go podzielic na wiecej bajtow a w pierwszy ppodac ilosc tych bajtow
        if (rozmiar % 255 != 0) bajty_roz += 1;
        buffor = static_cast<char>(128 + bajty_roz);                ///pierwszy bajt zawierajacy liczbe kolejnych bajtow z rozmiarem
        plik << buffor;
        if (seq_rozmiar != -1) seq_rozmiar = seq_rozmiar + bajty_roz + 1;
        for (int i = bajty_roz - 1; i >= 0; i--) {
            bajt = rozmiar >> (8 * i);
            buffor = static_cast<char>(bajt);
            plik << buffor;
        }
    }
    else {
        buffor = static_cast<char>(rozmiar);
        plik << buffor;
        if (seq_rozmiar != -1) seq_rozmiar += rozmiar + 1;
    }

    if (text == "") {                                        ///wpisanie do pliku jezeli wartosc jest liczba
        for (int i = rozmiar - 1; i >= 0; i--) {
            bajt = wartosc >> (8 * i);
            buffor = static_cast<char>(bajt);
            plik << buffor;
        }
    }
    else {                                                ///wpisanie do pliku jezeli wartosc jest tekstem
        if (wartosc == 0) {
            for (int i = 0; i < rozmiar; i++) {
                plik << text.at(i);
            }
        }
    }
}

void sprawdz_typ(ofstream& plik, int typ_danych, int& seq_rozmiar)          ///funkcja na podstawie danych podanych w konsoli wywietla odpowiedni typ
{
    ///1 2 6 16 19 23
    int rozmiar, wartosc;
    bool nowy_typ;
    char kopia[max_bajty];
    string text = "";
    char id_o[max_bajty], znak;
    unsigned short int czas, godziny, minuty;
    switch (typ_danych) {
    case 1:
        cout << "Wybrano typ BOOLEAN, podaj jego wartosc(0-falsz, 1-prawda): ";
        cin >> wartosc;
        rozmiar = 1;
        wpisz_do_pliku(plik, rozmiar, wartosc, text, seq_rozmiar);
        break;
    case 2:
        cout << "Wybrano typ INTEGER, podaj jego wartosc: ";
        cin >> wartosc;
        rozmiar = 4;
        wpisz_do_pliku(plik, rozmiar, wartosc, text, seq_rozmiar);
        break;
    case 6:
        cout << "Wybrano typ OBJECT IDENTIFIER";
        cout << "Podaj jego wartosc(np. 1.3.45.23): ";
        cin >> text;
        rozmiar = text.length();
        wpisz_do_pliku(plik, rozmiar, 0, text, seq_rozmiar);
        break;
    case 16:
        if (seq_rozmiar == -1) {
            cout << "Wybrano typ SEQUENCE (sekwencja jednego lub wiecej typow)" << endl;
            seq_rozmiar = 0;
            tworzenie_pliku(plik, seq_rozmiar);
        }
        else {
            text = "seq";
            rozmiar = seq_rozmiar;
            seq_rozmiar = -1;
            wpisz_do_pliku(plik, rozmiar, 1, text, seq_rozmiar);
            seq_rozmiar = rozmiar;
        }
        break;
    case 19:
        cout << "Wybrano typ PRINTABLE STRING, podaj jego wartosc: ";
        cin.ignore();
        getline(cin, text);
        rozmiar = text.length();
        wpisz_do_pliku(plik, rozmiar, 0, text, seq_rozmiar);
        break;
    case 23:
        cout << "Wybrano typ UTCTIME" << endl;
        cout << "Podaj godzine: ";
        cin >> godziny;
        cout << "Podaj minuty: ";
        cin >> minuty;
        czas = godziny << 8;
        czas += minuty;
        wpisz_do_pliku(plik, 2, czas, text, seq_rozmiar);
        break;
    }
    if (seq_rozmiar != -1 && typ_danych != 16) {                                     ///w wartosci typu SEQUENCE mozna dodac kilka innych typow
        cout << "Czy chcesz dodac nowy typ do sekwencji?(0 - NIE, 1 - TAK): ";       ///dlatego uzytkownik moze wybrac czy chce dodac kolejny pod typ czy nie
        cin >> nowy_typ;
        if (nowy_typ) {
            int s = 1;
            tworzenie_pliku(plik, seq_rozmiar);
        }
        else {
            sprawdz_typ(plik, 16, seq_rozmiar);
        }
    };
}

void tworzenie_pliku(ofstream& plik, int& seq_rozmiar)  ///funkcja tworzy plik i dodaje do niego informacje na temat danych podane w konsoli(jest sprawdzana poprawnosc danych)
{
    char buffor;
    int klasa_danych, rodzaj_danych, typ_danych, bajt_T, t = 0, k = 0, r = 0;

    cout << "Podaj typ danych(wpisz 32 zeby zobaczyc tabele): ";
    if (seq_rozmiar != -1) seq_rozmiar++;
    while (t == 0) {
        cin >> typ_danych;
        if (typ_danych < 0 || typ_danych > 32) {
            cout << "Nieprawidlowy typ! Podaj inny: ";
        }
        if (typ_danych == 32){
            wyswietl_tabele_typow();
            cout << endl;
            cout << "Teraz wybierz typ: ";
            cin >> typ_danych;
            t++;
            }
        if (typ_danych > 0 || typ_danych < 31) {
            t++;
            cout << "----" << endl;
            cout << "        Podaj klase danych:" << endl;
            cout << "  Rodzimy -> 0" << endl;
            cout << "  Dla aplikacji -> 1" << endl;
            cout << "  Zalezny od kontekstu -> 2" << endl;
            cout << "  Prywatna specyfikacja -> 3" << endl;
            cout << "----" << endl;
        }
        while (k == 0) {
            cin >> klasa_danych;
            if (klasa_danych == 0 || klasa_danych == 1 || klasa_danych == 2 || klasa_danych == 3) {
            k++;
            cout << "----" << endl;
            cout << "Podaj rodzaj danych:" << endl;
            cout << "  Prymitywny -> 0" << endl;
            cout << "  Zlozony    -> 1" << endl;
            cout << "----" << endl;
            }
        }
        while (r == 0) {
            if(typ_danych == 1 || typ_danych == 2 || typ_danych == 6){
                rodzaj_danych = 0;
                cout << rodzaj_danych << endl;
            }
            if(typ_danych == 23 || typ_danych == 19 || typ_danych == 16) cin >> rodzaj_danych;
            if (rodzaj_danych == 0) {
                r++;
                bajt_T = (klasa_danych << 6) + (rodzaj_danych << 5) + typ_danych;
                buffor = static_cast<char>(bajt_T);
                plik << buffor;
                sprawdz_typ(plik, typ_danych, seq_rozmiar);
            }
            if (rodzaj_danych == 1) {
                r++;
                bajt_T = (klasa_danych << 6) + (rodzaj_danych << 5);
                buffor = static_cast<char>(bajt_T);
                plik << buffor;
                /*if (typ_danych == 16){
                    sprawdz_typ(plik, typ_danych, seq_rozmiar);
                }else*/
                 tworzenie_pliku(plik, seq_rozmiar);
            }
        }
    }
}

int main()
{
    char nazwa[max_nazwa];
    int co_zrobic;
    int seq_rozmiar = -1;

    przesuniecie_bajta("seq3", 6);

    while(co_zrobic != 2) {
        cout << endl;
        cout << "-----------------------------------------------------" << endl;
        cout << " Chcesz odczytac czy stworzyc plik w standardzie BER?" << endl;
        cout << "   Odczytac plik, wpisz 0." << endl;
        cout << "   Stowrzyc plik, wpisz 1." << endl;
        cout << "   Wyjsc z programu, wpisz 2." << endl;
        cout << "-----------------------------------------------------" << endl;
        cout << "Twoje instrukcje: ";
        cin >> co_zrobic;
        while(co_zrobic > 2 || co_zrobic < 0) {
            cout << "Zla wartosc! Podaj poprawna: ";
            cin >> co_zrobic;
        }

        if(co_zrobic == 1) {                                        ///chcemy stworzy plik
            cout << "Podaj nazwe pliku: ";
            cin >> nazwa;
            ofstream plik(nazwa, ios::out | ios::binary);
            cout << "Tworzenie pliku..." << endl;
            tworzenie_pliku(plik, seq_rozmiar);
            plik.close();
            if(seq_rozmiar != -1) przesuniecie_bajta(nazwa, seq_rozmiar);
        } else if(co_zrobic == 0) {                                 ///chcemy otworzyc plik
            cout << "Podaj nazwe pliku: ";
            cin >> nazwa;
            ifstream plik;
            plik.open(nazwa, ios::in | ios::binary);
            while(plik.fail()) {
                plik.close();
                cout << "Plik nie istnieje!" << endl;
                cout << "Podaj nazwe pliku: ";
                cin >> nazwa;
                plik.open(nazwa, ios::in | ios::binary);
            }
            cout << "Odczytywanie pliku..." << endl << endl;
            odcz_plik(plik);
            plik.close();
        }
    }

    return 0;
}

