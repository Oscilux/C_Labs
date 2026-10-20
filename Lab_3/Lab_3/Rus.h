#pragma once

#ifndef RUS_H
#define RUS_H

//      /------------------------------------------------------------------------\
//      |                            ОБЯЗАТЕЛЬНО                                 |
//      |    кодировка файлов .c .h и т.п. должна быть - UTF-8 (С подписью)      |
//      | в свойствах проекта лучше поставить флаг чтобы он читал всё как UTF-8  |
//      \------------------------------------------------------------------------/

#define _CRT_SECURE_NO_WARNINGS	// для VS, отключает ошибки printf scanf

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include <locale.h>
#include <wchar.h>
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <stdarg.h>

//--------------------
//    ВВОД ВЫВОД
//--------------------
#define print(format, ...) wprintf(L##format, ##__VA_ARGS__)  // меняет сигнатуру wprintf(L"Ввод: %ls\n", string); на print("Ввод: %ls\n", string)
#define input(format, ...) _input(L##format, ##__VA_ARGS__)

// включает русский язык
void EnableRu()
{
	setlocale(LC_ALL, ".utf8");		// настраивает локаль стандартной библиотеки C на использование кодировки UTF-8

	//setlocale(LC_NUMERIC, "C");    // Оставляет точку в качестве разделителя для чисел
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1)  // переводит вывод консоли в Unicode
    {
        fprintf(stderr, "Ошибка: Не удалось перевести стандартный вывод (stdout) в режим Unicode.\n");
        exit(1);
    }
    if (_setmode(_fileno(stdin), _O_U16TEXT) == -1)   // переводит ввод консоли в Unicode
    {
        fprintf(stderr, "Ошибка: Не удалось перевести стандартный ввод (stdin) в режим Unicode.\n");
        exit(1);
    }
}

// ввод с русскими буквами и проверками
// wchar_t потому что функции обрабатывающие char не умеют адекватно получать русские буквы из консоли, или я не нашёл
int _input(const wchar_t* format, ...)
{
    //лист аргументов
    va_list args;
    va_start(args, format);

    int result = vwscanf(format, args);

    va_end(args);

    // проверка прочитанного значения
    if (result == 0)
    {
        wprintf(L"Ошибка: данные не соответствуют формату\n");
        return 1;
    }
    else if (result < 0)
    {
        int atEnd = feof(stdin);

        if (atEnd)
            wprintf(L"\nКонец входного потока. Завершение работы.\n");
        else
            wprintf(L"\nОшибка чтения входного потока. Завершение работы.\n");

        // если флаг EOF уже есть то другие функции не будут продолжать ввод
        if (_isatty(_fileno(stdin)))
        {
            wprintf(L"\nДля продолжения нажмите любую клавишу...");
            fflush(stdout);
            _getch();           // ждёт ровно одну клавишу, Enter не нужен, EOF не мешает
        }

        fflush(stdout);                             // чтобы текст точно вышел до выхода
        exit(atEnd ? EXIT_SUCCESS : EXIT_FAILURE);
    }

    return 0;
}

// очистка буфера
void ClearBuffer()
{
    int c;
    while ((c = getwchar()) != '\n' && c != WEOF);
}

#endif // !RUS_H