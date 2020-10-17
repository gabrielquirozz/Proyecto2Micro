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

//variables de almacenamiento de texto a cifrar
string buffer[64];	//buffer inicial para almacenar el texto en binario
int bufferpermutacion1[64]; //buffer para almacenar luego de la permutacion inicial
int izquierda[32];	//buffer para almacenar los primeros 32 bits luego de la permutacion inicial
int derecha[32]; //buffer para almacenar los ultimos 32 bits luego de la permutacion inicial
int izquierdanueva[32] //buffer utilizado para las 8 rondas para almacenar la izquierda nueva
int derechanueva[32] //buffer utilizado para las 8 rondas para almacenar la derecha nueva
int bufferintercambioDI[64] // buffer utilizado para intercambiar y almacenar la derecha e izquierda
int bufferfinal[64] //buffer utilizado para almacenar el texto cifrado luego de permutacion inversa

//variables de almacenamiento de clave
string bufferclave64[64];	//buffer inicial para almacenar la clave en binario
int bufferclave56permutacionpc1[56]; //buffer para almacenar la clave de 56 bits luego de permutacion pc1
int izquierdaclave[28];	//buffer para almacenar los primeros 28 bits de la clave luego de permutacion pc1
int derechaclave[28]; //buffer para almacenar los ultimos 28 bits de la clave luego de permutacion pc1
int izquierdadesplazadoclave[28]; //buffer para almacenar los primeros 28 bits luego de desplazamiento circular de 2 bit
int derechadesplazadoclave[28]; //buffer para almacenar los ultimos 28 bits luego de desplazamiento circular de 2 bit
int izquierdayDerechaClaveDesplazamiento2bit[56] //buffer para unir los dos buffers de los desplazamientos realizados
int bufferclave48permutacionpc2[48]; //buffer para almacenar la clave final

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

int Sbox1[6];
int Sbox2[6];
int Sbox3[6];
int Sbox4[6];
int Sbox5[6];
int Sbox6[6];
int Sbox7[6];
int Sbox8[6];
int auxbufferP[32];
int bufferP[32];

int permutacionP[32] = {
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

	std::string* palabra2 = static_cast<std::string*>(palabra1);
	string palabra = *palabra2;
	int n = palabra.length();

	if (palabra.length() % 8 != 0) {
		int contador = palabra.length() % 8;
		for (contador; contador < 8; contador++) {
			palabra += " ";
		}
	}
	for (int i = 0; i <= n; i++) {
		int val = int(palabra[i]);
		string bin = "";
		while (val > 0) {
			(val % 2) ? bin.push_back('1') :
				bin.push_back('0');
			val /= 2;
		}
		reverse(bin.begin(), bin.end());
		if (i < n) {
			buffer[i] = bin;
		}
	}

	return NULL;

}

//Subrutina para leer la clave y posteriormente convertir los caracteres a binario.
void* Leer2(void* palabra1) {

	std::string* palabra2 = static_cast<std::string*>(palabra1);
	string palabra = *palabra2;
	int n = palabra.length();

	if (palabra.length() % 8 != 0) {
		int contador = palabra.length() % 8;
		for (contador; contador < 8; contador++) {
			palabra += " ";
		}
	}
	for (int i = 0; i <= n; i++) {
		int val = int(palabra[i]);
		string bin = "";
		while (val > 0) {
			(val % 2) ? bin.push_back('1') :
				bin.push_back('0');
			val /= 2;
		}
		reverse(bin.begin(), bin.end());
		if (i < n) {
			bufferclave64[i] = bin;
		}
	}

	return NULL;

}

//Subrutina de permutacion P1 al texto a cifrar
void* PermutacionP1(NULL) {
	for (int i = 0; i < 64; i++) {
		bufferpermutacion1[i] = stoi(buffer[permutacionInicial[i] - 1]);
	}

	int k = 32;
	for (int j = 0; j < 32; j++) {
		izquierda[j] = bufferpermutacion1[j];
		derecha[j] = bufferpermutacion1[k];
		k++;
	}

	return NULL;

}

//Subrutina de permutacion PC1 a clave de 64 bits para convertirla s 56 bits
void* PermutacionPC1F(NULL) {

	for (int i = 0; i < 56; i++) {
		bufferclave56permutacionpc1[i] = stoi(bufferclave64[permutacionPC1[i] - 1]);
	}

	int k = 28;
	for (int j = 0; j < 28; j++) {
		izquierdaclave[j] = bufferclave56permutacionpc1[j];
		derechaclave[j] = bufferclave56permutacionpc1[k];
		k++;
	}

	return NULL;

}

