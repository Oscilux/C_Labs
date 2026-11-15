#include "Rus.h"

// структура даты
typedef struct Date
{
    int day;
    int month;
    int year;
} Date;

wchar_t* formatDate(struct Date* date)
{
    static wchar_t buffer[12];

    swprintf(buffer, 12, L"%02d.%02d.%d", date->day, date->month, date->year);

    return buffer;
}

int isValidDate(struct Date* date)
{
    int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if ((date->year < 1) || (date->year > 9999)) return 1;
    if ((date->month < 1) || (date->month > 12)) return 1;
    if ((date->day < 1) || (date->day > days[date->month - 1])) return 1;

    return 0;
}

// структура журнала
typedef struct Journal
{
    int code;               //код
    wchar_t name[50];       //название
    Date date;              //дата выпуска
    wchar_t publisher[50];  //издательство
    int pageCount;          //количество страниц
    wchar_t fullName[50];   //ФИО главного редактора
} Journal;

// структура госномера книги
// wchar_t потому что в начале может быть 0
typedef struct ISBN10
{
    wchar_t registrationGroup[10];  // номер группы регистрации
    wchar_t registrant[10];         // номер издателя или другой организации, которой выделен ISBN
    wchar_t publicationNumber[10];  // номер публикации
    wchar_t check_digit;            // контрольная цифра
} ISBN10;

wchar_t* formatISBN10(struct ISBN10* isbn)
{
    static wchar_t buffer[34];

    swprintf(buffer, 34, L"%ls-%ls-%ls-%lc", isbn->registrationGroup, isbn->registrant, isbn->publicationNumber, isbn->check_digit);

    return buffer;
}

// структура книги
typedef struct Book
{
    ISBN10 isbn;            // госномер книги
    wchar_t name[50];       // название
    wchar_t fullName[50];   // ФИО автора
    wchar_t publisher[50];  // издательство
    Date date;              // дата выпуска
} Book;

// объединение книг и журналов
typedef union JournalOrBook
{
    Journal journal;
    Book book;
} JournalOrBook;

// перечисление книга или журнал
typedef enum JournalOrBookTag
{
    None,
    isBook,
    isJournal
} JournalOrBookTag;

// структура предмета
typedef struct Item
{
    JournalOrBook item;
    JournalOrBookTag tag;
} Item;

// ввод журнала
Journal InputJournal()
{
    Journal journal;

    // ввод кода
    do
    {
        print("Введите код\n--> ");
        if (input("%d", &journal.code))
        {
            ClearBuffer();
            continue;
        }

        // проверка
        if (journal.code == 0)
        {
            print("Код не может быть 0\n");
            ClearBuffer();

            continue;
        }
        else if (journal.code > 999999 || journal.code < 1)
        {
            print("Код не может быть больше 999999 или меньше 1, введите другой\n");
            ClearBuffer();

            continue;
        }

        ClearBuffer();
        break;
    } while (1);

    // ввод названия
    do
    {
        print("Введите название\n--> ");
        if (inputString(journal.name))
            print("Обрезано до 50 символов\n");

        break;
    } while (1);

    // ввод даты
    do
    {
        print("Введите дату(## ## ####)\n--> ");
        if (input("%d %d %d", &journal.date.day, &journal.date.month, &journal.date.year))
        {
            ClearBuffer();
            continue;
        }

        if (isValidDate(&journal.date))
        {
            print("Введена неправильная дата\n");
            ClearBuffer();
            continue;
        }

        ClearBuffer();
        break;
    } while (1);

    // ввод издательства
    do
    {
        print("Введите издательство\n--> ");
        if (inputString(journal.publisher))
            print("Обрезано до 50 символов\n");

        break;
    } while (1);
    
    // ввод кол-ва страниц
    do
    {
        print("Введите количество страниц\n--> ");
        if (input("%d", &journal.pageCount))
        {
            ClearBuffer();
            continue;
        }

        if (journal.pageCount < 1)
        {
            print("Страниц не может быть меньше 1\n");

            ClearBuffer();
            continue;
        }

        ClearBuffer();
        break;
    } while (1);

    // ввод ФИО главного редактора
    do
    {
        print("Введите ФИО главного редактора\n--> ");
        if (inputString(journal.fullName))
            print("Обрезано до 50 символов\n");

        break;
    } while (1);

    return journal;
}

