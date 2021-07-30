#include <string>
#include <list>
using namespace std;

#ifndef profile_cpp
#define profile_cpp

class profile{
public:
    string username;
    int posicion;
    int number_tweets;
    int friends;
    int followers;
    double tendencia_politica[4]={0,0, 0, 0}; // [izq der centro libertario] %
    list<profile*> conexion;
    list<profile*> conexion_reverse;
private:
};

#endif // profile_cpp
