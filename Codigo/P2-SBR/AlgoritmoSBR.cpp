#include "bhClass.h"
#include "bcClass.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>

using namespace std;

vector<reglaClass> equiparar(class bcClass& bc, string Meta);
double CalculoCaso1(vector<double> v, char t);
double CalculoCaso2(vector<double> v);
void imprimirCaso1(vector<double> v, char t,ofstream& of);

double verificar(class bcClass& bc, class bhClass& bh, string Meta, ofstream& MyFile){

    double r = 2.5;
    if(bh.getMapaHechos().count(Meta)){
        r = bh.getMapaHechos().at(Meta);
        return r;
    }
    else{
        vector<reglaClass> conjuntoConflicto = equiparar(bc, Meta);
        vector<reglaClass> salida = conjuntoConflicto;
        vector<double> listaCaso2;
        vector<double> listaCaso1;
        vector<string> NuevasMetas;
        string NMet;
        double aux;
        double aux2;

        while(!conjuntoConflicto.empty()){
            reglaClass r(conjuntoConflicto.back());
            conjuntoConflicto.pop_back();
            NuevasMetas = r.getParteIzquierda();

            while(!NuevasMetas.empty()){
                NMet = NuevasMetas.back();
                NuevasMetas.pop_back();
                aux = verificar(bc, bh, NMet, MyFile);
                listaCaso1.push_back(aux);

            }
            aux2 = CalculoCaso1(listaCaso1, r.getTipo());
            if (listaCaso1.size() > 1){
                MyFile << "CASO-1: ";
                r.salidaRegla(MyFile);
                imprimirCaso1(listaCaso1, r.getTipo(), MyFile);
                MyFile << "Resultado: " << aux2 << "\n";
            }

            NuevasMetas.clear();
            listaCaso1.clear();

            MyFile << "CASO-3: ";
            r.salidaRegla(MyFile);
            MyFile << "\tFC: "<< r.getFactorCertezaRegla() << " * max(0, " << aux2 << ")\n";

            aux2 = r.getFactorCertezaRegla() * max(aux2, (double)0);

            MyFile << " = "<< aux2 << "\n";

            listaCaso2.push_back(aux2);
        }
        double aux3 = CalculoCaso2(listaCaso2);

        if (salida.size() > 1){
            MyFile << "CASO-2:\n ";
            for(auto i: salida){
                MyFile << "\t";
                i.salidaRegla(MyFile);
            }
            MyFile << "\tResultado del Calculo: " << aux3 <<"\n";
        }

        listaCaso2.clear();
        bh.aniadirEntradaMapa(Meta, aux3);
        bh.incrementarNumeroHechos();

        return aux3;
    }
}

void imprimirCaso1(vector<double> v, char t,ofstream& of){
    if (t == 'O'){
        of << "\tmax(";
        for (auto i: v){
            of << i << " ";
        }
        of << ")\n";
    }
    else if (t == 'Y'){
        of << "\tmin(";
        for (auto i: v){
            of << i << " ";
        }
        of << ")\n";
    }
}

vector<reglaClass> equiparar(class bcClass& bc, string Meta){
    vector <reglaClass> r;
    map<string, reglaClass> mapa = bc.getMapaReglas();
    for (auto i : mapa){
        if (Meta == i.second.getParteDerecha()){
            r.push_back(i.second);
        }
    }
    return r;
}

double CalculoCaso1(vector<double> v, char t){
    double r;
    if(t == 'N'){
        r = v.front();
    }
    else if (t == 'O'){
        r = v.front();
        for (auto i: v){
            if (i > r) r = i;
        }
    }
    else if (t == 'Y'){
        r = v.front();
        for (auto i: v){
            if (i < r) r = i;
        }
    }
    return r;
}

double CalculoCaso2(vector<double> v){
    double aux = v.front();
    for (size_t i = 1; i < v.size(); i++){
        if ((aux >= 0) && (v[i] >= 0)){
            aux = aux + v[i]*(1-aux);
        }
        else if((aux <= 0) && (v[i] <= 0)){
            aux = aux + v[i]*(1+aux);
        }
        else{
            aux = (aux+v[i])/(1-min(abs(aux), abs(v[i])));
        }
    }
    double r = aux;
    return r;
}





