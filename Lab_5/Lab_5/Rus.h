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
#include <limits.h>

typedef enum {
    READ_OK = 0,
    READ_TRUNCATED = 1,
    READ_BAD_ARGS = 2
} ReadStatus;

//--------------------
//    ВВОД ВЫВОД
//--------------------
#define print(format, ...) wprintf(L##format, ##__VA_ARGS__)  // меняет сигнатуру wprintf(L"Ввод: %ls\n", string); на print("Ввод: %ls\n", string)
#define input(format, ...) _input(L##format, ##__VA_ARGS__)
#define inputString(buf) _inputString((buf), sizeof(buf) / sizeof((buf)[0]))

// устанавливает высоту буфера в строках
void SetBufferHeight(SHORT requestedHeight);

// включает русский язык
void EnableRu();

// ввод с русскими буквами и проверками
// wchar_t потому что функции обрабатывающие char не умеют адекватно получать русские буквы из консоли, или я не нашёл
int _input(const wchar_t* format, ...);

// очистка буфера
void ClearBuffer();

int _inputString(wchar_t* buf, size_t n);

#endif // !RUS_H