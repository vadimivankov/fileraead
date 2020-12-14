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
1) 12 (0...11) = Имя объекта (char)
2) 2 (12...13) = Индекс (unsigned int)
3) 1 (14) = Ф - формат объекта (unsigned int)
4) 1 (15) = Т - тип объекта (unsigned int)
5) 4 (16...19) = А0 - масштабный коэффициент (float)
6) 4 (20...23) = А1 - масштабный коэффициент (float)
7) 8 (24...31) = Размерность параметра (char)

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

#include <iostream>
#include <cstdio>
#include <cstring>
#include <windows.h>


using namespace std;


string file_data_str(const char * filename, int position, int bytes)
{
    char * data_ch[10];
    memset(data_ch, 0, 10);

    string data_str = "";

    FILE * F = fopen(filename, "rb");
    
    fseek(F, position, 0);
    fread(data_ch, bytes, 1, F);
    
    data_str = (const char*)data_ch;


    fclose(F);
    return data_str;
}


float file_data_flt(const char * filename, int position, int bytes)
{
    float data_flt = .0;

    FILE * F = fopen(filename, "rb");
    
    fseek(F, position, 0);
    fread(&data_flt, bytes, 1, F);

    fclose(F);
    return data_flt;
}


unsigned int file_data_int(const char * filename, int position, int bytes)
{
    int data_int = 0;

    FILE * F = fopen(filename, "rb");
    
    fseek(F, position, 0);
    fread(&data_int, bytes, 1, F);
    
    fclose(F);
    return data_int;
}


void data_passport(const char * filename, int size_of_passport, int passport_position, int amount_passports)
{
    //printf("Passport position:\t%d\n", passport_position);
    //printf("Amount of passports:\t%d\n", amount_passports);
    cout << "\tName of subject:\t" << file_data_str(filename, passport_position, 12) << "\n";
    cout << "\tIndex:\t\t\t" << file_data_int(filename, passport_position + 12, 2) << "\n";
    cout << "\tSubject format:\t\t" << file_data_int(filename, passport_position + 14, 1) << "\n";
    cout << "\tType of subject:\t" << file_data_int(filename, passport_position + 15, 1) << "\n";
    cout << "\tA0:\t\t\t" << file_data_flt(filename, passport_position + 16, 4) << "\n";
    cout << "\tA1:\t\t\t" << file_data_flt(filename, passport_position + 20, 4) << "\n";
    cout << "\tAmount of parameter:\t" << file_data_str(filename, passport_position + 24, 8) << "\n\n\n";
}



int main()
{
    SetConsoleOutputCP(866);
//    SetConsoleCP(866);

    const char * filename = "1.us0";

    FILE * F = fopen (filename, "rb"); //проверка файла
    if (F == NULL) 
    {
        cout << "Unable to open file " << filename << endl;
        return 1;
    }
    fclose (F);

    cout << "YC0 :\t\t\t" << file_data_str(filename, 0, 4) << "\n"; // "УС0 "
    cout << "Name of product:\t" << file_data_str(filename, 4, 8) << "\n"; // "Имя изделия"
    cout << "Name of test:\t\t" << file_data_str(filename, 12, 8) << "\n"; // "Имя испытания"
    cout << "Date of test:\t\t" << file_data_str(filename, 20, 8) << "\n"; // "Дата испытания"
    cout << "Initial test time:\t" << file_data_flt(filename, 28, 4) << "\n";  // ТН
    cout << "End test time:\t\t" << file_data_flt(filename, 32, 4) << "\n";     // ТК
    cout << "Amount of passports:\t" << file_data_int(filename, 36, 2) << "\n"; // "Кол-во паспортов"
    cout << "Kind of file:\t\t" << file_data_int(filename, 38, 1) << "\n"; // "Разновидность файла"
    cout << "Comment length:\t\t" << file_data_int(filename, 39, 1) << "\n"; // "Длина комментария"
    cout << "\n";

// *************** PASSPORTS ***************

    int size_of_passport = 32;
    int passport_position = 40 + file_data_int(filename, 39, 1);
    
    int amount_passports = file_data_int(filename, 36, 2);

    //for (int i = 0; i < 5; i++)   // test of mechanic
    for (int i = 0; i < amount_passports; i++)
    {
        printf("Data of %d  passport:\n", i + 1);
        data_passport(filename, size_of_passport, passport_position, amount_passports);
        passport_position += size_of_passport;
    }


    return 0;
}
