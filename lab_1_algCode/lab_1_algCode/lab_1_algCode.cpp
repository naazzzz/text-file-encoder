#include <iostream>
#include <fstream>
#include <stdio.h>   
#include <stdlib.h>   
#include <string.h> 
#include <vector>
#include <string> 
#include <Windows.h> 

using namespace std;

fstream fileCode;
fstream file;

int low = 0;
int range = 100000;
int code = 0;
int range_dec = 1;
int n = 0;
char ch;
int start = 0;
int Size=0;
int total = 10;
char rh[5];
int mas[5][2];
bool flag = false;
bool flag1 = false;
int u = 0;

void lz77() {
	fileCode.open("mumuCodelz.txt");
	file.open("mumu.txt");

	if (!file.is_open() && !fileCode.is_open()) {
		cout << "ERROR";
		exit(0);
	}

    struct code {
        int off;
        int len;
        char nextChar;
    };

#define maxWindow 9
    vector<code> vt;

	while (!file.eof()) {

		string str;
		char c;
		while (!file.eof()) {

			c = file.get();
			if (c == -1)break;
			str = str + c;

		}

		//cout << str << " - Кодируем" << endl;
		int window1, window2;

		//Код 
		for (int i = 0; i < str.length(); i++) {
			if (i + 1 <= maxWindow) window1 = i; // Определяем размер окна запроса 
			else window1 = maxWindow;

			if (i + window1 < str.length()) window2 = window1;
			else window2 = str.length() - i;


			string str1 = str.substr(i - window1, window1);
			string str2 = str.substr(i, window2);


			int off = -1;
			while (true) {
				if (window2 == 0) break; // Пустая строка, прямой выход 
				string str3 = str2.substr(0, window2);
				off = str1.find(str3); // Не найдено, выкл. = -1 

				if (off != -1) break; // Нашли, выходим 
				window2--;
				if (window2 <= 0) break;
			}

			if (off != -1) {
				code cd;
				cd.len = window2;
				cd.off = window1 - off;
				cd.nextChar = str[i + window2];
				vt.push_back(cd);
				i += window2;
			}
			else {
				code cd;
				cd.len = 0;
				cd.off = 0;
				cd.nextChar = str[i + window2];
				vt.push_back(cd);
			}
		}
	}
	
	
		for (int i = 0; i < vt.size(); i++) { // Результат кодирования 
			fileCode << vt[i].off << vt[i].len << vt[i].nextChar;
			//cout << vt[i].off << vt[i].len << vt[i].nextChar;
		}
		//cout << " - Закодировали" << endl;
		printf("\n");
		
	fileCode.close();
	file.close();
}

void lz77_decode() {

	struct code {
		int off;
		int len;
		char nextChar;
	};

	vector<code> vt;

	fileCode.open("mumuDecode.txt");

	char c;

	while (!fileCode.eof()) {
		code cd;

		c = fileCode.get();
		if (c == -1)break;
		cd.off = (int)c-48;
		
		c = fileCode.get();
		if (c == -1)break;
		cd.len = (int)c-48;
		
		c = fileCode.get();
		if (c == -1)break;
		cd.nextChar = c;
		

		vt.push_back(cd);

		if (c == -1)break;
	}

	string strOut;
	for (int i = 0; i < vt.size(); i++) {
		if (vt[i].len == 0) {
			strOut += vt[i].nextChar;
		}
		else {
			int len = strOut.length();
			len -= vt[i].off;
			string temp = strOut.substr(len, vt[i].len);
			strOut += temp + vt[i].nextChar;
		}
	}
	
	fileCode.close();
	file.open("mumuDecode.txt", std::ofstream::out | std::ofstream::trunc);
	file << strOut;
	file.close();
}

/// /////////////////////////////////////////////////////

void EmitDigit()
{
	//cout << (low / 10000);
	fileCode << (low / 10000);
		low = (low % 10000) * 10;
		range *= 10;
	
}

void Encode(int start, int size, int total)
{
	
	range /= total;
	low += start * range;
	range *= size; 
	if (low != 10720) {
		while (low / 10000 == (low + range) / 10000)
			EmitDigit(); 
	}
	if (range < 1000)
	{
		EmitDigit();
		EmitDigit();
		range = 100000 - low;
	}
}

