/*

    Вариант № 5

    3. Выполнить задание лабораторной работы № 3 «Перечисляемый тип. Массив структур», используя для хранения информации динамический массив структур.

    ВАЖНО при выполнении заданий 1-3: для обращения к элементам массива использовать операцию разыменования (нельзя использовать [ ]);
    при добавлении и удалении информации грамотно работать с памятью

*/

#include "Zadanie3.h"

enum WorkList
{
    off,
    work1,
    work2,
    work3,
    work4,
    work5,
    work6
};

struct date
{
    int day;
    int month;
    int year;
};

wchar_t* formatDate(struct date* date)
{
    static wchar_t buffer[12];

    swprintf(buffer, 12, L"%02d.%02d.%d", date->day, date->month, date->year);

    return buffer;
}

int isValidDate(struct date* date)
{
    int days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if ((date->year < 1) || (date->year > 9999)) return 0;
    if ((date->month < 1) || (date->month > 12)) return 0;
    if ((date->day < 1) || (date->day > *(days + (date->month - 1)) )) return 0;

    return 1;
}

struct Journal
{
    int code;               //код
    wchar_t name[50];       //название
    struct date date;       //дата выпуска
    wchar_t publisher[50];  //издательство
    int pageCount;          //количество страниц
    wchar_t fullName[50];   //ФИО главного редактора
};

static struct Journal* journals = NULL;
static int journalCount = 0;       // сколько записей хранится
static int journalCapacity = 0;    // сколько записей помещается в выделенной памяти

static const struct Journal initialJournals[] = {
    {
        1100,
        L"Будни Меллстроя",
        {11, 9, 2067},
        L"Новое время",
        67,
        L"Иванов И.И."
    },
    {
        2222,
        L"Меллстройная устройность",
        {6, 7, 1984},
        L"Новое время",
        777,
        L"Иванов И.И."
    },
    {
        2834,
        L"Вред цыганских таборов для Македонии",
        {6, 7, 2016},
        L"ZXC_РУССКИЙ_СТАНДАРТ_ZXC",
        37923,
        L"Александр MOGGедонский"
    },
    {
        9473,
        L"План движухи",
        {6, 2, 2018},
        L"bil_ne_grov",
        37923,
        L"Путин В.В."
    },
    {
        8435,
        L"Навозная мысль",
        {6, 2, 2010},
        L"Новое время",
        1234,
        L"Джордж Дроид"
    },
    {
        999999,
        L"Кобилджон о важном",
        {12, 7, 2010},
        L"Новое время",
        78,
        L"Кобилджон"
    }
};

int reserveJournalMemory(int requireSize)
{
    if (requireSize <= journalCapacity)
        return 1;

    int newCapacity = journalCapacity;

    while (newCapacity < requireSize)
        newCapacity += 50;

    struct Journal* temp = realloc(journals, newCapacity * sizeof * journals);
    if (temp == NULL)    // ошибка выделения памяти
    {
        print("Ошибка выделения памяти\n");
        return 0;
    }

    journals = temp;
    journalCapacity = newCapacity;
    return 1;
}

int initializeJournals()
{
    int initialCount = (int)(sizeof initialJournals / sizeof *initialJournals);

    if (!reserveJournalMemory(initialCount))
        return 0;

    memcpy(journals, initialJournals, initialCount * sizeof *journals);
    journalCount = initialCount;

    return 1;
}

