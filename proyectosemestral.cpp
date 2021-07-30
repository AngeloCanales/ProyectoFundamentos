#include <iostream>
#include "profile.cpp"
#include <sstream>
#include <fstream>
#include <map>
#include <unordered_map>
#include <list>
#include <stack>
#include <iomanip>
#include <chrono>

using namespace std;
void DFS(profile *v,int sw);
void Ordenado(profile *perfil , stack<profile*> &Stack);
void Componentes_fuertes_conexas(map<string,profile> lista);
void influencia(map<string,int> influyentes, map<string,int> influenciables);
bool *visited;
int cantidad_nodos = 0;
int main(){
    // Lectura de los datos
    ifstream archivo ("twitter_users.csv");
    ifstream archivo2 ("twitter_connections.csv");
    string linea;
    char delimitador = ';';
    getline(archivo,linea);
    map<string,profile> lista;
    map<string,int> conteo_influyentes; // en este mapa cuenta las veces que aparecen los usuarios como seguidos
    map<string,int> conteo_influenciables; // en este mapa cuenta las veces que aparecen los usuarios como seguidores
    //chrono::high_resolution_clock::time_point inicio = chrono::high_resolution_clock::now();
    while(getline(archivo,linea)){
        profile perfil;
        string aux;
        stringstream stream(linea);
        getline(stream,aux,delimitador); // no se guarda
        getline(stream,perfil.username,delimitador);
        getline(stream,aux,delimitador);
        perfil.number_tweets = stoi(aux);
        getline(stream,aux,delimitador);
        perfil.friends = stoi(aux);
        getline(stream,aux,delimitador);
        perfil.followers = stoi(aux);
        getline(stream,aux,delimitador); // no se guarda
        /// almacenar el perfil en una estructura de datos///
        perfil.posicion = cantidad_nodos; // se le asigna una posicion fija tipo "id" a cada nodo
        lista.insert(pair<string,profile>(perfil.username,perfil)); // se inserta en el mapa como el username como llave
        cantidad_nodos++;
    }
    getline(archivo2,linea);
    int espacio_base = cantidad_nodos*sizeof(profile); // se calcula el espacio de la estructura base
    while(getline(archivo2,linea)){ // lista de adyacencia
        stringstream stream(linea);
        string llave,seguidor;
        getline(stream,llave,delimitador);
        getline(stream,seguidor,delimitador);
        lista[llave].conexion.push_back(&(lista[seguidor])); // mete en la lista de conexiones un puntero al perfil de los seguidores y está podada.
            lista[seguidor].conexion_reverse.push_back(&(lista[llave])); // guarda la lista reversa de las conexiones
            conteo_influyentes[llave]+=1; // se cuenta para los influyentes
            conteo_influenciables[seguidor]+=1; // se cuenta para los influenciables
            espacio_base += sizeof(profile*); // se le suma a la estructura base cuantos punteros se agregan en las listas de adyacencia
    }
    float espacio_total = (espacio_base + sizeof(map<string,profile>))/1024; // se calcula el espacio total en KB
    //cout<<"tamaño mapa en KB: "<< espacio_total<<endl;
    float espacio_MB = espacio_total/1024; // se calcula el espacio total en MB
    //cout<<"tamaño mapa en MB: "<< espacio_MB<<endl;
    /*chrono::high_resolution_clock::time_point t_end_mil = chrono::high_resolution_clock::now();
    chrono::duration<double,milli> time_span = chrono::duration_cast<chrono::duration<double,milli>>(t_end_mil - inicio);
    cout<<"Tiempo de guardado de datos: "<<time_span.count()<<endl;
    */
    visited = new bool[cantidad_nodos]; // vector booleano que marcará si algun nodo fue visitado a través de la posicion que se les fué dada como "id"
    for(int i=0;i<cantidad_nodos;i++) visited[i] = false;
    lista["Cooperativa"].tendencia_politica[0]=1; // se inicializan las tendencias politicas de los medios de ocmunicación
    lista["soyvaldiviacl"].tendencia_politica[1]=1;
    lista["latercera"].tendencia_politica[2]=1;
    lista["elmostrador"].tendencia_politica[3]=1;

    //cout<<"flags: " << lista["mariade50285440"].tendencia_politica[0] << lista["mariade50285440"].tendencia_politica[1] <<lista["mariade50285440"].tendencia_politica[2] <<lista["mariade50285440"].tendencia_politica[3]<<"|| size de la lista:" << lista["mariade50285440"].conexion.size()<<endl;
    ///ciclo for
    //inicio = chrono::high_resolution_clock::now();
    DFS(&lista["Cooperativa"],0); // se hace un DFS desde cooperativa para encontrar los que tengan tendencias politicas en esta dirección
    for(int i=0;i<cantidad_nodos;i++) visited[i] = false; // se vuelve a dejar el vector booleano en falso
    DFS(&lista["soyvaldiviacl"],1); // ahora se hace el DFS para soyvaldivia, así para todos los medios
    for(int i=0;i<cantidad_nodos;i++) visited[i] = false;
    DFS(&lista["latercera"],2);
    for(int i=0;i<cantidad_nodos;i++) visited[i] = false;
    DFS(&lista["elmostrador"],3);
    for(int i=0;i<cantidad_nodos;i++) visited[i] = false;
    //tendencias(perfil, contador);
    /// Calculo de tendencias politicas
    for(map<string,profile>::iterator it = lista.begin();it!=lista.end();it++){
        double acum = 0;
        for(int i=0;i<4;i++){
            acum = acum + (it->second.tendencia_politica[i]); // se suman las tendencias en de los 4 sectores
        }
        for(int i=0;i<4;i++){
            double aux2 = it->second.tendencia_politica[i];
            aux2 = (aux2/acum)*100;
            if(acum!=0){it->second.tendencia_politica[i]=aux2;} // se divide cada tendencia de cada sector en la suma de las 4 tendencias para poder normalizarlas y llevarlas a %
        }
    }
    /*t_end_mil = chrono::high_resolution_clock::now();
    time_span = chrono::duration_cast<chrono::duration<double,milli>>(t_end_mil - inicio);
    cout<<"Tiempo para calcular tendencias: "<<time_span.count()<<endl;
    */
    /// imprimir puntajes ///
    for(map<string,profile>::iterator it = lista.begin();it!=lista.end();it++){
        //cout<<it->second.username<<"||"<<setprecision(4)<<it->second.tendencia_politica[0]<<"|"<<it->second.tendencia_politica[1]<<"|"<<it->second.tendencia_politica[2]<<"|"<<it->second.tendencia_politica[3]<<endl;
    }
    /// calculo e impresion de las componentes conexas ///
    //inicio = chrono::high_resolution_clock::now();
    //Componentes_fuertes_conexas(lista); // Esta es la función que calcula las componentes conexas
    /*t_end_mil = chrono::high_resolution_clock::now();
    time_span = chrono::duration_cast<chrono::duration<double,milli>>(t_end_mil - inicio);
    cout<<"Tiempo para mostrar componentes conexas: "<<time_span.count()<<endl;
    */
    //inicio = chrono::high_resolution_clock::now();
    //influencia(conteo_influyentes,conteo_influenciables); // esta funcion calcula el top de influencia de los usuarios
    /*t_end_mil = chrono::high_resolution_clock::now();
    time_span = chrono::duration_cast<chrono::duration<double,milli>>(t_end_mil - inicio);
    cout<<"Tiempo para mostrar influencia: "<<time_span.count()<<endl;*/
    delete[] visited;
    return 0;
}
double caminos = 0;
int cont_nodos_conexos=0;
int cont_grupos_conexos=0;
void DFS(profile *v,int sw){
    if(sw==5) { // si el sw es igual a 5 funciona como un DFS normal, pero utiliza la lista reversa de adyacencia y no hace cambio en las tendencias
        if(v->posicion<cantidad_nodos && visited[v->posicion] == false){
            //cout<<v->username<<"||";
            cont_nodos_conexos++; // se suman la cantidad de nodos conexos hasta salir del DFS
        }
        if(v->posicion<cantidad_nodos)visited[v->posicion] = true; //marca el nodo actual como visitado
        for(list<profile*>::iterator it = v->conexion_reverse.begin(); it!=v->conexion_reverse.end();++it){ //nodos adyacentes
                if((*it)->posicion<cantidad_nodos){
                   if(!visited[(*it)->posicion]){ // si el nodo es falso entonces se llama recursivamente a la funcion
                    DFS(*it,sw);
                    }
                }
        }
    }
    if(sw<4){ // si el switch es igual a <4 entonces se cuentan los caminos y se calcula la tendencia por medio de eso.
        //cout<<v->username<<"||"<< v->tendencia_politica[sw] <<" "<<caminos<<" "<<1/(caminos+1)<<endl;
        if(v->posicion<cantidad_nodos)visited[v->posicion] = true; //marca el nodo actual como visitado e imprimelo
        if(v->username=="Cooperativa" || v->username=="soyvaldiviacl"||v->username=="latercera"||v->username=="elmostrador"){
        }else{if(sw<4)v->tendencia_politica[sw] = 1/caminos;}
        for(list<profile*>::iterator it = v->conexion.begin(); it!=v->conexion.end();++it){ //nodos adyacentes
            if(!visited[(*it)->posicion]){
                caminos++; // se suman la cantidad de caminos
                DFS(*it,sw); // se llama recursivamente a la funcion
                caminos--; // cuando se vuelve del DFS quiere decir que se empezó a devolver, por lo que se deben restar los caminos
            }
        }
    }
}
void Componentes_fuertes_conexas(map<string,profile> lista_dir){
    stack<profile*> stk; // se crea un stack con punteros a profile
    for(int i=0;i<cantidad_nodos;i++) visited[i]=false; // se vuelve a poner el vector booleano de nodos visitados en falso
    for(map<string,profile>::iterator i=lista_dir.begin();i!=lista_dir.end();i++){
        if(visited[i->second.posicion] == false){
            Ordenado(&(i->second),stk); // se llama a una funcion DFS que guarda en el stack los nodos a medida que se van visitando para asi conocer el orden inverso de los nodos
        }
    }
    for(int i=0;i<cantidad_nodos;i++) visited[i]=false; // se vuelve a poner el vector booleano de nodos visitados en falso
    while (!stk.empty()){
        profile *v = stk.top(); // se almacena el top del stack en una variable auxiliar y luego se hace pop
        stk.pop();
        if (visited[v->posicion] == false)
        {
            DFS(v, 5); //sw-> no cambia tendencias politicas, se llama al DFS con el vector V
            //cout << endl;
            cout<< "LA CANTIDAD DE NODOS CONEXOS ES: "<< cont_nodos_conexos<<endl;
            cont_nodos_conexos = 0; // se devuelve el contador a cero
            cont_grupos_conexos++;
        }
    }
    cout<< "LA CANTIDAD DE GRUPOS CONEXOS SON: "<< cont_grupos_conexos<<endl;
}
void Ordenado(profile *perfil , stack<profile*> &Stack)
{
    visited[perfil->posicion] = true; // se marca visitado
    list<profile*>::iterator i;
    for(i = perfil->conexion.begin(); i != perfil->conexion.end(); ++i)
        if(!visited[(*i)->posicion]) // DFS normal
            Ordenado(*i,Stack);
    Stack.push(perfil);  // se hace un push del perfil del stack
}
void influencia(map<string,int> influyentes, map<string,int> influenciables){
    list<pair<string,int>> influyentescount; // se guardarán los influyentes en esta lista de forma ordenada
    list<pair<string,int>> influenciablescount; // se guardarán los influenciables en esta lista de forma ordenada
    for(int i=0;i<10;i++){
        int aux =0;
        string auxs = " ";
        map<string,int>::iterator itaux;
        for(map<string,int>::iterator it= influyentes.begin();it!=influyentes.end();it++){
            //cout<< "Nombre: "<<": "<<it->first<<" Influencia: "<<it->second<<endl;
            if(it->second >aux){ // busca el máximo
                aux=it->second;
                auxs = it->first;
                itaux=it;
            }
        }
        itaux->second=0;
        influyentescount.push_back(make_pair(auxs,aux)); // al terminar hace push del valor máximo encontrado
    }
    for(list<pair<string,int>>::iterator it= influyentescount.begin();it!=influyentescount.end();it++){
        cout<< "Nombre: "<<": "<<it->first<<" Seguidores: "<<it->second<<endl;
    }
    cout<<"-------------------------------------"<<endl;
    for(int i=0;i<10;i++){
        int aux =0;
        string auxs = " ";
        map<string,int>::iterator itaux;
        for(map<string,int>::iterator it= influenciables.begin();it!=influenciables.end();it++){
            //cout<< "Nombre: "<<": "<<it->first<<" Influencia: "<<it->second<<endl;
            if(it->second >aux){ // se busca el máximo
                aux=it->second;
                auxs = it->first;
                itaux=it;
            }
        }
        itaux->second=0;
        influenciablescount.push_back(make_pair(auxs,aux)); // al terminar hace push del valor máximo encontrado
    }
    for(list<pair<string,int>>::iterator it= influenciablescount.begin();it!=influenciablescount.end();it++){
        cout<< "Nombre: "<<": "<<it->first<<" Seguidos: "<<it->second<<endl;
    }
}
