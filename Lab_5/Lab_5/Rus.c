#include "Rus.h"

// устанавливает высоту буфера в строках
void SetBufferHeight(SHORT requestedHeight)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;

    if (console == NULL ||
        console == INVALID_HANDLE_VALUE ||
        !GetConsoleScreenBufferInfo(console, &info))
    {
        fprintf(stderr, "Не удалось получить доступ к буферу консоли.\n");
        return;
    }

    // Буфер должен быть не ниже видимой области окна.
    int visibleHeight = (int)info.srWindow.Bottom - (int)info.srWindow.Top + 1;

    // не позволяет попросить буфер меньше видимой области
    if (requestedHeight < visibleHeight)
        requestedHeight = (SHORT)visibleHeight;

    COORD size;
    size.X = info.dwSize.X;       // сохраняет текущую ширину
    size.Y = requestedHeight;     // задаёт высоту буфера

    // устанавливает новую высоту буфера в строках
    if (!SetConsoleScreenBufferSize(console, size))
    {
        fprintf(stderr, "Не удалось увеличить буфер. Код ошибки: %lu\n",
            (unsigned long)GetLastError());
    }
}

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
    while ((c = getwchar()) != L'\n' && c != WEOF);
}

int _inputString(wchar_t* buf, size_t n)
{
    // проверка значений
    if (buf == NULL || n < 2)
        return READ_BAD_ARGS;

    int count = (n > (size_t)INT_MAX) ? INT_MAX : (int)n;

    // ввод строки из конслои
    if (fgetws(buf, count, stdin) == NULL)
    {
        // обработка ошибки
        int atEnd = feof(stdin);

        if (atEnd)
            wprintf(L"\nКонец входного потока. Завершение работы.\n");
        else
            wprintf(L"\nОшибка чтения входного потока.\n");

        if (_isatty(_fileno(stdin)))
        {
            wprintf(L"Для продолжения нажмите любую клавишу...");
            fflush(stdout);
            _getch();
        }

        fflush(stdout);
        exit(atEnd ? EXIT_SUCCESS : EXIT_FAILURE);
    }

    size_t len = wcslen(buf);   // длина строки в буфере

    int complete = (len > 0 && buf[len - 1] == L'\n');

    if (complete)
    {
        buf[len - 1] = L'\0';
        return READ_OK;         // успех
    }

    // если '\n' в буфере нет => буфер забит под завязку,
    // хвост строки всё ещё висит в stdin и его надо убрать
    ClearBuffer();
    return READ_TRUNCATED;  // строка обрезана
}