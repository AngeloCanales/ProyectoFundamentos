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
    map<string,int> conteo_influyentes;
    map<string,int> conteo_influenciables;
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
        lista.insert(pair<string,profile>(perfil.username,perfil));
        perfil.posicion = cantidad_nodos;
        cantidad_nodos++;
    }
    cout<<lista["Cooperativa"].posicion<<"||"<<lista["Cooperativa"].number_tweets<<endl;
    getline(archivo2,linea);
    int espacio_base = cantidad_nodos*sizeof(profile);
    while(getline(archivo2,linea)){ // lista de adyacencia
        stringstream stream(linea);
        string llave,seguidor;
        getline(stream,llave,delimitador);
        getline(stream,seguidor,delimitador);
        if(seguidor=="Cooperativa" || seguidor=="soyvaldiviacl"||seguidor=="latercera"||seguidor=="elmostrador"){
        }else{
            lista[llave].conexion.push_back(&(lista[seguidor])); // mete en la lista de conexiones un puntero al perfil de los seguidores y está podada.
            lista[seguidor].conexion_reverse.push_back(&(lista[llave])); // guarda la lista reversa de las conexiones
            //cout<<lista[seguidor].conexion_reverse.back()->username<<"||"<<seguidor<<endl;
            //cout<<lista[llave].conexion.back()->username<<"||"<<llave<<endl;
            conteo_influyentes[llave]+=1;
            conteo_influenciables[seguidor]+=1;
            espacio_base += sizeof(profile*);
        }
    }
    float espacio_total = (espacio_base + sizeof(map<string,profile>))/1024;
    //cout<<"tamaño mapa en kB: "<< espacio_total<<endl;
    float espacio_MB = espacio_total/1024;
    //cout<<"tamaño mapa en MB: "<< espacio_MB<<endl;
    /*chrono::high_resolution_clock::time_point t_end_mil = chrono::high_resolution_clock::now();
    chrono::duration<double,milli> time_span = chrono::duration_cast<chrono::duration<double,milli>>(t_end_mil - inicio);
    cout<<"Tiempo de guardado de datos: "<<time_span.count()<<endl;
    */
    visited = new bool[cantidad_nodos];
    for(int i=0;i<cantidad_nodos;i++) visited[i] = false;
    lista["Cooperativa"].tendencia_politica[0]=1;
    lista["soyvaldiviacl"].tendencia_politica[1]=1;
    lista["latercera"].tendencia_politica[2]=1;
    lista["elmostrador"].tendencia_politica[3]=1;

    //cout<<"flags: " << lista["mariade50285440"].tendencia_politica[0] << lista["mariade50285440"].tendencia_politica[1] <<lista["mariade50285440"].tendencia_politica[2] <<lista["mariade50285440"].tendencia_politica[3]<<"|| size de la lista:" << lista["mariade50285440"].conexion.size()<<endl;
    ///ciclo for
    //inicio = chrono::high_resolution_clock::now();
    /*DFS(&lista["Cooperativa"],0);
    for(int i=0;i<cantidad_nodos;i++) visited[i] = false;
    DFS(&lista["soyvaldiviacl"],1);
    for(int i=0;i<cantidad_nodos;i++) visited[i] = false;
    DFS(&lista["latercera"],2);
    for(int i=0;i<cantidad_nodos;i++) visited[i] = false;
    DFS(&lista["elmostrador"],3);
    for(int i=0;i<cantidad_nodos;i++) visited[i] = false;*/
    //tendencias(perfil, contador);
    /// Calculo de tendencias politicas
    for(map<string,profile>::iterator it = lista.begin();it!=lista.end();it++){
        double acum = 0;
        for(int i=0;i<4;i++){
            acum = acum + (it->second.tendencia_politica[i]);
        }
        for(int i=0;i<4;i++){
            double aux2 = it->second.tendencia_politica[i];
            aux2 = (aux2/acum)*100;
            if(acum!=0){it->second.tendencia_politica[i]=aux2;}
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
    Componentes_fuertes_conexas(lista);
    /*t_end_mil = chrono::high_resolution_clock::now();
    time_span = chrono::duration_cast<chrono::duration<double,milli>>(t_end_mil - inicio);
    cout<<"Tiempo para mostrar componentes conexas: "<<time_span.count()<<endl;
    */
    //inicio = chrono::high_resolution_clock::now();
    //influencia(conteo_influyentes,conteo_influenciables);
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
    if(sw==5) {
        if(v->username=="Cooperativa"){cout<<"si existe"<<endl;}
        if(v->posicion<cantidad_nodos && visited[v->posicion] == false){
            //cout<<v->username<<"||";
            cont_nodos_conexos++;
        }
        if(v->posicion<cantidad_nodos)visited[v->posicion] = true; //marca el nodo actual como visitado e imprimelo
        for(list<profile*>::iterator it = v->conexion_reverse.begin(); it!=v->conexion_reverse.end();++it){ //nodos adyacentes
                if((*it)->posicion<cantidad_nodos){
                   if(!visited[(*it)->posicion]){
                    DFS(*it,sw);
                    }
                }
        }
    }
    if(sw<4){
        //cout<<v->username<<"||"<< v->tendencia_politica[sw] <<" "<<caminos<<" "<<1/(caminos+1)<<endl;
        if(v->posicion<cantidad_nodos)visited[v->posicion] = true; //marca el nodo actual como visitado e imprimelo
        if(v->username=="Cooperativa" || v->username=="soyvaldiviacl"||v->username=="latercera"||v->username=="elmostrador"){
        }else{if(sw<4)v->tendencia_politica[sw] = 1/caminos;}
        for(list<profile*>::iterator it = v->conexion.begin(); it!=v->conexion.end();++it){ //nodos adyacentes
            if(!visited[(*it)->posicion]){
                caminos++;
                DFS(*it,sw);
                caminos--;
            }
        }
    }
}
void Componentes_fuertes_conexas(map<string,profile> lista_dir){
    stack<profile*> stk;
    for(int i=0;i<cantidad_nodos;i++) visited[i]=false;
    for(map<string,profile>::iterator i=lista_dir.begin();i!=lista_dir.end();i++){
        if(visited[i->second.posicion] == false){
            Ordenado(&(i->second),stk);
        }
    }
    for(int i=0;i<cantidad_nodos;i++) visited[i]=false;
    while (!stk.empty()){
        profile *v = stk.top();
        stk.pop();
        if (visited[v->posicion] == false)
        {
            DFS(v, 5); //sw-> no cambia tendencias politicas
            //cout << endl;
            //cout<< "LA CANTIDAD DE NODOS CONEXOS ES: "<< cont_nodos_conexos<<endl;
            cont_nodos_conexos = 0;
            cont_grupos_conexos++;
        }
    }
    //cout<< "LA CANTIDAD DE GRUPOS CONEXOS SON: "<< cont_grupos_conexos<<endl;
}
void Ordenado(profile *perfil , stack<profile*> &Stack)
{
    visited[perfil->posicion] = true;
    list<profile*>::iterator i;
    for(i = perfil->conexion.begin(); i != perfil->conexion.end(); ++i)
        if(!visited[(*i)->posicion])
            Ordenado(*i,Stack);
    Stack.push(perfil);
}
void influencia(map<string,int> influyentes, map<string,int> influenciables){
    list<pair<string,int>> influyentescount;
    list<pair<string,int>> influenciablescount;
    for(int i=0;i<10;i++){
        int aux =0;
        string auxs = " ";
        map<string,int>::iterator itaux;
        for(map<string,int>::iterator it= influyentes.begin();it!=influyentes.end();it++){
            //cout<< "Nombre: "<<": "<<it->first<<" Influencia: "<<it->second<<endl;
            if(it->second >aux){
                aux=it->second;
                auxs = it->first;
                itaux=it;
            }
        }
        itaux->second=0;
        influyentescount.push_back(make_pair(auxs,aux));
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
            if(it->second >aux){
                aux=it->second;
                auxs = it->first;
                itaux=it;
            }
        }
        itaux->second=0;
        influenciablescount.push_back(make_pair(auxs,aux));
    }
    for(list<pair<string,int>>::iterator it= influenciablescount.begin();it!=influenciablescount.end();it++){
        cout<< "Nombre: "<<": "<<it->first<<" Seguidos: "<<it->second<<endl;
    }
}
