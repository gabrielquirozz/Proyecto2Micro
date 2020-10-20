#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>		//usleep
#include <bits/stdc++.h> 
#include <sstream> 
#include <fstream>
#include <cstdlib>
#include <string>
#include<string>

//Gabriel Quiroz 19255 y Jose Pablo Ponce 19092
//Microprocesadores
//Proyecto2


using namespace std;

pthread_mutex_t mutex1;//variable mutex

//variables de almacenamiento de texto a cifrar
int buffer[64];	//buffer inicial para almacenar el texto en binario
int bufferpermutacion1[64]; //buffer para almacenar luego de la permutacion inicial
int izquierda[32];	//buffer para almacenar los primeros 32 bits luego de la permutacion inicial
int derecha[32]; //buffer para almacenar los ultimos 32 bits luego de la permutacion inicial
int izquierdanueva[32]; //buffer utilizado para las 8 rondas para almacenar la izquierda nueva
int derechanueva[32]; //buffer utilizado para las 8 rondas para almacenar la derecha nueva
int bufferintercambioDI[64]; // buffer utilizado para intercambiar y almacenar la derecha e izquierda
int bufferfinal[64]; //buffer utilizado para almacenar el texto cifrado luego de permutacion inversa

//variables de almacenamiento de clave
int bufferclave64[64];	//buffer inicial para almacenar la clave en binario
int bufferclave56permutacionpc1[56]; //buffer para almacenar la clave de 56 bits luego de permutacion pc1
int izquierdaclave[28];	//buffer para almacenar los primeros 28 bits de la clave luego de permutacion pc1
int derechaclave[28]; //buffer para almacenar los ultimos 28 bits de la clave luego de permutacion pc1
int izquierdadesplazadoclave[28]; //buffer para almacenar los primeros 28 bits luego de desplazamiento circular de 2 bit
int derechadesplazadoclave[28]; //buffer para almacenar los ultimos 28 bits luego de desplazamiento circular de 2 bit
int izquierdayDerechaClaveDesplazamiento2bit[56]; //buffer para unir los dos buffers de los desplazamientos realizados
int bufferclave48permutacionpc2[48]; //buffer para almacenar la clave final
int bufferclave1[48];//buffer para almacenar la clave para descifrar
int bufferclave2[48];//buffer para almacenar la clave para descifrar
int bufferclave3[48];//buffer para almacenar la clave para descifrar
int bufferclave4[48];//buffer para almacenar la clave para descifrar
int bufferclave5[48];//buffer para almacenar la clave para descifrar
int bufferclave6[48];//buffer para almacenar la clave para descifrar
int bufferclave7[48];//buffer para almacenar la clave para descifrar
int bufferclave8[48];//buffer para almacenar la clave para descifrar

char newbuffer[100];
int newlista[8];
int bufferexpxorki[48];//buffer para almacenar la operacion xor
int bufferexpansion[48];//buffer para almacenar la expansion
int E[] = { //buffer con la expansion base
	32,  1,  2,  3,  4,  5,
	 4,  5,  6,  7,  8,  9,
	 8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};
//Matrices base para las S-Box
int cajaS1[4][16] =
{
	14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
	0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
	4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
	15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
};
int cajaS2[4][16] =
{
	15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
	3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
	0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
	13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
};
int cajaS3[4][16] =
{
	10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
	13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
	13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
	1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
};
int cajaS4[4][16] =
{
	7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
	13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
	10,6,9,0,12,11,7,13,13,1,3,14,5,2,8,4,
	3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
};
int cajaS5[4][16] =
{
	2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
	14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
	4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
	11,8,15,7,1,14,2,13,6,15,0,9,10,4,5,3
};
int cajaS6[4][16] =
{
	12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
	10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
	9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
	4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
};
int cajaS7[4][16] =
{
	4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
	13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
	1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
	6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
};
int cajaS8[4][16] =
{
	13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
	1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
	7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
	2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
};

