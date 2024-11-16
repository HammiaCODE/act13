#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <chrono>
#include <list>
#include <string>
#include <unordered_map>

using namespace std;

class Persona {
public:
    string nombre;
    string apellido;
    string direccion;
    string ADN;
    string ciudad;
    string telefono;

    Persona(string ID_n, string ID_a, string ID_d, string ID_adn, string ID_c, string ID_tel)
        : nombre(ID_n), apellido(ID_a),direccion(ID_d), ADN(ID_adn), ciudad(ID_c) ,telefono(ID_tel){}
};

class Grafo {
public:
    unordered_map<string, list<Persona*>> adjacencias;

    void addPersona(Persona* persona) {
        adjacencias[persona->ADN].push_back(persona);
    }
};

/*CARGAR DATOS*/
void dataLeer(Grafo& grafo,const string& archivo) {
    ifstream file(archivo);
    string linea;

    while(getline(file,linea)) {
        stringstream ss(linea);
        string nombre,apellido, direccion, ADN, ciudad, telefono;

        getline(ss, nombre, ',');
        getline(ss, apellido, ',');
        getline(ss, direccion, ',');
        getline(ss, ADN, ',');
        getline(ss, ciudad, ',');
        getline(ss, telefono, ',');

        Persona* persona = new Persona(nombre, apellido, direccion,ADN,ciudad,telefono);
        grafo.addPersona(persona);
    }
    file.close();
}


/*BFS*/
Persona* buscarBFS(Grafo& grafo, const std::string& codigoObjetivo) {
    queue<Persona*> cola;
    unordered_map<string, bool> visitados;

    for (auto& [codigo, personas] : grafo.adjacencias) {
        for (Persona* persona : personas) {
            if (!visitados[persona->ADN]) {
                visitados[persona->ADN] = true;
                cola.push(persona);

                while (!cola.empty()) {
                    Persona* actual = cola.front();
                    cola.pop();

                    if (actual->ADN == codigoObjetivo) {
                        return actual;
                    }

                    for (Persona* vecino : grafo.adjacencias[actual->ADN]) {
                        if (!visitados[vecino->ADN]) {
                            visitados[vecino->ADN] = true;
                            cola.push(vecino);
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

/*DFS*/
Persona* buscarDFS(Grafo& grafo, const string& codigoObjetivo) {
    stack<Persona*> pila;
    unordered_map<std::string, bool> visitados;

    for (auto& [codigo, personas] : grafo.adjacencias) {
        for (Persona* persona : personas) {
            if (!visitados[persona->ADN]) {
                visitados[persona->ADN] = true;
                pila.push(persona);

                while (!pila.empty()) {
                    Persona* actual = pila.top();
                    pila.pop();

                    if (actual-> ADN == codigoObjetivo) {
                        return actual;
                    }

                    for (Persona* vecino : grafo.adjacencias[actual-> ADN]) {
                        if (!visitados[vecino-> ADN]) {
                            visitados[vecino-> ADN] = true;
                            pila.push(vecino);
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

/*MAIN*/
int main() {
    Grafo grafo;
    dataLeer(grafo, "people.txt");

    string busqueda;
    cin >> busqueda;

    auto inicioBFS = chrono::high_resolution_clock::now();
    Persona* resultadoBFS = buscarBFS(grafo, busqueda);
    auto finBFS = chrono::high_resolution_clock::now();

    auto inicioDFS = chrono::high_resolution_clock::now();
    Persona* resultadoDFS = buscarDFS(grafo, busqueda);
    auto finDFS = chrono::high_resolution_clock::now();

    auto duracionBFS = chrono::duration_cast<chrono::microseconds>(finBFS - inicioBFS).count();
    auto duracionDFS = chrono::duration_cast<chrono::microseconds>(finDFS - inicioDFS).count();


    cout << "Tiempo de BFS: " << duracionBFS << " microsegundos" << std::endl;
    cout << "Tiempo de DFS: " << duracionDFS << " microsegundos" << std::endl;

    if (resultadoBFS) {
        cout << "Encontrado en BFS: " << resultadoBFS->nombre << std::endl;
    }

    if (resultadoDFS) {
        cout << "Encontrado en DFS: " << resultadoDFS->nombre << std::endl;
    }

    return 0;
}