void addJournalInList()
{
    if (!reserveJournalMemory(journalCount + 1))
    {
        print("Ошибка выделения памяти\n");
        return;
    }

    // проверка уникальности кода
    int code;
    for (;;)
    {
        print("Добавление нового журнала, введите код нового журнала: ");

        if (input("%d", &code))
        {
            ClearBuffer();
            print("Некорректный ввод.\n");
            continue;
        }

        if (code < 1 || code > 999999)
        {
            print("Код должен быть от 1 до 999999.\n");
            continue;
        }

        int codeIsset = 0;

        for (int i = 0; i < journalCount; i++)
        {
            struct Journal* current = journals + i;

            if ((*current).code == code)
            {
                codeIsset = 1;
                break;
            }
        }

        if (codeIsset)
        {
            print("Этот код уже занят.\n");
            continue;
        }

        break;
    }

    // создание журнала
    struct Journal addedJournal;
    addedJournal.code = code;

    do
    {
        ClearBuffer();
        print("Введите название(50 символов): ");
        if (input("%49l[^\n]", addedJournal.name))
            continue;

        break;
    } while (1);

    do
    {
        ClearBuffer();
        print("Введите дату выпуска(## ## ####): ");
        if (input("%d %d %d", &addedJournal.date.day, &addedJournal.date.month, &addedJournal.date.year))
            continue;

        if (!isValidDate(&addedJournal.date))
        {
            print("Введена некорректная дата: %d %d %d\n", addedJournal.date.day, addedJournal.date.month, addedJournal.date.year);
            continue;
        }

        break;
    } while (1);

    do
    {
        ClearBuffer();
        print("Введите издательство(50 символов): ");
        if (input("%49l[^\n]", addedJournal.publisher))
            continue;

        break;
    } while (1);

    do
    {
        ClearBuffer();
        print("Введите количество страниц: ");
        if (input("%d", &addedJournal.pageCount))
            continue;

        if (addedJournal.pageCount < 1)
        {
            print("Страниц не может быть меньше 1\n");
            continue;
        }

        break;
    } while (1);

    do
    {
        ClearBuffer();
        print("Введите ФИО главного редактора(50 символов): ");
        if (input("%49l[^\n]", addedJournal.fullName))
            continue;

        break;
    } while (1);

    *(journals + journalCount) = addedJournal;
    journalCount++;

    print("Журнал добавлен!\n");
}

// тип функции условия
typedef int (*Condition)(struct Journal*);

// поиск по журналам
void journalSearch(Condition check, struct Journal* resultOUT, int* counterOUT)
{
    int counter = 0;

    for (int i = 0; i < journalCount; i++)
    {
        if ((journals + i)->code != 0 && check((journals + i)))
        {
            if (resultOUT != NULL)
                *(resultOUT + counter) = *(journals + i);
            counter++;
        }
    }

    if (counterOUT != NULL)
        *counterOUT = counter;
}

// проверка издателя
int fullNameCheck(struct Journal* journal)
{
    return !wcscmp(journal->fullName, L"Иванов И.И.") ? 1 : 0;
}

// проверка по датам c 2015 марта по 2018 марта
int dateCheck(struct Journal* journal)
{
    if ((journal->date.year > 2015 || (journal->date.year == 2015 && journal->date.month >= 3)) &&
        (journal->date.year < 2018 || (journal->date.year == 2018 && journal->date.month <= 3)))
        return 1;

    return 0;
}

// проверка по датам c 2015 марта по 2018 марта
int work5Check(struct Journal* journal)
{
    if (journal->date.year == 2010 && !wcscmp(journal->publisher, L"Новое время"))
        return 1;

    return 0;
}

int work6Check(struct Journal* journal)
{
    if (*(journal->name) == L'К')
        return 1;

    return 0;
}