// вывод журнала на экран
void PrintJournal(Journal* journal)
{
    if (journal->code != 0)
        print("| %-6d | %-*ls | %-12ls | %-*ls | %-7d | %-*ls |\n",
            journal->code,
            (int)(sizeof(journal->name) / sizeof(journal->name[0])), journal->name,
            formatDate(&journal->date),
            (int)(sizeof(journal->publisher) / sizeof(journal->publisher[0])), journal->publisher,
            journal->pageCount,
            (int)(sizeof(journal->fullName) / sizeof(journal->fullName[0])), journal->fullName);
    else
        print("Код журнала равен 0");
}

// проверка что строка число
int NotNumber(wchar_t* string)
{
    if (string == NULL || *string == L'\0')
        return 1;

    for (; *string != L'\0'; ++string)
        if (*string < L'0' || *string > L'9')
            return 1;

    return 0;
}

// ввод книги
Book InputBook()
{
    Book book;

    // ввод ISBN10
    print("Введите ISBN10\n");

    // номер группы регистрации
    do
    {
        print("Введите номер группы регистрации\n--> ");
        if (inputString(book.isbn.registrationGroup))
            print("Обрезано до %d символов\n", (int)(sizeof(book.isbn.registrationGroup) / sizeof(book.isbn.registrationGroup[0])));

        if (NotNumber(book.isbn.registrationGroup))
        {
            print("Не является числом, введено: %ls\n", book.isbn.registrationGroup);
            continue;
        }

        break;
    } while (1);

    // номер издателя или другой организации, которой выделен ISBN
    do
    {
        print("Введите номер издателя или другой организации, которой выделен ISBN\n--> ");
        if (inputString(book.isbn.registrant))
            print("Обрезано до %d символов\n", (int)(sizeof(book.isbn.registrant) / sizeof(book.isbn.registrant[0])));

        if (NotNumber(book.isbn.registrant))
        {
            print("Не является числом, введено: %ls\n", book.isbn.registrant);
            continue;
        }

        break;
    } while (1);

    // номер публикации
    do
    {
        print("Введите номер публикации\n--> ");
        if (inputString(book.isbn.publicationNumber))
            print("Обрезано до %d символов\n", (int)(sizeof(book.isbn.publicationNumber) / sizeof(book.isbn.publicationNumber[0])));

        if (NotNumber(book.isbn.publicationNumber))
        {
            print("Не является числом, введено: %ls\n", book.isbn.publicationNumber);
            continue;
        }

        break;
    } while (1);

    // контрольная цифра
    do
    {
        print("Введите контрольную цифру\n--> ");
        if (input("%lc", &book.isbn.check_digit))
        {
            ClearBuffer();
            continue;
        }
        
        if (!((book.isbn.check_digit >= L'0' || book.isbn.check_digit <= L'9') || 
            (book.isbn.check_digit == L'X' || book.isbn.check_digit == L'x')))
        {
            print("Не является числом или X, введено: %lc\n", book.isbn.check_digit);
            continue;
        }

        ClearBuffer();
        break;
    } while (1);

    // ввод названия
    do
    {
        print("Введите название\n--> ");
        if (inputString(book.name))
            print("Обрезано до 50 символов\n");

        break;
    } while (1);

    // ввод ФИО автора
    do
    {
        print("Введите ФИО автора\n--> ");
        if (inputString(book.fullName))
            print("Обрезано до 50 символов\n");

        break;
    } while (1);

    // ввод издательства
    do
    {
        print("Введите издательство\n--> ");
        if (inputString(book.publisher))
            print("Обрезано до 50 символов\n");

        break;
    } while (1);

    // ввод даты
    do
    {
        print("Введите дату(## ## ####)\n--> ");
        if (input("%d %d %d", &book.date.day, &book.date.month, &book.date.year))
        {
            ClearBuffer();
            continue;
        }

        if (isValidDate(&book.date))
        {
            print("Введена неправильная дата\n");
            ClearBuffer();
            continue;
        }

        ClearBuffer();
        break;
    } while (1);

    return book;
}

void PrintBook(Book* book)
{
    print("| %-35ls | %-*ls | %-*ls | %-*ls | %-12ls |\n",
        formatISBN10(&book->isbn),
        (int)(sizeof(book->name) / sizeof(book->name[0])), book->name,
        (int)(sizeof(book->fullName) / sizeof(book->fullName[0])), book->fullName,
        (int)(sizeof(book->publisher) / sizeof(book->publisher[0])), book->publisher,
        formatDate(&book->date));
}