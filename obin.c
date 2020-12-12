/*
Заголовок:
1) 40-295 = Заголовок файла
2) 32*КП = Таблица паспортов
3) *без_ограничений = Информация

Структура заголовка:
1) 4 (0...3) = "УС0 " (string)
2) 8 (4...11) = Имя изделия (string)
3) 8 (12...19) = Имя испытания (string)
4) 8 (20...27) = Дата испытания (string)

5) 4 (28...31) = ТН - начальное время испытания (float)
6) 4 (32...35) = ТК - конечное время испытания (float)

7) 2 (36...37) = КП - кол-во паспортов (unsigned int)
8) 1 (38) = РФ - разновидность файла (unsigned int)
9) 1 (39) = L - длина комментария, если L=0, то комментарий отсутствует (unsigned int)
10) L = Комментарий (В комментарии первые 4 байта в формате числа с плавающей точкой 
занимает временная поправка (dt), по которой производится пересчет  текущегое времени (t) 
следующим образом: t = t_us0 – dt; где t_us0 – текущее время во входном файле УС0.)

Структура паспорта:
1) 12 = Имя объекта (char)
2) 2 = Индекс (unsigned int)
3) 1 = Ф - формат объекта (unsigned int)
4) 1 = Т - тип объекта (unsigned int)
5) 4 = А0 - масштабный коэффициент (float)
6) 4 = А1 - масштабный коэффициент (float)
7) 8 = Размерность параметра (char)

Форматы объектов следующие:
0 - массив (кадр, строка) неопределенной длины,
1 - длина 1 байт,
2 - длина 2 байта,
4 - длина 4 байта,
8 - длина 8 байт.

Типы объектов следующие:
0 - время в формате числа с плавающей точкой,
6 - время в формате беззнакового целого,
1 - измеряемый параметр в кодовом виде,
2 - измеряемый параметр в виде масштабированного целого без знака,
3 - измеряемый параметр в виде числа с плавающей точкой,
4 - структура измерительной системы,
5 - элемент структуры.

*/

#include <stdio.h>
#include <iostream>
#include <fstream>


using namespace std;


int open_file(string filename) // проверим наличие файла
{
	ifstream file;

	int open_file_is_true7 = 0;
	
	file.open(filename, ios::binary);

	if (file.is_open() == true)
	{
		cout << "File " << filename << " opened!" << endl;
		cout << "\n";
		open_file_is_true7 = 1;
	}
	else
	{
		cout << "WARNING! File has not opened!" << endl;
	}
	file.close();

	return open_file_is_true7;
}


string file_data_str(string filename, int position, int bytes) // принять имя файла, позицию и количество считываемых байт
{
	char ch;
	string data_str = ""; // возвращаемая функцией строка

	ifstream file;
	file.open(filename, ios::binary); //открыть файл
	file.seekg(position); //перейти на нужную позицию в файле

	for (int i = 0; i < bytes; i++) //запускаем счетчик байт для считывания из файла
		{
			file.get(ch);
			data_str += ch; // составить строку из считанных байт
		}
	file.close();

	return data_str;
}


float file_data_float(string filename, int position, int bytes) // принять имя файла, позицию и количество считываемых байт
{
	char ch;
	std::string data_str = ""; // возвращаемая функцией строка

	ifstream file;
	file.open(filename, ios::binary); //открыть файл
	file.seekg(position); //перейти на нужную позицию в файле

	for (int i = 0; i < bytes; i++) //запускаем счетчик байт для считывания из файла
		{
			file.get(ch);
			data_str += ch; // составить строку из считанных байт
			cout << int(ch) << " - " << data_str << endl;

		}
	
	
//	float data_float = std::stof(data_str);

//	cout << data_float << endl;

	float data_float;


	file.close();

	return data_float;
}




string file_data_int(string filename, int position, int bytes) // принять имя файла, позицию и количество считываемых байт
{
	char ch;
	string data_str = ""; // возвращаемая функцией строка
	int data_int;

	ifstream file;
	file.open(filename, ios::binary); //открыть файл
	file.seekg(position); //перейти на нужную позицию в файле

	for (int i = 0; i < bytes; i++) //запускаем счетчик байт для считывания из файла
		{
			file.get(ch);
			data_str += &ch; // составить строку из считанных байт
		}		
	file.close();

	//data_str = stoi(data_str);

	return data_str;
}










int main(int argc, const char** argv)
{
	int amount; 	//количество считываемых байт из файла
	int counter; 	//счетчик считанных байт
	char ch;

	string filename = "1.us0"; //текущее название кириллицей программа не может прочесть
	
	if (open_file(filename) == 1) // проверить наличие файла
	{
		cout << "YC0 :\t\t\t" << file_data_str(filename, 0, 4) << "\n"; // "УС0 "
		cout << "Name of product:\t" << file_data_str(filename, 4, 8) << "\n"; // "Имя изделия"
		cout << "Name of test:\t\t" << file_data_str(filename, 12, 8) << "\n"; // "Имя испытания"
		cout << "Date of test:\t\t" << file_data_str(filename, 20, 8) << "\n"; // "Дата испытания"
		

		cout << "Amount of passports:\t" << file_data_int(filename, 36, 2) << "\n"; // "Кол-во паспортов"
		cout << "Kind of file:\t\t" << file_data_int(filename, 38, 1) << "\n"; // "Разновидность файла"

		cout << "\n";

	}

	system("pause");
	return 0;
}