void create_Coder() {
	char rep1;
	bool exit1 = false;
	bool a1 = false;
	int a[5][2];
	fileCode.open("mumuCodeinter.txt");
	file.open("mumuCodelz.txt");
	while (!file.eof()) {
		for (int i = 0; i < 5; i++)
		{
			a[i][1] = 0;
			a[i][0] = 0;
		}
		 low = 0;  range = 100000;
		string str;
		char c;
		for(int i=0;i<5;i++) {

			c = file.get();
			if (file.eof()) {
				exit1 = true;
				break;
			}
			
			str = str + c;

		}
		
		
		while (str.size() != 5) {
			str += "$";
		}

		//cout << str << endl;
		//заполнение size
		int k = 1;
		int num = 1;
		int sum = 0;
		for (int i = 0; i < 5; i++)
		{
			while (k != 5) {
				if (str[i] == str[k]) {
					num++;
				}
				k++;
			}
			k = i - 1;
			while (k != -1)
			{
				if (str[i] == str[k]) {
					num++;
				}
				k--;
			}
			k = i + 2;

				a[i][1] = num * 2;
				num = 1;
		}
		//заполнение start
		int r = -1;
		int max = 0;
		 k = 0;
		bool a2 = false;
		
		for (int i = 0; i < 5; i++) {
			if (r == -1) {
				sum = 0; r++;
			}
			else {
				while (r != -1) {
					if (str[i] == str[r]) {
						sum = a[r][0];
						a2 = true;
						r++;
						break;
					}
					r--;

				}
				if (r == -1) {
					a2 = false; 
				}
				r = i-1;
				if (a2 == false) {
					while (r != -1) {
						if (max <= a[r][0]) {
							max = a[r][0] + a[r][1];
							break;
						}
						r--;
					}
					sum = max;
				}
			}
			a[i][0] = sum;
			r = i;
		}
		for (int i = 0; i < 5; i++) {
			Encode(a[i][0], a[i][1], 10);
		}
		
		while (range < 10000)
			EmitDigit();

		low += 10000;
		EmitDigit();
		//cout << endl;

		fileCode << ":";
		a2 = false;
		for (int i = 0; i < 5; i++)
		{
			r = i-1;
			while (r != -1) {
				if (str[i]==str[r]) {
					a2 = true;
				}
				r--;
			}
			if (a2 == false) {
				fileCode << str[i] << a[i][0] << a[i][1];
			}
			a2 = false;
		}
		fileCode << "|";

	}

	//посмотреть эту часть кода
	file.close();
	fileCode.close();
}


////////////////////////////////////////////////////////

int ReadNextDigit() {
	
	if (flag == true) {
		ch = 'l';
		flag = false;
		}
		if (ch == ':' || ch== '|') {
			return NULL;
		}
		else {
			ch = file.get();
			//if (ch == -1)exit(0);
			if (ch == ':')return NULL;
		}
	//cout << ch;
	return ch-48;
}

void AppendDigit()
{
	code = (code % 10000) * 10 + ReadNextDigit();
	low = (low % 10000) * 10;
	range_dec *= 10;
}

void InitializeDecoder() {
	for (int i = 0; i < 5; i++)
	{
		AppendDigit();
	}
} 

void Decode(int start, int size, int total)
{ 
	
	range_dec /= total;
	low += start * range_dec;
	range_dec *= size; 
	while (low / 10000 == (low + range_dec) / 10000) {
		AppendDigit(); 
	}

	if (range_dec < 1000)
	{
		AppendDigit();
		AppendDigit();
		range_dec = 100000 - low;
} 
}

int GetValue(int total)
{
	return (code - low) / (range_dec / total);
}

void init() {
	for (int i = 0; i < 6; i++)
	{	
		ch = file.get();
		if (ch != '|') {
			rh[i] = ch;
			mas[i][0] = file.get() - 48;
			mas[i][1] = file.get() - 48;
			if (mas[i][1] == 1) {
				mas[i][1] = 10;
				ch = file.get();
			}
		}
		else break;
	}
}

void Ver(int v) {
	
	
	for (int i = 0; i < 5; i++) {
		if (mas[i][0]<=v && (mas[i][1]+mas[i][0]-1) >= v)
		{
			if (rh[i] != '$') {
				fileCode << rh[i];
			//	cout << "Выводим " << rh[i] << endl;
			}
			Decode(mas[i][0], mas[i][1], total);
		}
	}
	
}

void Run_inter_decoder() {
	code = 0; low = 0; range_dec = 1;
	file.open("mumuCodeinter.txt");
	fileCode.open("mumuDecode.txt");
	while (!file.eof()) {
		InitializeDecoder();
		//cout << endl;
		
		init();

		for (int i = 0; i < 5; i++)
		{
			int v = GetValue(total);
			Ver(v);
		}
		flag = true;
		range_dec = 1;
		low = 0;
		code = 0;
		for (int i = 0; i < 5; i++)
		{
			mas[i][0] = 0;
				mas[i][1] = 0;
				rh[i] = 'e';
		}
	}
	file.close();
	fileCode.close();
} 

 ////////////////////////////////////////////////////////

int main()
{
	SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
	SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода
	setlocale(LC_ALL, "Rus"); // Подключение русского языка
	
	while (true) {
		cout << "Выберете действие, которое хотите совершить(1-4)" << endl;
		cout << "(1)кодирование" << endl;
		cout << "(2)декодирование" << endl;
		cout << "Любая другая - выход " << endl;
		int a = 0;
		cin >> a;
		switch (a)
		{
		case 1: {
			lz77();
			create_Coder();
			break;
		}
		case 2:{
			Run_inter_decoder();
			lz77_decode();
			break;
		}
		default:
			exit(0);
	}
	}
    return 0;
}