int Sbox1[6];//buffer para almacenar la S-box
int Sbox2[6];//buffer para almacenar la S-box
int Sbox3[6];//buffer para almacenar la S-box
int Sbox4[6];//buffer para almacenar la S-box
int Sbox5[6];//buffer para almacenar la S-box
int Sbox6[6];//buffer para almacenar la S-box
int Sbox7[6];//buffer para almacenar la S-box
int Sbox8[6];//buffer para almacenar la S-box
int auxbufferP[32];//buffer para almacenar los 4 bits de cada Sbox
int bufferP[32];//buffer para realizar la permutacion P

int permutacionP[32] = { //buffer con la matriz bade de la permutacionP
		16,  7, 20, 21,
		29, 12, 28, 17,
		 1, 15, 23, 26,
		 5, 18, 31, 10,
		 2,  8, 24, 14,
		32, 27,  3,  9,
		19, 13, 30,  6,
		22, 11,  4, 25
};

//Matriz de permutacion INICIAL - texto a cifrar
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

//Matriz de permutacion PC1 - clave
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

//Matriz de permutacion PC2 - clave
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

//Matriz de permutacion INICIAL INVERSA - texto a cifrar

int permutacionInicialInversa[64] = {
		40, 8,  48, 16, 56, 24, 64, 32,
		39, 7,  47, 15, 55, 23, 63, 31,
		38, 6,  46, 14, 54, 22, 62, 30,
		37, 5,  45, 13, 53, 21, 61, 29,
		36, 4,  44, 12, 52, 20, 60, 28,
		35, 3,  43, 11, 51, 19, 59, 27,
		34, 2,  42, 10, 50, 18, 58, 26,
		33, 1,  41,  9, 49, 17, 57, 25
};

//Subrutina para leer el archivo de texto y posteriormente convertir los caracteres a binario.
void* Leer(void* palabra1) {
	
	pthread_mutex_lock(&mutex1);	
	std::string* palabra2 = static_cast<std::string*>(palabra1);
	string palabra = *palabra2;
	string buffert[64];
	string nuevo;
	
	
  for (char& _char : palabra) {
        nuevo +=bitset<8>(_char).to_string();
    }
    
    for(int i=0;i<64;i++){
		
		buffert[i] = nuevo.at(i);
		buffer[i] = stoi(buffert[i]);
	}
	pthread_mutex_unlock(&mutex1);
	
	return NULL;

}

//Subrutina para leer la clave y posteriormente convertir los caracteres a binario.
void* Leer2(void* palabra1) {
	
	pthread_mutex_lock(&mutex1);	
	std::string* palabra2 = static_cast<std::string*>(palabra1);
	string palabra = *palabra2;
	string nuevo;
	string buffert[64];
	
			//bufferclave64[i] = stoi(bin);
			
    for (char& _char : palabra) {
        nuevo +=bitset<8>(_char).to_string();
    }
    
    for(int i=0;i<64;i++){
		
		buffert[i] = nuevo.at(i);
		bufferclave64[i] = stoi(buffert[i]);
	}
	pthread_mutex_unlock(&mutex1);


	return NULL;

}

//Subrutina de permutacion P1 al texto a cifrar
void* PermutacionP1(void* palabra1) {
	pthread_mutex_lock(&mutex1);	
	for (int i = 0; i < 64; i++) {
		bufferpermutacion1[i] = buffer[permutacionInicial[i] - 1];
	}

	int k = 32;
	for (int j = 0; j < 32; j++) {
		izquierda[j] = bufferpermutacion1[j];
		derecha[j] = bufferpermutacion1[k];
		k++;
	}
	pthread_mutex_unlock(&mutex1);

	return NULL;

}