//Subrutina de desplazamiento de 2 bits a la izquierda a la clave
void* ClaveDesplazamientoLS(NULL) {

	izquierdadesplazadoclave[26] = izquierdaclave[0];
	izquierdadesplazadoclave[27] = izquierdaclave[1];
	derechadesplazadoclave[26] = derechaclave[0];
	derechadesplazadoclave[27] = derechaclave[1];

	for (int i = 0; i < 26; i++) {
		izquierdadesplazadoclave[i] = izquierdaclave[i + 2];
		derechadesplazadoclave[i] = derechaclave[i + 2]
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
void* PermutacionPC2F(NULL) {

	for (int i = 0; i < 48; i++) {
		bufferclave48permutacionpc2[i] = izquierdayDerechaClaveDesplazamiento2bit[permutacionPC2[i] - 1]);
	}

	return NULL;

}

//funcion para realizar las rondas de izquierda y derecha
void* rondasLiRi(NULL) {

	for (int i = 0; i < 32; i++) {
		izquierdanueva[i] = derecha[i];
		derechanueva[j] = auxbufferP[i] ^ izquierda[i];
	}

	for (int j = 0; j < 32; j++) {
		derecha[i] = derechanueva[i];
		izquierda[i] = izquierdanueva[i];
	}

	return NULL;

}

//Permutacion inversa final para obtener el texto cifrado
void* PermutacionINVERSA(NULL) {

	for (int i = 0; i < 32; i++) {
		bufferintercambioDI[i] = derecha[i];
		bufferintercambioDI[i + 32] = izquierda[i];
	}

	for (int j = 0; j < 64; j++) {
		bufferfinal[i] = bufferintercambioDI[permutacionInicialInversa[i] - 1]
	}

	return NULL;

}

void* expansion(void* arg) {
	for (int i = 0; i < 48; i++) {
		bufferexpansion[i] = E[i] - 1;
	}
}

void* Sboxes(void* arg) {
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

	bitset < 4> 01 = enteroABinario(cajaS1[m11][n11]);
	bitset < 4> 02 = enteroABinario(cajaS2[m22][n22]);
	bitset < 4> 03 = enteroABinario(cajaS3[m33][n33]);
	bitset < 4> 04 = enteroABinario(cajaS4[m44][n44]);
	bitset < 4> 05 = enteroABinario(cajaS5[m55][n55]);
	bitset < 4> 06 = enteroABinario(cajaS6[m66][n66]);
	bitset < 4> 07 = enteroABinario(cajaS7[m77][n77]);
	bitset < 4> 08 = enteroABinario(cajaS8[m88][n88]);
	int j = 3;
	for (int i = 0; i < 4; i++) {
		auxbufferP[i + 4] = 01[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		int j = 3;
		auxbufferP[i + 8] = 02[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		int j = 3;
		auxbufferP[i + 12] = 03[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		int j = 3;
		auxbufferP[i + 16] = 04[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		int j = 3;
		auxbufferP[i + 20] = 05[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		int j = 3;
		auxbufferP[i + 24] = 06[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		int j = 3;
		auxbufferP[i + 28] = 07[j];
		j--;
	}
	j = 3;
	for (int i = 0; i < 4; i++) {
		int j = 3;
		auxbufferP[i + 32] = 08[j];
		j--;
	}
	for (int i = 0; i < 32; i++) {
		auxbufferP[i] = permutacionP[i]-1

	}





}

int binarioadecimal(string binario) {
	int d = 0, e = 0, c = 0;
	int b = stoi(binario);
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

bitset<4> enteroABinario(int valor) {
	bitset<4> binario(valor);
	return binario;
}


int main() {

	pthread_t thread, thread2, thread3, thread4; //threads para el texto a cifrar 
	pthread_t threadC1, threadC2, threadC3 threadC4; //threads para la clave
	string numero;	//string para leer el archivo de texto 
	string texto;	//string donde se almacena el archivo leido de 64 bits
	string clave;	//string para almacenar la clave de 64 bits

//----------Inicio de leida del archivo de texto--------------
	ifstream palabra("FUENTE.txt", ios::in);

	if (!palabra)
	{
		cerr << "No se pudo leer el archivo" << endl;
		exit(EXIT_FAILURE);
	}

	while (palabra >> numero) {
		texto += " " + numero;
	}
	//----------fin de leida del archivo de texto--------------

	pthread_create(&thread, NULL, Leer, (void*)&texto);
	pthread_join(thread, NULL);

	pthread_create(&thread2, NULL, Permutacion1, NULL);
	pthread_join(thread2, NULL)

		cout << "Bienvenido introduce la clave a utilizar para cifrar: ";
	cin >> clave;

	pthread_create(&threadC1, NULL, Leer, (void*)&clave);
	pthread_join(threadC1, NULL);

	pthread_create(&threadC2, NULL, PermutacionPC1F, NULL);
	pthread_join(threadC2, NULL);

	pthread_create(&threadC3, NULL, ClaveDesplazamientoLS, NULL);
	pthread_join(threadC3, NULL);

	pthread_create(&threadC4, NULL, PermutacionPC2F, NULL);
	pthread_join(threadC4, NULL);

	pthread_create(&thread3, NULL, Permutacion1, NULL);
	pthread_join(thread3, NULL)

		pthread_create(&thread4, NULL, PermutacionINVERSA, NULL);
	pthread_join(thread4, NULL)

}