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
#include <vector>

using namespace std;


int file_opening(const char * filename) // проверка на наличие файла
{
    int ofile = 0;

    FILE * F = fopen (filename, "rb");
    if (F == NULL) 
    {
        cout << "Unable to open file ---> " << filename << endl;
        ofile = 1;
    }
    fclose (F);
    return ofile;
}


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


struct list_heading
{
    string yc0;
    string name_of_product;
    string name_of_test;
    string date_of_test;
    float initial_test_time;
    float end_test_time;
    int amount_passports;
    int kind_of_file;
    int comment_lenght;

};
typedef struct list_heading Heading;


void read_heading(const char * filename, int head[], Heading * h)
{
    int pos[9] = {0}; //положение параметра в заголовке
    
    for (int n = 1; n < 9; n++) // счетчик
        pos[n] = pos[n-1] + head[n-1];

    h->yc0 = file_data_str(filename, pos[0], head[0]);
    h->name_of_product = file_data_str(filename, pos[1], head[1]);
    h->name_of_test = file_data_str(filename, pos[2], head[2]);
    h->date_of_test = file_data_str(filename, pos[3], head[3]);
    h->initial_test_time = file_data_flt(filename, pos[4], head[4]);
    h->end_test_time = file_data_flt(filename, pos[5], head[5]);
    h->amount_passports = file_data_int(filename, pos[6], head[6]);
    h->kind_of_file = file_data_int(filename, pos[7], head[7]);
    h->comment_lenght = file_data_int(filename, pos[8], head[8]);
}


void print_heading(Heading h)
{
    cout << "YC0 :\t\t\t" << h.yc0 << "\n";
    cout << "Name of product:\t" << h.name_of_product << "\n";
    cout << "Name of test:\t\t" << h.name_of_test << "\n";
    cout << "Date of test:\t\t" << h.date_of_test << "\n";
    cout << "Initial test time:\t" << h.initial_test_time << "\n";
    cout << "End test time:\t\t" << h.end_test_time << "\n";
    cout << "Amount of passports:\t" << h.amount_passports << "\n";
    cout << "Kind of file:\t\t" << h.kind_of_file << "\n";
    cout << "Comment length:\t\t" << h.comment_lenght << "\n";
    cout << "\n";
}


struct list_passport
{
    string name_of_subject;
    int index;
    int subject_format;
    int type_of_subject;
    float A0;
    float A1;
    string amount_of_parameter;
};
typedef struct list_passport Passport;


void read_passport(const char * filename, int passport_position, int head[], Passport * p)
{
    int pos[7] = {0}; //положение параметра в заголовке
    
    for (int n = 1; n < 7; n++) // счетчик
        pos[n] = pos[n-1] + head[n-1];

    p->name_of_subject = file_data_str(filename, passport_position + pos[0], head[0]);
    p->index = file_data_int(filename, passport_position + pos[1], head[1]);
    p->subject_format = file_data_int(filename, passport_position + pos[2], head[2]);
    p->type_of_subject = file_data_int(filename, passport_position + pos[3], head[3]);
    p->A0 = file_data_flt(filename, passport_position + pos[4], head[4]);
    p->A1 = file_data_flt(filename, passport_position + pos[5], head[5]);
    p->amount_of_parameter = file_data_str(filename, passport_position + pos[6], head[6]);
}

void print_passport(Passport p)
{
    
    cout << "\tName of subject:\t" << p.name_of_subject << "\n";
    cout << "\tIndex:\t\t\t" << p.index << "\n";
    cout << "\tSubject format:\t\t" << p.subject_format << "\n";
    cout << "\tType of subject:\t" << p.type_of_subject << "\n";
    cout << "\tA0:\t\t\t" << p.A0 << "\n";
    cout << "\tA1:\t\t\t" << p.A1 << "\n";
    cout << "\tAmount of parameter:\t" << p.amount_of_parameter << "\n\n\n";
}




int main()
{
    const char * filename = "1.us0";
    //const char * filename = "ЕН Д1-042-014 слот ММП дв ед.us0"; // такое имя не читается
    if (file_opening(filename) == 1)
        return 1;
    SetConsoleOutputCP(866);
//    SetConsoleCP(866);


    int source_head[] = {4, 8, 8, 8, 4, 4, 2, 1, 1}; // ИД для чтения заголовка, сколько Байт весит параметр

    Heading h;
    read_heading(filename, source_head, &h);
    print_heading(h); // прочитать из файла и вывести данные заголовка


    int source_pas[] = {12, 2, 1, 1, 4, 4, 8}; // ИД для чтения паспорта, сколько Байт весит параметр
    //int amount_passports = h.amount_passports; // сколько всего паспортов
    int amount_passports = 10; //test
    int passport_position = 40 + h.comment_lenght; // с какой позиции начинаются паспорта    
    int size_of_passport = 32;

    Passport pas[amount_passports];
    for (int n = 0; n < amount_passports; n++) // прочитать и вывести данные паспортов
    {
        read_passport(filename, passport_position + n * size_of_passport, source_pas, &pas[n]);
        print_passport(pas[n]);
    }

    return 0;
}