//Subrutina de permutacion PC1 a clave de 64 bits para convertirla s 56 bits
void* PermutacionPC1F(void* palabra1) {
	pthread_mutex_lock(&mutex1);

	for (int i = 0; i < 56; i++) {
		bufferclave56permutacionpc1[i] = bufferclave64[permutacionPC1[i] - 1];
	}

	int k = 28;
	for (int j = 0; j < 28; j++) {
		izquierdaclave[j] = bufferclave56permutacionpc1[j];
		derechaclave[j] = bufferclave56permutacionpc1[k];
		k++;
	}
	pthread_mutex_unlock(&mutex1);

	return NULL;

}

//Subrutina de desplazamiento de 2 bits a la izquierda a la clave
void* ClaveDesplazamientoLS(void* palabra1) {

	izquierdadesplazadoclave[26] = izquierdaclave[0];
	izquierdadesplazadoclave[27] = izquierdaclave[1];
	derechadesplazadoclave[26] = derechaclave[0];
	derechadesplazadoclave[27] = derechaclave[1];

	for (int i = 0; i < 26; i++) {
		izquierdadesplazadoclave[i] = izquierdaclave[i + 2];
		derechadesplazadoclave[i] = derechaclave[i + 2];
	}

	for (int k = 0; k < 28; k++) {
		izquierdayDerechaClaveDesplazamiento2bit[k] = izquierdadesplazadoclave[k];
		izquierdayDerechaClaveDesplazamiento2bit[28 + k] = derechadesplazadoclave[k];
	}

	for (int l = 0; l < 28; l++) {
		izquierdaclave[l] = izquierdadesplazadoclave[l];
		derechaclave[l] = derechadesplazadoclave[l];
	}

	return NULL;
}

//Permutacion final para obtener la clave de 48 bits
void* PermutacionPC2F(void* palabra1) {

	for (int i = 0; i < 48; i++) {
		bufferclave48permutacionpc2[i] = izquierdayDerechaClaveDesplazamiento2bit[permutacionPC2[i] - 1];
	}

	return NULL;

}

//funcion para realizar las rondas de izquierda y derecha
void* rondasLiRi(void* palabra1) {

	for (int i = 0; i < 32; i++) {
		izquierdanueva[i] = derecha[i];
		derechanueva[i] = bufferP[i] ^ izquierda[i];
	}

	for (int j = 0; j < 32; j++) {
		derecha[j] = derechanueva[j];
		izquierda[j] = izquierdanueva[j];
	}

	return NULL;

}

//Permutacion inversa final para obtener el texto cifrado
void* PermutacionINVERSA(void* palabra1) {

	for (int i = 0; i < 32; i++) {
		bufferintercambioDI[i] = derecha[i];
		bufferintercambioDI[i + 32] = izquierda[i];
	}

	for (int j = 0; j < 64; j++) {
		bufferfinal[j] = bufferintercambioDI[permutacionInicialInversa[j] - 1];
	}

	return NULL;

}
//Subrutina para realizar expansion de 32 a 48 bits
void* expansion(void* arg) {
	for (int i = 0; i < 48; i++) {
		bufferexpansion[i] = derecha[E[i] - 1];
	}
	return NULL;

}
//Subrutina para convertir un numero de binario a decimal
int binarioadecimal(string binario) {
	int d = 0, e = 0, c = 0;
	int b = atoi(binario.c_str());
	while (b >= 10) {
		c = 0;

		while (b >= 10) {

			b = b - 10;

			c++;

		}

		d = d + b * pow(2, e);

		e++;

		b = c;

	}

	d = d + c * pow(2, e);

	return d;
}
//Subrutina para convertir un numero entero a binario 
bitset<4> enteroABinario(int valor) {
	bitset<4> binario(valor);
	return binario;
}

