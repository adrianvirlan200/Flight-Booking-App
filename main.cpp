#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include <ctime>
using namespace std;

std::time_t t = std::time(0); // get time now
std::tm *now = std::localtime(&t);
// std::cout << (now->tm_year + 1900) << '-'<< (now->tm_mon + 1) << '-'<< now->tm_mday
int este_parola_puternica(string parola)
{
    int c = 0;
    if (parola.length() >= 7) // are cel putin 7 carac.
    {
        c++;
    }
    for (int i = 0; i < parola.length(); i++)
    {
        if (isupper(parola[i])) // are caractere litere mari
        {
            c++;
        }
        if (parola.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos) // are caractere speciale
        {
            c++;
        }
    }
    if (c >= 3)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

class Operator
{
public:
    int login(string, string);
    void adauga_zbor(string, int, int, int, int); //
    void sterge_zbor(string, int, int, int);      //
};

class Utilizator
{
public:
    void creare_cont(string, string, string);
    int login(string, string);
    int cauta_zbor(string, int, int, int);     //
    int rezervare_zbor(string, int, int, int); //
    friend int este_parola_puternica(string);
};

int Operator::login(string mail, string parola)
{
    fstream file;
    file.open("conturi_operator.csv", ios::in);
    if (!file.is_open())
    {
        perror("Error open");
        exit(EXIT_FAILURE);
    }
    string line, fisier_mail, fisier_parola;

    while (getline(file, line)) // cat timp extragem linii
    {
        istringstream aux_line(line);

        getline(aux_line, fisier_mail, ',');
        getline(aux_line, fisier_parola, ',');

        if (fisier_mail == mail && fisier_parola == parola)
        {
            return 1;
        }

        fisier_mail.clear();
        fisier_parola.clear();
        line.clear();
    }

    file.close();
    throw "mail sau parola gresita";
}

void Operator::adauga_zbor(string oras, int zi, int luna, int an, int nr)
{ // verifca daca data este in trecut
    if (an < now->tm_year + 1900)
    {
        throw "\ndata este in trecut\n";
    }
    else if (an == now->tm_year + 1900)
    {
        if (luna < now->tm_mon + 1)
        {
            throw "\ndata este in trecut\n";
        }
        else if (luna == now->tm_mon + 1)
            if (zi < now->tm_mday)
            {
                throw "\ndata este in trecut\n";
            }
    }
    // verfica daca orasul are doar litere
    for (int i = 0; i < oras.length(); i++)
    {
        if (!isalpha(oras[i]))
        {
            throw "\ntara nu exista\n";
        }
    }
    // verifca daca formatul este zi/luna/an e corect
    if (zi > 31 || zi < 1 || luna > 12 || luna < 1 || an < now->tm_year + 1900)
    {
        throw "\nformatul data nu este corect\n";
    }

    fstream file;
    file.open("zboruri.csv", ios::app);
    if (!file.is_open())
    {
        perror("Error open");
        exit(EXIT_FAILURE);
    }

    file.seekp(0, ios::end); // seteaza cursorul din fisier la final
    int spot = file.tellg(); // lungimea de la iceputul fisierului la cursor

    if (spot == 0) // fisierul e gol
    {
        file << oras << "," << zi << "," << luna << "," << an << "," << nr;
    }
    else
    {
        file << "\n"
             << oras << "," << zi << "," << luna << "," << an << "," << nr;
    }

    file.close();
}

void Operator::sterge_zbor(string oras, int zi, int luna, int an)
{
    // verfica daca orasul are doar litere
    for (int i = 0; i < oras.length(); i++)
    {
        if (!isalpha(oras[i]))
        {
            throw "\ntara nu exista\n";
        }
    }
    // verifca daca formatul este zi/luna/an e corect
    if (zi > 31 || zi < 1 || luna > 31 || luna < 1 || an < now->tm_year + 1900)
    {
        throw "\nformatul data nu este corect\n";
    }

    fstream fin, fout;

    fin.open("zboruri.csv", ios::in);       // fisier initial;
    fout.open("aux_zboruri.csv", ios::out); // fisiser aux temporar;

    if (!fin.is_open() || !fout.is_open())
    {
        perror("Error open");
        exit(EXIT_FAILURE);
    }

    string aux_oras, aux_zi, aux_luna, aux_an;
    string line;

    int i = 0;

    while (getline(fin, line)) // cat timp extragem linii
    {
        istringstream aux_line(line);

        getline(aux_line, aux_oras, ',');
        getline(aux_line, aux_zi, ',');
        getline(aux_line, aux_luna, ',');
        getline(aux_line, aux_an, ',');

        // copiaza doar linia care nu este cautata
        if (aux_oras != oras || stoi(aux_zi) != zi || stoi(aux_luna) != luna || stoi(aux_an) != an)
        {
            if (i == 0) // daca e primul element nu adauga \n
            {
                fout << line;
                i++;
            }
            else
            {
                fout << "\n";
                fout << line;
                i++;
            }
        }
        // else nu scrie in fisier linia

        aux_oras.clear();
        aux_zi.clear();
        aux_luna.clear();
        aux_an.clear();
        line.clear();
    }

    fin.close();
    fout.close();

    remove("zboruri.csv");
    rename("aux_zboruri.csv", "zboruri.csv");
}

int Utilizator::cauta_zbor(string oras, int zi, int luna, int an)
{
    fstream file;
    file.open("zboruri.csv", ios::in);
    if (!file.is_open())
    {
        perror("Error open");
        exit(EXIT_FAILURE);
    }

    string line, aux_oras, aux_zi, aux_luna, aux_an, aux_nr;

    while (getline(file, line)) // cat timp extragem linii
    {
        istringstream aux_line(line);

        getline(aux_line, aux_oras, ',');
        getline(aux_line, aux_zi, ',');
        getline(aux_line, aux_luna, ',');
        getline(aux_line, aux_an, ',');
        getline(aux_line, aux_nr, ',');

        if (aux_oras == oras && stoi(aux_zi) == zi && stoi(aux_luna) == luna && stoi(aux_an) == an && stoi(aux_nr) > 0)
        {
            file.close();
            return 1; // zborul exista
        }

        aux_oras.clear();
        aux_zi.clear();
        aux_luna.clear();
        aux_an.clear();
        line.clear();
    }

    file.close();
    return 0; // zborul nu exista
}

int Utilizator::rezervare_zbor(string oras, int zi, int luna, int an)
{
    if (Utilizator::cauta_zbor(oras, zi, luna, an) == 0) // daca zborul nu exista
    {
        throw "nu exista zborul";
    }
    // verifca daca data nu este in trecut
    if (an < now->tm_year + 1900)
    {
        throw "\ndata este in trecut\n";
    }
    else if (an == now->tm_year + 1900)
    {
        if (luna < now->tm_mon + 1)
        {
            throw "\ndata este in trecut\n";
        }
        else if (luna == now->tm_mon + 1)
            if (zi < now->tm_mday)
            {
                throw "\ndata este in trecut\n";
            }
    }
    // verfica daca orasul are doar litere
    for (int i = 0; i < oras.length(); i++)
    {
        if (!isalpha(oras[i]))
        {
            throw "\ntara nu exista\n";
        }
    }
    // verifca daca formatul este zi/luna/an e corect
    if (zi > 31 || zi < 1 || luna > 31 || luna < 1 || an < now->tm_year + 1900)
    {
        throw "\nformatul data nu este corect\n";
    }

    fstream fin, fout;

    fin.open("zboruri.csv", ios::in);       // fisier initial;
    fout.open("aux_zboruri.csv", ios::out); // fisiser aux temporar;

    if (!fin.is_open() || !fout.is_open())
    {
        perror("Error open");
        exit(EXIT_FAILURE);
    }

    string aux_oras, aux_zi, aux_luna, aux_an, aux_nr;
    string line;

    int i = 0;

    while (getline(fin, line)) // cat timp extragem linii
    {
        istringstream aux_line(line);

        getline(aux_line, aux_oras, ',');
        getline(aux_line, aux_zi, ',');
        getline(aux_line, aux_luna, ',');
        getline(aux_line, aux_an, ',');
        getline(aux_line, aux_nr, ',');

        // cauta zborul
        if (aux_oras == oras && stoi(aux_zi) == zi && stoi(aux_luna) == luna && stoi(aux_an) == an) // daca e zborul cautat
        {
            if (i == 0) // daca e primul element nu adauga \n
            {
                fout << aux_oras << "," << aux_zi << "," << aux_luna << "," << aux_an << "," << stoi(aux_nr) - 1; // scade 1 la nr locuri
                i++;
            }
            else
            {
                fout << "\n";
                fout << aux_oras << "," << aux_zi << "," << aux_luna << "," << aux_an << "," << stoi(aux_nr) - 1; // scade 1 la nr locuri
                i++;
            }
        }
        else // daca nu e zborul cautat
        {
            if (i == 0) // daca e primul element nu adauga \n
            {
                fout << aux_oras << "," << aux_zi << "," << aux_luna << "," << aux_an << "," << stoi(aux_nr);
                i++;
            }
            else
            {
                fout << "\n";
                fout << aux_oras << "," << aux_zi << "," << aux_luna << "," << aux_an << "," << stoi(aux_nr);
                i++;
            }
        }

        aux_oras.clear();
        aux_zi.clear();
        aux_luna.clear();
        aux_an.clear();
        line.clear();
    }

    fin.close();
    fout.close();

    remove("zboruri.csv");
    rename("aux_zboruri.csv", "zboruri.csv");
    return 1;
}

int Utilizator::login(string mail, string parola)
{
    fstream file;
    file.open("conturi_utilizatori.csv", ios::in);
    if (!file.is_open())
    {
        perror("Error open");
        exit(EXIT_FAILURE);
    }
    string line, fisier_mail, fisier_parola;

    while (getline(file, line)) // cat timp extragem linii
    {
        istringstream aux_line(line);

        getline(aux_line, fisier_mail, ',');
        getline(aux_line, fisier_parola, ',');

        if (fisier_mail == mail && fisier_parola == parola)
        {
            return 1;
        }

        fisier_mail.clear();
        fisier_parola.clear();
        line.clear();
    }

    file.close();
    throw "mail sau parola gresita";
}

void Utilizator::creare_cont(string mail, string parola1, string parola2)
{

    if (mail.find(".com") == string::npos || mail.find("@") == string::npos)
    {
        throw "formatul mail nu este corect";
    }
    if (parola1 != parola2)
    {
        throw "parolele nu sunt identice";
    }
    else if (!este_parola_puternica(parola1))
    {
        throw "parola nu este buna;trebuie sa contina minim o litera mare,si un caracter special";
    }

    fstream file;
    file.open("conturi_utilizatori.csv", ios::app);
    if (!file.is_open())
    {
        perror("Error open");
        exit(EXIT_FAILURE);
    }

    file.seekp(0, ios::end); // seteaza cursorul din fisier la final
    int spot = file.tellg(); // lungimea de la iceputul fisierului la cursor

    if (spot == 0) // fisierul e gol
    {
        file << mail << "," << parola1;
    }
    else
    {
        file << "\n"
             << mail << "," << parola1;
    }

    file.close();
}

// zborurile sunt stocate in zboruri.csv sub forma: oras,zi,luna,an,nr locuri
//parolele in conturi_....csv sub forma mail,parola
int main()
{
    Operator x;
    Utilizator y;

    int input;
    string mail, parola1, parola2;
    string oras;
    int zi, luna, an, locuri;

    cout << "Login operator: 1\nLogin utilizator: 2\n";
    cin >> input;
    if (input == 1) // operator
    {
        // date cont
        cout << "mail:";
        cin >> mail;
        cout << "\nparola:";
        cin >> parola1;

        try
        {
            x.login(mail, parola1);
        }
        catch (const char *msg)
        {
            cout << msg;
            cout << "\ninchidere program;";
            return 1;
        }

        while (input != 3)
        {
            cout << "Adaugare zbor: 1\nStergere zbor: 2\nIesire: 3\n";
            cin >> input;

            if (input == 1) // adaugare zbor
            {
                cout << "oras/zi/luna/an/locuri\n";
                cin >> oras >> zi >> luna >> an >> locuri;
                try
                {
                    x.adauga_zbor(oras, zi, luna, an, locuri);
                }
                catch (const char *msg)
                {
                    cout << msg;
                    return 1;
                }
            }
            if (input == 2) // stergere zbor
            {
                cout << "oras/zi/luna/an\n";
                cin >> oras >> zi >> luna >> an;
                try
                {
                    x.sterge_zbor(oras, zi, luna, an);
                }
                catch (const char *msg)
                {
                    cout << msg;
                    return 1;
                }
            }
        }
    }
    else // utilizator
    {
        cout << "creare cont: 1\nautentificare: 2\n";
        cin >> input;

        if (input == 1)//creare cont
        {
            cout << "mail:";
            cin >> mail;
            cout << "\nparola:";
            cin >> parola1;
            cout << "repeta parola:";
            cin >> parola2;
            try
            {
                y.creare_cont(mail, parola1, parola2);
            }
            catch (const char *msg)
            {
                cout << msg;
                return 1;
            }
            // login
            cout << "\nLogin:\n";
            cout << "mail:";
            cin >> mail;
            cout << "\nparola:";
            cin >> parola1;

            try
            {
                y.login(mail, parola1);
            }
            catch (const char *msg)
            {
                cout << msg;
                return 1;
            }
        }
        else // doar login
        {
            //  date cont
            cout << "mail:";
            cin >> mail;
            cout << "\nparola:";
            cin >> parola1;
            try
            {
                y.login(mail, parola1);
            }
            catch (const char *msg)
            {
                cout << msg;
                return 1;
            }
        }

        cout << "cautare zbor: 1\nrezervare zbor: 2\niesire: 3\n";
        cin >> input;

        while (input != 3)
        {
            if (input == 1) // cauta zbor
            {
                cout << "oras/zi/luna/an";
                cin >> oras >> zi >> luna >> an;

                if (y.cauta_zbor(oras, zi, luna, an) == 1)
                {
                    cout << "exista\n";
                }
                else
                {
                    cout << "Nu exista\n";
                }
            }

            if (input == 2) // rezervare zbor
            {
                cout << "oras/zi/luna/an\n";
                cin >> oras >> zi >> luna >> an;

                try
                {
                    y.rezervare_zbor(oras, zi, luna, an);
                }
                catch (const char *msg)
                {
                    cout << msg;
                    return 1;
                }
            }
            
            cout << "cautare zbor: 1\nrezervare zbor: 2\niesire: 3\n";
            cin >> input;
        }
    }

    return 0;
}