// вывод журналов в виде таблицы
void printJournals(struct Journal* journals)
{
    print("/---------------------------------------------------------------------------------"
        "---------------------------------------------------------------------------------------------------------------\\\n");
    print("| %-6ls | %-50ls | %-12ls | %-50ls | %-7ls | %-50ls |\n",
        L"Код", L"Название", L"Дата", L"Издательство", L"Стр.", L"Главный редактор");
    print("----------------------------------------------------------------------------------"
        "----------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < journalCount; i++)
    {
        if ((journals + i)->code != 0)
            print("| %-6d | %-50ls | %-12ls | %-50ls | %-7d | %-50ls |\n",
                (journals + i)->code, (journals + i)->name, formatDate(&((journals + i)->date)), (journals + i)->publisher, (journals + i)->pageCount, (journals + i)->fullName);
    }

    print("\\---------------------------------------------------------------------------------"
        "---------------------------------------------------------------------------------------------------------------/\n");
}

void Zadanie3Main()
{
    if (!initializeJournals())
    {
        print("Ошибка инициализации\n");
        return;
    }

    int pick = 0;

    while (1)
    {
        print("%d - Добавление нового журнала\n", work1);
        print("%d - Поиск информации о журналах, главным редактором которых является «Иванов И.И.»\n", work2);
        print("%d - Поиск информации о журналах, изданных с марта 2015 по март 2018 гг.\n", work3);
        print("%d - Отображение информации о журналах в виде таблицы\n", work4);
        print("%d - Определение количества журналов издательства «Новое время», вышедших в 2010 году\n", work5);
        print("%d - Определение количества журналов с названием на букву «К»\n", work6);

        print("%d - Выход\n", off);

        print("Введите команду: ");

        // ввод команды
        if (input("%d", &pick))
        {
            print("\n\n");
            ClearBuffer();
            continue;
        }

        print("\n");

        switch (pick)
        {
            // выключение
        case off:
        {
            print("Выход\n");

            free(journals);
            journals = NULL;

            return;
        }
        // Добавление нового журнала
        case work1:
        {
            addJournalInList();
            break;
        }
        // Поиск информации о журналах, главным редактором которых является «Иванов И.И.»
        case work2:
        {
            print("Журналы, главным редактором которых является «Иванов И.И.»:\n");

            struct Journal* searchResult = malloc(journalCount * sizeof * journals);
            int counter;

            journalSearch(fullNameCheck, searchResult, &counter);

            print("/---------------------------------------------------------------------------------------------------------------------------------\\\n");
            print("| %-6ls | %-50ls | %-12ls | %-50ls |\n", L"Код", L"Название", L"Дата", L"Издательство");
            print("-----------------------------------------------------------------------------------------------------------------------------------\n");

            for (int i = 0; i < counter; i++)
                print("| %-6d | %-50ls | %-12ls | %-50ls |\n",
                    (searchResult + i)->code, (searchResult + i)->name, formatDate(&((searchResult + i)->date)), ((searchResult + i)->publisher));

            print("\\---------------------------------------------------------------------------------------------------------------------------------/\n");

            free(searchResult);

            break;
        }
        // Поиск информации о журналах, изданных с марта 2015 по март 2018 гг
        case work3:
        {
            print("Журналы, изданные с марта 2015 по март 2018 гг:\n");

            struct Journal* searchResult = malloc(journalCount * sizeof * journals);
            int counter;

            journalSearch(dateCheck, searchResult, &counter);

            print("/----------------------------------------------------------------------------------------------------------------------------------\\\n");
            print("| %-50ls | %-12ls | %-50ls | %-7ls |\n", L"Название", L"Дата", L"Издательство", L"Стр.");
            print("------------------------------------------------------------------------------------------------------------------------------------\n");

            for (int i = 0; i < counter; i++)
                print("| %-50ls | %-12ls | %-50ls | %-7d |\n",
                    (searchResult + i)->name, formatDate(&((searchResult + i)->date)), (searchResult + i)->publisher, (searchResult + i)->pageCount);

            print("\\----------------------------------------------------------------------------------------------------------------------------------/\n");

            free(searchResult);

            break;
        }
        // Отображение информации о журналах в виде таблицы
        case work4:
        {
            print("Таблица всех журналов:\n");

            printJournals(journals);
            break;
        }
        // Определение количества журналов издательства «Новое время», вышедших в 2010 году
        case work5:
        {
            int counter = 0;

            journalSearch(work5Check, NULL, &counter);

            print("Количество журналов издательства «Новое время», вышедших в 2010 году: %d\n", counter);

            break;
        }
        // Определение количества журналов с названием на букву «К»
        case work6:
        {
            int counter = 0;

            journalSearch(work6Check, NULL, &counter);

            print("Количество журналов с названием на букву «К»: %d\n", counter);

            break;
        }
        default:
        {
            print("Неверная команда, ввод: %d\n", pick);
            break;
        }
        }

        print("\n\n");
    }

    return;
}