//Subrutina en la cual se realiza el xor entre la clave y la expansion. Luego se obtienen las S-boxes y se obtienen4 bits de estas para finalizar con la permutacion P
void* Sboxes(void* arg) {
	pthread_mutex_lock(&mutex1);
	for (int i = 0; i < 48; i++) {
		bufferexpxorki[i] = bufferexpansion[i] ^ bufferclave48permutacionpc2[i];
	}
	for (int i = 0; i < 6; i++) {
		Sbox1[i] = bufferexpxorki[i];
	}
	for (int i = 0; i < 6; i++) {
		Sbox2[i] = bufferexpxorki[i + 6];
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

	string m1 = to_string(Sbox1[0]) + to_string(Sbox1[5]);
	string n1 = to_string(Sbox1[1]) + to_string(Sbox1[2]) + to_string(Sbox1[3]) + to_string(Sbox1[4]);

	int m11 = binarioadecimal(m1);
	int n11 = binarioadecimal(n1);

	string m2 = to_string(Sbox2[0]) + to_string(Sbox2[5]);
	string n2 = to_string(Sbox2[1]) + to_string(Sbox2[2]) + to_string(Sbox2[3]) + to_string(Sbox2[4]);

	int m22 = binarioadecimal(m2);
	int n22 = binarioadecimal(n2);

	string m3 = to_string(Sbox3[0]) + to_string(Sbox3[5]);
	string n3 = to_string(Sbox3[1]) + to_string(Sbox3[2]) + to_string(Sbox3[3]) + to_string(Sbox3[4]);

	int m33 = binarioadecimal(m3);
	int n33 = binarioadecimal(n3);

	string m4 = to_string(Sbox4[0]) + to_string(Sbox4[5]);
	string n4 = to_string(Sbox4[1]) + to_string(Sbox4[2]) + to_string(Sbox4[3]) + to_string(Sbox4[4]);

	int m44 = binarioadecimal(m4);
	int n44 = binarioadecimal(n4);

	string m5 = to_string(Sbox5[0]) + to_string(Sbox5[5]);
	string n5 = to_string(Sbox5[1]) + to_string(Sbox5[2]) + to_string(Sbox5[3]) + to_string(Sbox5[4]);

	int m55 = binarioadecimal(m5);
	int n55 = binarioadecimal(n5);

	string m6 = to_string(Sbox6[0]) + to_string(Sbox6[5]);
	string n6 = to_string(Sbox6[1]) + to_string(Sbox6[2]) + to_string(Sbox6[3]) + to_string(Sbox6[4]);

	int m66 = binarioadecimal(m6);
	int n66 = binarioadecimal(n6);

	string m7 = to_string(Sbox7[0]) + to_string(Sbox7[5]);
	string n7 = to_string(Sbox7[1]) + to_string(Sbox7[2]) + to_string(Sbox7[3]) + to_string(Sbox7[4]);

	int m77 = binarioadecimal(m7);
	int n77 = binarioadecimal(n7);

	string m8 = to_string(Sbox8[0]) + to_string(Sbox8[5]);
	string n8 = to_string(Sbox8[1]) + to_string(Sbox8[2]) + to_string(Sbox8[3]) + to_string(Sbox8[4]);

	int m88 = binarioadecimal(m8);
	int n88 = binarioadecimal(n8);

	bitset < 4> a = enteroABinario(cajaS1[m11][n11]);
	bitset < 4> b = enteroABinario(cajaS2[m22][n22]);
	bitset < 4> c = enteroABinario(cajaS3[m33][n33]);
	bitset < 4> d = enteroABinario(cajaS4[m44][n44]);
	bitset < 4> e = enteroABinario(cajaS5[m55][n55]);
	bitset < 4> f = enteroABinario(cajaS6[m66][n66]);
	bitset < 4> g = enteroABinario(cajaS7[m77][n77]);
	bitset < 4> h = enteroABinario(cajaS8[m88][n88]);
	int j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i] = a[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 4] = b[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {

		auxbufferP[i + 8] = c[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 12] = d[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 16] = e[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 20] = f[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 24] = g[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 28] = h[j];
		j--;
	}
	for (int i = 0; i < 32; i++) {
		
	bufferP[i] = auxbufferP[permutacionP[i]-1];

	}
	pthread_mutex_unlock(&mutex1);

	return NULL;

}

//Subrutina en la cual se realiza el xor entre la clave y la expansion. Luego se obtienen las S-boxes y se obtienen4 bits de estas para finalizar con la permutacion P
void* SboxesDescifrado(int arg[48]) {
	for (int i = 0; i < 48; i++) {
		bufferexpxorki[i] = bufferexpansion[i] ^ arg[i];
	}
	for (int i = 0; i < 6; i++) {
		Sbox1[i] = bufferexpxorki[i];
	}
	for (int i = 0; i < 6; i++) {
		Sbox2[i] = bufferexpxorki[i + 6];
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

	string m1 = to_string(Sbox1[0]) + to_string(Sbox1[5]);
	string n1 = to_string(Sbox1[1]) + to_string(Sbox1[2]) + to_string(Sbox1[3]) + to_string(Sbox1[4]);

	int m11 = binarioadecimal(m1);
	int n11 = binarioadecimal(n1);

	string m2 = to_string(Sbox2[0]) + to_string(Sbox2[5]);
	string n2 = to_string(Sbox2[1]) + to_string(Sbox2[2]) + to_string(Sbox2[3]) + to_string(Sbox2[4]);

	int m22 = binarioadecimal(m2);
	int n22 = binarioadecimal(n2);

	string m3 = to_string(Sbox3[0]) + to_string(Sbox3[5]);
	string n3 = to_string(Sbox3[1]) + to_string(Sbox3[2]) + to_string(Sbox3[3]) + to_string(Sbox3[4]);

	int m33 = binarioadecimal(m3);
	int n33 = binarioadecimal(n3);

	string m4 = to_string(Sbox4[0]) + to_string(Sbox4[5]);
	string n4 = to_string(Sbox4[1]) + to_string(Sbox4[2]) + to_string(Sbox4[3]) + to_string(Sbox4[4]);

	int m44 = binarioadecimal(m4);
	int n44 = binarioadecimal(n4);

	string m5 = to_string(Sbox5[0]) + to_string(Sbox5[5]);
	string n5 = to_string(Sbox5[1]) + to_string(Sbox5[2]) + to_string(Sbox5[3]) + to_string(Sbox5[4]);

	int m55 = binarioadecimal(m5);
	int n55 = binarioadecimal(n5);

	string m6 = to_string(Sbox6[0]) + to_string(Sbox6[5]);
	string n6 = to_string(Sbox6[1]) + to_string(Sbox6[2]) + to_string(Sbox6[3]) + to_string(Sbox6[4]);

	int m66 = binarioadecimal(m6);
	int n66 = binarioadecimal(n6);

	string m7 = to_string(Sbox7[0]) + to_string(Sbox7[5]);
	string n7 = to_string(Sbox7[1]) + to_string(Sbox7[2]) + to_string(Sbox7[3]) + to_string(Sbox7[4]);

	int m77 = binarioadecimal(m7);
	int n77 = binarioadecimal(n7);

	string m8 = to_string(Sbox8[0]) + to_string(Sbox8[5]);
	string n8 = to_string(Sbox8[1]) + to_string(Sbox8[2]) + to_string(Sbox8[3]) + to_string(Sbox8[4]);

	int m88 = binarioadecimal(m8);
	int n88 = binarioadecimal(n8);

	bitset < 4> a = enteroABinario(cajaS1[m11][n11]);
	bitset < 4> b = enteroABinario(cajaS2[m22][n22]);
	bitset < 4> c = enteroABinario(cajaS3[m33][n33]);
	bitset < 4> d = enteroABinario(cajaS4[m44][n44]);
	bitset < 4> e = enteroABinario(cajaS5[m55][n55]);
	bitset < 4> f = enteroABinario(cajaS6[m66][n66]);
	bitset < 4> g = enteroABinario(cajaS7[m77][n77]);
	bitset < 4> h = enteroABinario(cajaS8[m88][n88]);
	int j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i] = a[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 4] = b[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {

		auxbufferP[i + 8] = c[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 12] = d[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 16] = e[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 20] = f[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 24] = g[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 28] = h[j];
		j--;
	}
	for (int i = 0; i < 32; i++) {

		bufferP[i] = auxbufferP[permutacionP[i] - 1];

	}

	return NULL;

}

//Subrutina para convertir de binario a ascii
string binarioascii(string binario){
	 
    stringstream sstream(binario);
    string output;
    while(sstream.good())
    {
        bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        output += c;
    }

    return output;
}


int main() {

	pthread_t thread, thread2, thread3[8], thread4; //threads para el texto a cifrar 
	pthread_t threadC1, threadC2, threadC3[8], threadC4[8]; //threads para la clave
	pthread_t threadE1[8], threadE2[8]; //threads de boxes
	string numero;	//string para leer el archivo de texto 
	string texto;	//string donde se almacena el archivo leido de 64 bits
	string clave;	//string para almacenar la clave de 64 bits
	string nueva; 
	string cifrado;
	string descifrado;
	int opcion;
	bool menu = true;
	
	if (pthread_mutex_init(&mutex1, NULL) != 0) 						//inicializacion de mutex no completada
    {
        printf("\n Fallo la inicializacion de Mutex\n");
        return 1;
    }
	
	while (menu) {
		cout << "\nBienvenido al programa de cifrado y descifrado\n Ingrese el numero de opcion que desea: \n1.Cifrar texto \n2.Descifrar texto \n3.Salir\n" << endl;
		cin >> opcion;
		if (opcion == 1) {
			//----------Inicio de leida del archivo de texto--------------
			ifstream palabra("FUENTE.txt", ios::in);

			if (!palabra)
			{
				cerr << "No se pudo leer el archivo" << endl;
				exit(EXIT_FAILURE);
			}

			while (palabra >> numero) {
				texto += numero;
			}

			//----------fin de leida del archivo de texto--------------

			int cantidad = (texto.size() * 8) / 64;


			cout << "Bienvenido introduce la clave a utilizar para cifrar: \n Verificar que sea de 8 caracteres\n";
			cin >> clave;


			for (int j = 1; j <= cantidad; j++) {

				if (j == 1) {

					nueva = texto.substr(0 * j, 8 * j);
				}

				else {

					nueva = texto.substr(8 * (j - 1), 8 * (j));

				}

				pthread_create(&thread, NULL, Leer, (void*)&nueva);
				pthread_join(thread, NULL);

				pthread_create(&thread2, NULL, PermutacionP1, NULL);
				pthread_join(thread2, NULL);


				pthread_create(&threadC1, NULL, Leer2, (void*)&clave);
				pthread_join(threadC1, NULL);

				pthread_create(&threadC2, NULL, PermutacionPC1F, NULL);
				pthread_join(threadC2, NULL);


				for (int i = 0; i < 8; i++) {

					pthread_create(&threadC3[i], NULL, ClaveDesplazamientoLS, NULL);
					pthread_join(threadC3[i], NULL);

					pthread_create(&threadC4[i], NULL, PermutacionPC2F, NULL);
					pthread_join(threadC4[i], NULL);

					pthread_create(&threadE1[i], NULL, expansion, NULL);
					pthread_join(threadE1[i], NULL);

					pthread_create(&threadE2[i], NULL, Sboxes, NULL);
					pthread_join(threadE2[i], NULL);

					pthread_create(&thread3[i], NULL, rondasLiRi, NULL);
					pthread_join(thread3[i], NULL);

				}

				pthread_create(&thread4, NULL, PermutacionINVERSA, NULL);
				pthread_join(thread4, NULL);

				for (int k = 0; k < 64; k++) {
					//cout << bufferfinal[k]<<" ";
					cifrado += to_string(bufferfinal[k]);

				}
			}
			binarioascii(cifrado);
			cout << binarioascii(cifrado)<<"\n";
		
		}
		if (opcion == 2) {
			if (clave == "") {
				printf("Primero debes cifrar el texto");
			}
			else {
				//Leer2(clave);
				for(int i =0;i<64;i++){
					bufferpermutacion1[i] = bufferfinal[i]; 
				}
				
				//PermutacionP1(NULL);
				pthread_create(&thread2, NULL, PermutacionP1, NULL);
				pthread_join(thread2, NULL);

				//PermutacionPC1F(NULL);
				pthread_create(&threadC2, NULL, PermutacionPC1F, NULL);
				pthread_join(threadC2, NULL);
				
				//ClaveDesplazamientoLS(NULL);
				//PermutacionPC2F(NULL);
				pthread_create(&threadC3[0], NULL, ClaveDesplazamientoLS, NULL);
				pthread_join(threadC3[0], NULL);

				pthread_create(&threadC4[0], NULL, PermutacionPC2F, NULL);
				pthread_join(threadC4[0], NULL);
				for (int k = 0; k < 48;k++) {
					bufferclave1[k] = bufferclave48permutacionpc2[k];
				}
				

				//ClaveDesplazamientoLS(NULL);
				//PermutacionPC2F(NULL);
				pthread_create(&threadC3[1], NULL, ClaveDesplazamientoLS, NULL);
				pthread_join(threadC3[1], NULL);

				pthread_create(&threadC4[1], NULL, PermutacionPC2F, NULL);
				pthread_join(threadC4[1], NULL);
				for (int k = 0; k < 48; k++) {
					bufferclave2[k] = bufferclave48permutacionpc2[k];
				}

				//ClaveDesplazamientoLS(NULL);
				//PermutacionPC2F(NULL);
				pthread_create(&threadC3[2], NULL, ClaveDesplazamientoLS, NULL);
				pthread_join(threadC3[2], NULL);

				pthread_create(&threadC4[2], NULL, PermutacionPC2F, NULL);
				pthread_join(threadC4[2], NULL);

				for (int k = 0; k < 48; k++) {
					bufferclave3[k] = bufferclave48permutacionpc2[k];
				}

				//ClaveDesplazamientoLS(NULL);
				//PermutacionPC2F(NULL);
				pthread_create(&threadC3[3], NULL, ClaveDesplazamientoLS, NULL);
				pthread_join(threadC3[3], NULL);

				pthread_create(&threadC4[3], NULL, PermutacionPC2F, NULL);
				pthread_join(threadC4[3], NULL);
				for (int k = 0; k < 48; k++) {
					bufferclave4[k] = bufferclave48permutacionpc2[k];
				}

				//ClaveDesplazamientoLS(NULL);
				//PermutacionPC2F(NULL);
				pthread_create(&threadC3[4], NULL, ClaveDesplazamientoLS, NULL);
				pthread_join(threadC3[4], NULL);

				pthread_create(&threadC4[4], NULL, PermutacionPC2F, NULL);
				pthread_join(threadC4[4], NULL);
				for (int k = 0; k < 48; k++) {
					bufferclave5[k] = bufferclave48permutacionpc2[k];
				}

				//ClaveDesplazamientoLS(NULL);
				//PermutacionPC2F(NULL);
				pthread_create(&threadC3[5], NULL, ClaveDesplazamientoLS, NULL);
				pthread_join(threadC3[5], NULL);

				pthread_create(&threadC4[5], NULL, PermutacionPC2F, NULL);
				pthread_join(threadC4[5], NULL);
				for (int k = 0; k < 48; k++) {
					bufferclave6[k] = bufferclave48permutacionpc2[k];
				}

				//ClaveDesplazamientoLS(NULL);
				//PermutacionPC2F(NULL);
				pthread_create(&threadC3[6], NULL, ClaveDesplazamientoLS, NULL);
				pthread_join(threadC3[6], NULL);

				pthread_create(&threadC4[6], NULL, PermutacionPC2F, NULL);
				pthread_join(threadC4[6], NULL);
				for (int k = 0; k < 48; k++) {
					bufferclave7[k] = bufferclave48permutacionpc2[k];
				}

				//ClaveDesplazamientoLS(NULL);
				//PermutacionPC2F(NULL);
				pthread_create(&threadC3[7], NULL, ClaveDesplazamientoLS, NULL);
				pthread_join(threadC3[7], NULL);

				pthread_create(&threadC4[7], NULL, PermutacionPC2F, NULL);
				pthread_join(threadC4[7], NULL);
				for (int k = 0; k < 48; k++) {
					bufferclave8[k] = bufferclave48permutacionpc2[k];
				}
				//expansion(NULL);
				//
				//rondasLiRi(NULL);
				pthread_create(&threadE1[0], NULL, expansion, NULL);
				pthread_join(threadE1[0], NULL);

				//pthread_create(&threadE2[0], NULL, Sboxes, NULL);
				//pthread_join(threadE2[0], NULL);
				SboxesDescifrado(bufferclave8);

				pthread_create(&thread3[0], NULL, rondasLiRi, NULL);
				pthread_join(thread3[0], NULL);

				//expansion(NULL);
				//
				//rondasLiRi(NULL);
				pthread_create(&threadE1[1], NULL, expansion, NULL);
				pthread_join(threadE1[1], NULL);

				SboxesDescifrado(bufferclave7);

				pthread_create(&thread3[1], NULL, rondasLiRi, NULL);
				pthread_join(thread3[1], NULL);

				//expansion(NULL);
				//
				//rondasLiRi(NULL);
				pthread_create(&threadE1[2], NULL, expansion, NULL);
				pthread_join(threadE1[2], NULL);

				SboxesDescifrado(bufferclave6);

				pthread_create(&thread3[2], NULL, rondasLiRi, NULL);
				pthread_join(thread3[2], NULL);

				//expansion(NULL);
				//
				//rondasLiRi(NULL);
				pthread_create(&threadE1[3], NULL, expansion, NULL);
				pthread_join(threadE1[3], NULL);

				SboxesDescifrado(bufferclave5);

				pthread_create(&thread3[3], NULL, rondasLiRi, NULL);
				pthread_join(thread3[3], NULL);

				//expansion(NULL);
				//
				//rondasLiRi(NULL);
				pthread_create(&threadE1[4], NULL, expansion, NULL);
				pthread_join(threadE1[4], NULL);

				SboxesDescifrado(bufferclave4);

				pthread_create(&thread3[4], NULL, rondasLiRi, NULL);
				pthread_join(thread3[4], NULL);

				//expansion(NULL);
				//
				//rondasLiRi(NULL);
				pthread_create(&threadE1[5], NULL, expansion, NULL);
				pthread_join(threadE1[5], NULL);

				SboxesDescifrado(bufferclave3);

				pthread_create(&thread3[5], NULL, rondasLiRi, NULL);
				pthread_join(thread3[5], NULL);

				//expansion(NULL);
				//
				//rondasLiRi(NULL);
				pthread_create(&threadE1[6], NULL, expansion, NULL);
				pthread_join(threadE1[6], NULL);

				SboxesDescifrado(bufferclave2);

				pthread_create(&thread3[6], NULL, rondasLiRi, NULL);
				pthread_join(thread3[6], NULL);

				//expansion(NULL);
				
				//rondasLiRi(NULL);
				pthread_create(&threadE1[7], NULL, expansion, NULL);
				pthread_join(threadE1[7], NULL);

				SboxesDescifrado(bufferclave1);

				pthread_create(&thread3[7], NULL, rondasLiRi, NULL);
				pthread_join(thread3[7], NULL);

				//PermutacionINVERSA(NULL);
				pthread_create(&thread4, NULL, PermutacionINVERSA, NULL);
				pthread_join(thread4, NULL);

				for (int k = 0; k < 64; k++) {
					descifrado += to_string(bufferfinal[k]);

				}
				
				cout << binarioascii(descifrado) << "\n";
				
				pthread_mutex_destroy(&mutex1);


				


			}
			
		}
		if (opcion == 3) {
			menu = false;
			printf("Gracias por utilizar el programa");
		
		}

	
	}
		

}
