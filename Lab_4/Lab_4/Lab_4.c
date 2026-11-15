// Lab_4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

/*

    В программе создать структуры :
    1) «Журнал» с обязательными полями : код, название, дата выпуска, издательство, количество страниц, ФИО главного редактора;

    2) «Книга» с обязательными полями : ISBN, название, ФИО автора, издательство, год выпуска.

    Разработать объединение «Журналы и книги» с полями : структура «Журнал», структура «Книга».

    Информацию о журналах и книгах хранить в статическом массиве объединений (массив ОДИН для журналов и книг).

    Меню(функционал) программы :
    1) добавление информации о новом журнале или новой книге в массив;
    2) отображение информации о журналах и книгах;
    3) отображение информации о журналах в виде таблицы;
    4) определение количества книг;
    5) поиск информации о книгах с названием на букву «М»(вывести на экран всю информацию о найденных книгах);
    6) выход.

    В программе для разработки меню использовать перечисляемый тип.
    Программа должна работать до тех пор, пока пользователь не решит из неё выйти (использовать оператор цикла).
    При вводе некорректных данных выдавать сообщение об ошибке.

*/

#include "Rus.h"
#include "Structures.h"

#define ITEM_COUNT 50

enum WorkList
{
    off,
    work1,
    work2,
    work3,
    work4,
    work5
};

static Item journalsAndBooks[ITEM_COUNT] = {
    {
        .item = {
            .book = {
                .isbn = {
                    .registrationGroup = L"0",
                    .registrant = L"306",
                    .publicationNumber = L"40615",
                    .check_digit = L'2'
                },
                .name = L"Мастер и Маргарита",
                .fullName = L"Михаил Булгаков",
                .publisher = L"Пример издательства",
                .date = { 1, 1, 1967 }
            }
        },
        .tag = isBook
    },
    {
        .item = {
            .journal = {
                .code = 101,
                .name = L"Наука и жизнь",
                .date = { 15, 5, 2024 },
                .publisher = L"Пример издательства",
                .pageCount = 120,
                .fullName = L"Иванов Иван Иванович"
            }
        },
        .tag = isJournal
    },
    {
        .item = {
            .book = {
                .isbn = {
                    .registrationGroup = L"5",
                    .registrant = L"699",
                    .publicationNumber = L"12345",
                    .check_digit = L'8'
                },
                .name = L"Маленький принц",
                .fullName = L"Антуан де Сент-Экзюпери",
                .publisher = L"Эксмо",
                .date = { 6, 4, 1943 }
            }
        },
        .tag = isBook
    },
    {
        .item = {
            .journal = {
                .code = 102,
                .name = L"Мир техники",
                .date = { 15, 4, 2023 },
                .publisher = L"Техносфера",
                .pageCount = 80,
                .fullName = L"Алексей Сергеевич Смирнов"
            }
        },
        .tag = isJournal
    },
    {
        .item = {
            .journal = {
                .code = 103,
                .name = L"Сад и огород",
                .date = { 1, 6, 2024 },
                .publisher = L"Дом-пресс",
                .pageCount = 64,
                .fullName = L"Мария Ивановна Петрова"
            }
        },
        .tag = isJournal
    }
};

void inputNewItem()
{
    // поиск свободного места в массиве
    int freeItemNumber = -1;
    for (int i = 0; i < ITEM_COUNT; i++)
        if (journalsAndBooks[i].tag == None)
        {
            freeItemNumber = i;
            break;
        }

    if (freeItemNumber == -1)
    {
        print("Все места в массиве заняты\n");
        return;
    }

    int type;
    Item newItem;

    while (1)
    {
        type = 0;

        print("Книга или журнал?\n%d - книга\n%d - журнал\n--> ", isBook, isJournal);
        if (input("%d", &type))
            return;

        ClearBuffer();

        switch (type)
        {
            // ввод журнала
            case isJournal:
            {
                newItem.item.journal = InputJournal();
                newItem.tag = isJournal;

                journalsAndBooks[freeItemNumber] = newItem;

                return;
            }
            // ввод книги
            case isBook:
            {
                newItem.item.book = InputBook();
                newItem.tag = isBook;

                journalsAndBooks[freeItemNumber] = newItem;

                return;
            }
            default:
                continue;
        }
    }
}

// Вывод таблицы журналов
void PrintJournals()
{
    print("----------------------------------------------------------------------------------"
        "----------------------------------------------------------------------------------------------------------------\n");
    print("|\t\t\t\t\t\t\t\t\t\tТаблица журналов                                                                                                 |\n");
    print("----------------------------------------------------------------------------------"
        "----------------------------------------------------------------------------------------------------------------\n");
   
    print("| %-6ls | %-*ls | %-12ls | %-*ls | %-7ls | %-*ls |\n",
        L"Код",
        (int)(sizeof(journalsAndBooks[0].item.journal.name) / sizeof(journalsAndBooks[0].item.journal.name[0])), L"Название",
        L"Дата выпуска",
        (int)(sizeof(journalsAndBooks[0].item.journal.publisher) / sizeof(journalsAndBooks[0].item.journal.publisher[0])), L"Издатель",
        L"Страниц",
        (int)(sizeof(journalsAndBooks[0].item.journal.fullName) / sizeof(journalsAndBooks[0].item.journal.fullName[0])), L"ФИО главного редактора");

    print("----------------------------------------------------------------------------------"
        "----------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (journalsAndBooks[i].tag == isJournal)
            PrintJournal(&journalsAndBooks[i].item.journal);
    }

    print("----------------------------------------------------------------------------------"
        "----------------------------------------------------------------------------------------------------------------\n");
}

