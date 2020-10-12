#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>		//usleep
#include <bits/stdc++.h> 
#include <sstream> 
#include <fstream>

using namespace std;

int counter = 0, thread_count=6, releaser = 0;
pthread_mutex_t mutex_forvar;
pthread_cond_t cond_var;

string buffer[64];
string bufferclave[64];

int bufferpermutacion1[64];
int izquierda[32];
int derecha[32];






int permutacionInicial[64] = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17,  9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
    };
    


//Subrutina para leer el archivo de texto y posteriormente convertir los caracteres a binario.
 
void *Leer(void *palabra1){
	
	std::string *palabra2 = static_cast<std::string*>(palabra1);
	string palabra = *palabra2;
	int n = palabra.length();
	
	if(palabra.length() %8 !=0){
        int contador = palabra.length() %8;
        for(contador; contador <8; contador++){
            palabra+= " ";
        }
    }
	for(int i=0; i<=n;i++){
		int val = int(palabra[i]);
		string bin = "";
		while(val>0){
            (val % 2)? bin.push_back('1') : 
                       bin.push_back('0'); 
            val /= 2;		
		} 	
        reverse(bin.begin(), bin.end());
		if(i<n){
			buffer[i]=  bin;
		}
	}
return NULL;
}

void *Leer2(void *palabra1){
	
	std::string *palabra2 = static_cast<std::string*>(palabra1);
	string palabra = *palabra2;
	int n = palabra.length();
	
	if(palabra.length() %8 !=0){
        int contador = palabra.length() %8;
        for(contador; contador <8; contador++){
            palabra+= " ";
        }
    }
	for(int i=0; i<=n;i++){
		int val = int(palabra[i]);
		string bin = "";
		while(val>0){
            (val % 2)? bin.push_back('1') : 
                       bin.push_back('0'); 
            val /= 2;		
		} 	
        reverse(bin.begin(), bin.end());
		if(i<n){
			bufferclave[i]=  bin;
		}
	}
return NULL;
}

void *Permutacion1(NULL){	
	for(int i=0; i<64; i++){
		bufferpermutacion1[i]= stoi(buffer[permutacionInicial[i]-1]);
	}
	
	int k = 33;
	for(int j=0; j<32; j++){
		izquierda[j] = bufferpermutacion1[j];
		derecha[j] = bufferpermutacion1[k];
		k++;
	}	
}


int main(){

	pthread_t thread, thread2;
	pthread_t threadC1;
	string numero;	
	string texto;
	string clave;
	
	ifstream palabra("FUENTE.txt",ios::in);
	
	if(!palabra)
	{
		cerr<<"No se pudo leer el archivo"<<endl;
		exit(EXIT_FAILURE);
	}
		
		while(palabra>>numero){
			texto += " " + numero;	
		}
	

	pthread_create(&thread,NULL,Leer,(void*)&texto);
	pthread_join(thread, NULL);
	
	pthread_create(&thread2,NULL,Permutacion1,NULL);
	pthread_join(thread2, NULL)	
	
	
	
	
    cout << "Bienvenido introduce la clave a utilizar para cifrar: ";
	cin >> clave; 	

	pthread_create(&threadC1,NULL,Leer,(void*)&clave);
	pthread_join(threadC1, NULL);

	
	
	}
