#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>		//usleep
#include <bits/stdc++.h> 
#include <sstream> 
#include <fstream>
//Hecho por Gabriel Quiroz 19255 y Jose Pablo Ponce 19092
//17/09/2020
//El otro companero con el que ibamos nos comento que no podia trabajar, por lo que no hizo nada.
using namespace std;
/*----- Variables globales compartidas -----*/
int counter = 0, thread_count=6, releaser = 0;
pthread_mutex_t mutex_forvar;
pthread_cond_t cond_var;

string buffer[100];
char newbuffer[100];
int newlista[8];
int bufferexpxorki[48];
int bufferexpansion[48];
int E[] = {
	32,  1,  2,  3,  4,  5,
	 4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};

/* The S-Box tables */
int S[8][64] = { {
		/* S1 */
		14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
		 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
		 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
		15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
	},{
		/* S2 */
		15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
		 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
		 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
		13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
	},{
		/* S3 */
		10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
		13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
		13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
		 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
	},{
		/* S4 */
		 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
		13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
		10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
		 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
	},{
		/* S5 */
		 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
		14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
		 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
		11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
	},{
		/* S6 */
		12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
		10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
		 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
		 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
	},{
		/* S7 */
		 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
		13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
		 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
		 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
	},{
		/* S8 */
		13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
		 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
		 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
		 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
	} };
int Sbox1[6];
int Sbox2[6];
int Sbox3[6];
int Sbox4[6];
int Sbox5[6];
int Sbox6[6];
int Sbox7[6];
int Sbox8[6];

void* expansion(void * arg) {
	for(int i = 0; i < 48;i++) {
		bufferexpansion[i] = E[i]-1;
	}
}

void* Sboxes(void* arg) {
	for (int i = 0; i < 48; i++) {
		bufferexpxorki[i] = bufferexpansion[i] ^ bufferki[i];
	}
	for (int i = 0; i < 6;i++) {
		Sbox1[i] = bufferexpxorki[i];
	}
	for (int i = 0; i < 6; i++) {
		Sbox2[i] = bufferexpxorki[i+6];
	}
	for (int i = 0; i < 6; i++) {
		Sbox3[i] = bufferexpxorki[i + 12];
	}
	for (int i = 0; i < 6; i++) {
		Sbox4[i] = bufferexpxorki[i + 18];
	}
	for (int i = 0; i < 6; i++) {
		Sbox5[i] = bufferexpxorki[i + 24];
	}
	for (int i = 0; i < 6; i++) {
		Sbox6[i] = bufferexpxorki[i + 30];
	}
	for (int i = 0; i < 6; i++) {
		Sbox7[i] = bufferexpxorki[i + 36];
	}
	for (int i = 0; i < 6; i++) {
		Sbox8[i] = bufferexpxorki[i + 42];
	}


}



//Barrera utilizando variables condicionales.
//Hecha por Jose Pablo Ponce y Gabriel Quiroz
void *barrera_condicional(void * arg){
	//**Barrier**
	int my_rank = (int)arg;
	pthread_mutex_lock(&mutex_forvar);
	counter++;
	if(counter == thread_count){
	counter = 0;
	
	//--Notify all waiting threads--
	pthread_cond_broadcast(&cond_var);
	}
	else{
		while(pthread_cond_wait(&cond_var, &mutex_forvar) != 0);
	}
//**Release barrier**
	releaser++;
	pthread_mutex_unlock(&mutex_forvar);
	pthread_exit(0);
	return NULL;
}

//Subrutina para leer el archivo de texto y posteriormente convertir los caracteres a binario.
//Hecha por Jose Pablo Ponce  
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
	//for (int k=0; k<100; k++) 
    //cout << buffer[k]<<" ";
	
	
return NULL;
}

//Subrutina ronda para realizar la encriptacion.
//Hecha por Jose Pablo Ponce y Gabriel Quiroz
void *ronda(void *){
	for(int j = 0; j<88; j=j){
		for(int i = 0; i <8; i++){
			newlista[i] = stoi(buffer[j]);
			j++;
			if(i == 7){
				int resultado1 = newlista[4] ^ 01001101; //M
				int resultado2 = newlista[5] ^ 01011001; //Y
				int resultado3 = newlista[6] ^ 01001011; //K
				int resultado4 = newlista[7] ^ 01000101; //E
				int caracter1 =stoi(buffer[0]);
				int caracter2 =stoi(buffer[1]);
				int caracter3 =stoi(buffer[2]);
				int caracter4 =stoi(buffer[3]);
				
				newlista[0] = resultado1;
				newlista[1] = resultado2;
				newlista[2] = resultado3;
				newlista[3] = resultado4;
				newlista[4] = caracter1;
				newlista[5] = caracter2;
				newlista[6] = caracter3;
				newlista[7] = caracter4;
			for(int p = 0; p<88; p=p){
				for(int k = 0; k<8; k++){
					buffer[p] = to_string(newlista[k]);
					p++;
				
				}
			}
				
			}
			
		}
	}
	
	return NULL;
}

//Subrutina hecha para la escritura del archivo salida.bin
//Hecha por Jose Pablo Ponce y Gabriel Quiroz
void* escritura(void *){
	
	//for (int k=0; k<100; k++) 
    //cout << buffer[k]<<" ";
	
	for(int i=0; i<100;i++){

        newbuffer[i]= (char)atoi(buffer[i].c_str());

    }
		

	
	//for (int k=0; k<8; k++) 
//cout << newbuffer[k]<<" ";		
    
	ofstream myFile ("salida.bin", ios::out | ios::binary);
	myFile.write (newbuffer, 100);	
    
	myFile.close();
	return NULL;
	
	
}

//Main del programa en el que se utilizan threads para cada subrutina
//Hecha por Jose Pablo Ponce y Gabriel Quiroz
int main(){

	pthread_t thread, thread2[4], thread3;
	pthread_t hijo[thread_count];
	pthread_mutex_init(&mutex_forvar, NULL); 
	pthread_cond_init(&cond_var, NULL); 
	

	string numero;	
	string texto;

	ifstream palabra("FUENTE.txt",ios::in);
	
	
	if(!palabra)
	{
		cerr<<"No se pudo leer el archivo"<<endl;
		exit(EXIT_FAILURE);
	}
		
		while(palabra>>numero){
			texto += " " + numero;
		
		}
		cout << texto <<" ";
		cout << "\nCompletada con exito la encriptacion.\n Busque su archivo salida.bin" <<" ";

	pthread_create(&thread,NULL,Leer,(void*)&texto);
	pthread_join(thread, NULL);
  
    
   for (int j=0; j<4; j++) {
	   pthread_create(&thread2[j], NULL, ronda, NULL);
	   
} 

   for (int j=0; j<4; j++) {
	   pthread_join(thread2[j], NULL);
} 

	for(int i = 0; i < thread_count; i++)
	{
		pthread_create(&hijo[i],NULL,barrera_condicional,(void *)i);
	}      


  //  for (int k=0; k<8; k++) 
    //cout << buffer[k]<<" ";

		
		pthread_create(&thread3, NULL, escritura, NULL);
		pthread_join(thread3,NULL);

    



	
}