// Вывод таблицы книг
void PrintBooks()
{
    print("-----------------------------------------------------------------------------------------------------"
        "----------------------------------------------------------------------------------------------------------------\n");
    print("|\t\t\t\t\t\t\t\t\t\t\t\tТаблица книг                                                                                                        |\n");
    print("-----------------------------------------------------------------------------------------------------"
        "----------------------------------------------------------------------------------------------------------------\n");

    print("| %-35ls | %-*ls | %-*ls | %-*ls | %-12ls |\n",
        L"Госномер книги",
        (int)(sizeof(journalsAndBooks[0].item.book.name) / sizeof(journalsAndBooks[0].item.book.name[0])), L"Название",
        (int)(sizeof(journalsAndBooks[0].item.book.fullName) / sizeof(journalsAndBooks[0].item.book.fullName[0])), L"Имя автора",
        (int)(sizeof(journalsAndBooks[0].item.book.publisher) / sizeof(journalsAndBooks[0].item.book.publisher[0])), L"Издательство",
        L"Дата выпуска");

    print("-----------------------------------------------------------------------------------------------------"
        "----------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < ITEM_COUNT; i++)
    {
        if (journalsAndBooks[i].tag == isBook)
            PrintBook(&journalsAndBooks[i].item.book);
    }
    print("-----------------------------------------------------------------------------------------------------"
        "----------------------------------------------------------------------------------------------------------------\n");
}

int main()
{
    EnableRu();
    system("mode con cols=220 lines=40");
    SetBufferHeight(1000);

    int choice;

    while (1)
    {
        print("/-----------------------------------------------------------------------\\\n");
        print("| %d - Добавление информации о новом журнале или новой книге в массив    |\n", work1);
        print("| %d - Отображение информации о журналах и книгах                        |\n", work2);
        print("| %d - Отображение информации о журналах                                 |\n", work3);
        print("| %d - Определение количества книг                                       |\n", work4);
        print("| %d - Поиск информации о книгах с названием на букву «М»                |\n", work5);
        print("| %d - Выход                                                             |\n", off);
        print("\\-----------------------------------------------------------------------/\n\n");

        // ввод команды
        choice = -1;
        print("Введите команду:\n--> ");
        if (input("%d", &choice))
        {
            print("\n");
            ClearBuffer();
            continue;
        }

        // команды
        print("\n");
        switch (choice)
        {
            // Добавление информации о новом журнале или новой книге в массив
            case work1:
                print("Добавление информации о новом журнале или новой книге в массив\n");
                inputNewItem();

                break;
            // вывод всех журналов и книг, по типам
            case work2:
                print("Отображение информации о журналах и книгах\n\n");

                PrintJournals();

                print("\n\n\n");

                PrintBooks();

                break;
            // отображение информации о журналах в виде таблицы
            case work3:
                print("Отображение информации о журналах в виде таблицы\n\n");

                PrintJournals();

                break;
            // Определение количества книг
            case work4:
                print("Определение количества книг\n\n");

                int count = 0;

                for (int i = 0; i < ITEM_COUNT; i++)
                {
                    if (journalsAndBooks[i].tag == isBook)
                        count++;
                }

                print("Всего книг: %d\n", count);

                break;
            // Поиск информации о книгах с названием на букву «М»
            case work5:
                print("Поиск информации о книгах с названием на букву «М»\n\n");

                print("-----------------------------------------------------------------------------------------------------"
                    "----------------------------------------------------------------------------------------------------------------\n");

                print("| %-35ls | %-*ls | %-*ls | %-*ls | %-12ls |\n",
                    L"Госномер книги",
                    (int)(sizeof(journalsAndBooks[0].item.book.name) / sizeof(journalsAndBooks[0].item.book.name[0])), L"Название",
                    (int)(sizeof(journalsAndBooks[0].item.book.fullName) / sizeof(journalsAndBooks[0].item.book.fullName[0])), L"Имя автора",
                    (int)(sizeof(journalsAndBooks[0].item.book.publisher) / sizeof(journalsAndBooks[0].item.book.publisher[0])), L"Издательство",
                    L"Дата выпуска");

                print("-----------------------------------------------------------------------------------------------------"
                    "----------------------------------------------------------------------------------------------------------------\n");

                for (int i = 0; i < ITEM_COUNT; i++)
                    if (journalsAndBooks[i].tag == isBook)
                        if (journalsAndBooks[i].item.book.name[0] == L'М' || journalsAndBooks[i].item.book.name[0] == L'м' ||   // русская
                            journalsAndBooks[i].item.book.name[0] == L'M' || journalsAndBooks[i].item.book.name[0] == L'm')     // английская
                            PrintBook(&journalsAndBooks[i].item.book);

                print("-----------------------------------------------------------------------------------------------------"
                    "----------------------------------------------------------------------------------------------------------------\n");

                break;
            case off:
                print("Выход\n");

                return 0;
            // Неверная команда
            default:
                print("Неверная команда\n");

                break;
        }

        print("\n");
    }
}
