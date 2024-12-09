#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

struct Nodo {
    string name;
    string last_name;
    char gender;
    int age;
    bool is_dead;
    Nodo* father;
    Nodo* mother;
    Nodo** siblings;
    int num_siblings;
    int max_siblings;

    Nodo(string name, string last_name, char gender, int age, bool is_dead)
        : name(name), last_name(last_name), gender(gender), age(age), is_dead(is_dead), father(nullptr), mother(nullptr), num_siblings(0), max_siblings(2) {
        siblings = new Nodo*[max_siblings];
    }

    void agregarHermano(Nodo* hermano) {
        if (num_siblings == max_siblings) {
            max_siblings *= 2;
            Nodo** nuevos_siblings = new Nodo*[max_siblings];
            for (int i = 0; i < num_siblings; ++i) {
                nuevos_siblings[i] = siblings[i];
            }
            delete[] siblings;
            siblings = nuevos_siblings;
        }
        siblings[num_siblings++] = hermano;
    }

    ~Nodo() {
        delete[] siblings;
    }
};

class ArbolGenealogico {
private:
    Nodo* raiz;
    Nodo** nodos;
    int num_nodos;
    int max_nodos;

    Nodo* obtenerNodo(const string& name, const string& last_name) {
        for (int i = 0; i < num_nodos; ++i) {
            if (nodos[i]->name == name && nodos[i]->last_name == last_name) {
                return nodos[i];
            }
        }
        if (num_nodos == max_nodos) {
            max_nodos *= 2;
            Nodo** nuevos_nodos = new Nodo*[max_nodos];
            for (int i = 0; i < num_nodos; ++i) {
                nuevos_nodos[i] = nodos[i];
            }
            delete[] nodos;
            nodos = nuevos_nodos;
        }
        Nodo* nuevo_nodo = new Nodo(name, last_name, ' ', 0, false);
        nodos[num_nodos++] = nuevo_nodo;
        return nuevo_nodo;
    }

public:
    ArbolGenealogico() : raiz(nullptr), num_nodos(0), max_nodos(10) {
        nodos = new Nodo*[max_nodos];
    }

    ~ArbolGenealogico() {
        for (int i = 0; i < num_nodos; ++i) {
            delete nodos[i];
        }
        delete[] nodos;
    }

    void leerCSV(const string& nombreArchivo) {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open()) {
            cerr << "Error: No se pudo abrir el archivo " << nombreArchivo << endl;
            exit(EXIT_FAILURE);
        }

        string linea;
        while (getline(archivo, linea)) {
            stringstream ss(linea);
            string name, last_name, gender, age, is_dead, father_name, father_last_name, mother_name, mother_last_name;

            getline(ss, name, ',');
            getline(ss, last_name, ',');
            getline(ss, gender, ',');
            getline(ss, age, ',');
            getline(ss, is_dead, ',');
            getline(ss, father_name, ',');
            getline(ss, father_last_name, ',');
            getline(ss, mother_name, ',');
            getline(ss, mother_last_name, ',');

            Nodo* nodo = obtenerNodo(name, last_name);
            nodo->name = name;
            nodo->last_name = last_name;
            nodo->gender = gender.empty() ? ' ' : gender[0];
            nodo->age = age.empty() ? 0 : stoi(age);
            nodo->is_dead = is_dead.empty() ? false : stoi(is_dead);

            if (!father_name.empty() && !father_last_name.empty()) {
                Nodo* padre = obtenerNodo(father_name, father_last_name);
                nodo->father = padre;
                padre->agregarHermano(nodo);
            }

            if (!mother_name.empty() && !mother_last_name.empty()) {
                Nodo* madre = obtenerNodo(mother_name, mother_last_name);
                nodo->mother = madre;
                madre->agregarHermano(nodo);
            }

            if (raiz == nullptr) {
                raiz = nodo;
            }
        }
    }

    void mostrarArbol(Nodo* nodo, int nivel = 0) const {
        if (nodo == nullptr) return;

        for (int i = 0; i < nivel; ++i) {
            cout << "  ";
        }
        cout << nodo->name << " " << nodo->last_name << " (Edad: " << nodo->age << ", " << (nodo->is_dead ? "Muerto" : "Vivo") << ")" << endl;

        for (int i = 0; i < nodo->num_siblings; ++i) {
            mostrarArbol(nodo->siblings[i], nivel + 1);
        }

        mostrarArbol(nodo->father, nivel + 1);
        mostrarArbol(nodo->mother, nivel + 1);
    }

    void mostrarArbol() const {
        mostrarArbol(raiz);
    }
};

void mostrarMenu() {
    cout << "Seleccione una opcion:" << endl;
    cout << "1. Mostrar arbol genealogico" << endl;
    cout << "2. Salir" << endl;
}

int main() {
    ArbolGenealogico arbol;
    arbol.leerCSV("arbol_genealogico.csv");

    int opcion;
    do {
        mostrarMenu();
        cout << "Ingrese su opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                arbol.mostrarArbol();
                break;
            case 2:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
                break;
        }
    } while (opcion != 2);

    return 0;
}