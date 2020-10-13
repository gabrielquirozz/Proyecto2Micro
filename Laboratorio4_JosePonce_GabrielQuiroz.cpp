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
int bufferpermutacion1[64];
int izquierda[32];
int derecha[32];

string bufferclave64[64];
int bufferclave56permutacionpc1[56];
int izquierdaclave[28];
int derechaclave[28];
int izquierdadesplazadoclave[28];
int derechadesplazadoclave[28];
int izquierdayDerechaClaveDesplazamiento2bit[56]
int bufferclave48permutacionpc2[48];





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
    
int permutacionPC1[56] = {
        57, 49, 41, 33, 25, 17,  9,
         1, 58, 50, 42, 34, 26, 18,
        10,  2, 59, 51, 43, 35, 27,
        19, 11,  3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
         7, 62, 54, 46, 38, 30, 22,
        14,  6, 61, 53, 45, 37, 29,
        21, 13, 5 , 28, 20, 12,  4
    };
	
	
int permutacionPC2[48] = {
        14, 17, 11, 24,  1,  5,
         3, 28, 15,  6, 21, 10,
        23, 19, 12,  4, 26,  8,
        16,  7, 27, 20, 13,  2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
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
			bufferclave64[i]=  bin;
		}
	}
	
return NULL;

}

void *PermutacionP1(NULL){	
	for(int i=0; i<64; i++){
		bufferpermutacion1[i]= stoi(buffer[permutacionInicial[i]-1]);
	}
	
	int k = 32;
	for(int j=0; j<32; j++){
		izquierda[j] = bufferpermutacion1[j];
		derecha[j] = bufferpermutacion1[k];
		k++;
	}	

return NULL;

}

void *PermutacionPC1F(NULL){	

	for(int i=0; i<56; i++){
		bufferclave56permutacionpc1[i]= stoi(bufferclave64[permutacionPC1[i]-1]);
	}
	
	int k = 28;
	for(int j=0; j<28; j++){
		izquierdaclave[j] = bufferclave56permutacionpc1[j];
		derechaclave[j] = bufferclave56permutacionpc1[k];
		k++;
	}		
	
return NULL;
	
}


void *ClaveDesplazamientoLS(NULL){	

izquierdadesplazadoclave[26] = izquierdaclave[0];
izquierdadesplazadoclave[27] = izquierdaclave[1];
derechadesplazadoclave[26] = derechaclave[0];
derechadesplazadoclave[27] = derechaclave[1];

for(int i =0; i<26; i++){
	
	izquierdadesplazadoclave[i] = izquierdaclave[i+2];
	derechadesplazadoclave[i]=  derechaclave[i+2]
}

for (int k=0; k<28; k++){

	izquierdayDerechaClaveDesplazamiento2bit[k] = izquierdadesplazadoclave[k];
	izquierdayDerechaClaveDesplazamiento2bit[28+k] = derechadesplazadoclave[k];
	
}


return NULL;


}

void *PermutacionPC2F(NULL){	

	for(int i=0; i<48; i++){
		bufferclave48permutacionpc2[i]= izquierdayDerechaClaveDesplazamiento2bit[permutacionPC2[i]-1]);
	}
	

}


int main(){

	pthread_t thread, thread2;
	pthread_t threadC1, threadC2, threadC3;
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

	pthread_create(&threadC2,NULL,PermutacionPC1F, NULL);
	pthread_join(threadC2, NULL);
	
	pthread_create(&threadC3,NULL,ClaveDesplazamientoLS, NULL);
	pthread_join(threadC3, NULL);
	
	
	
	}

