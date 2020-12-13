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


int main()
{
    SetConsoleOutputCP(1251);
//    SetConsoleCP(1251);

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

/*    char s[10] = "";

    fread (s , 4 , 1 , F);
    cout << "YC0 :\t\t\t" << s << endl;

    fread (s , 8 , 1 , F);
    cout << "Name of product:\t" << s << endl;

    fread (s , 8 , 1 , F);
    cout << "Name of test:\t\t" << s << endl;

    fread (s , 8 , 1 , F);
    cout << "Date of test:\t\t" << s << endl;
    


    float f = .0;

    fread (&f , 4 , 1 , F);
    cout << "Initial test time:\t" << f << endl;
    f = .0;

    fread (&f , 4 , 1 , F);
    cout << "End test time:\t\t" << f << endl;
    f = .0;



    unsigned int ui = 0;

    fread (&ui , 2 , 1 , F);
    cout << "Amount of passports:\t" <<  ui << endl;
    ui = 0;

    fread (&ui , 1 , 1 , F);
    cout << "Kind of file:\t\t" <<  ui << endl;
    ui = 0;

    fread (&ui , 1 , 1 , F);
    cout << "Comment length:\t\t" << ui << endl;
    ui = 0;

    fclose (F);

*/

    return 0;
}

