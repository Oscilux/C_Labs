/*

    Вариант № 5
    1. Определить одномерный вещественный динамический массив (при помощи указателя).

    Меню (функционал) программы:
    1) добавление элемента в указанную позицию;
    2) удаление последнего элемента;
    3) отображение значений элементов массива;
    4) очистка массива;
    5) поиск указанного пользователем элемента (вывести на экран позицию элемента);
    6) сортировка элементов массива по возрастанию.

    Программа должна работать до тех пор, пока пользователь не решит из неё выйти (использовать оператор цикла).
    При вводе некорректных данных выдавать сообщение об ошибке.
    
    ВАЖНО при выполнении заданий 1-3: для обращения к элементам массива использовать операцию разыменования (нельзя использовать [ ]);
    при добавлении и удалении информации грамотно работать с памятью
*/

#include "Zadanie1.h"

typedef enum Zadanie1Menu
{
    z1_off,
    z1_1,
    z1_2,
    z1_3,
    z1_4,
    z1_5,
    z1_6
} Zadanie1Menu;

// вывод значений массива
void PrintMas(float* mas, int *count)
{
    if (mas == NULL || count <= 0)
    {
        print("Массив полностью пуст\n");
        return;
    }

    for (int i = 0; i < *count; ++i)
    {
        print("%d:%0.3f  ", i, *(mas + i));
    }

    print("\n");
}

// добавление элемента по индексу и сдвиг остальных вправо
void AddElement(float** mas, int *currentCount, int id, float newValue)
{    
    // проверяет что указатель и размен не NULL и что не отрицательный
    if (mas == NULL || currentCount == NULL || *currentCount < 0)
    {
        print("Некорректные аргументы\n");
        return;
    }

    // просто провкерка на согласованность 
    if (*currentCount > 0 && *mas == NULL) {
        printf("Некорректный указатель на массив\n");
        return;
    }

    // проверка вхождения id
    if (*currentCount == 0)
    {
        print("Массив полностью пуст, элемент будет добавлен в начало\n\n");
        id = 0;
    }
    else if (id < 0)
    {
        print("Выбранный id меньше 0, элемент будет добавлен в начало\n\n");
        id = 0;
    }
    else if (id > *currentCount)
    {
        print("Выбранный id больше количества элементов, элемент будет добавлен в конец\n\n");
        id = *currentCount;
    }

    // выделение памяти сработает даже если массив очищен
    float* tmp = realloc(*mas, sizeof **mas * (*currentCount + 1));
    if (tmp == NULL)
    {
        print("Ошибка выделения памяти\n");
        return;
    }

    // сдвиг значений вправо
    for (int i = *currentCount; i > id ; --i)
        *(tmp + i) = *(tmp + i - 1);

    *(tmp + id) = newValue;

    *mas = tmp;
    *currentCount = *currentCount + 1;

    print("\n");
}

// удаление последнего элемента с освобождением его памяти
void DeleteLast(float** mas, int* currentCount)
{
    if (mas == NULL || currentCount == NULL)
    {
        print("Некорректный указатель на массив\n");
        return;
    }

    if (*mas == NULL || *currentCount <= 0)
    {
        print("Массив уже пуст\n");
        return;
    }

    int newCount = *currentCount - 1;
    if (newCount == 0)
    {
        free(*mas);
        *mas = NULL;
        *currentCount = 0;

        print("Массив полностью пуст\n");
        return;
    }

    float* tmp = realloc(*mas, sizeof **mas * newCount);
    if (tmp == NULL)
    {
        print("Ошибка выделения памяти\n");

        return;
    }

    *mas = tmp;
    *currentCount = newCount;

    print("\n");
}

// QuickSort
void sortSwap(float* swap1, float* swap2)
{
    float tmp = *swap1;
    *swap1 = *swap2;
    *swap2 = tmp;
}

void _quickSort(float* mas, int startI, int endI)
{
    if (endI - startI < 2)
        return;

    int pivotIndex = startI + rand() % (endI - startI);
    float pivot = *(mas + pivotIndex);

    int less = startI;
    int current = startI;
    int greater = endI;

    // обход значений
    while (current < greater)
    {
        // если меньше помещается влево
        if (*(mas + current) < pivot)
        {
            sortSwap(mas + less, mas + current);
            less++;
            current++;
        }
        // если больше помещается вправо
        else if (*(mas + current) > pivot)
        {
            greater--;
            sortSwap(mas + greater, mas + current);
        }
        // если равен пропускается
        else
        {
            current++;
        }
    }

    _quickSort(mas, startI, less);
    _quickSort(mas, greater, endI);
}

void QuickSort(float* mas, int count)
{
    _quickSort(mas, 0, count);
}

void Zadanie1Main()
{
    srand((unsigned)time(NULL));

    print("         -------------\n");
    print("         | Задание 1 |\n");
    print("         -------------\n\n");

    // ввод размера массива
    int n = -1;
    while (1)
    {
        print("Введите размер массива:\n--> ");
        if (input("%d", &n))
        {
            print("\n");
            ClearBuffer();

            continue;
        }

        if (n <= 0)
        {
            print("Размер не может быть меньше 1\n\n");

            ClearBuffer();
            continue;
        }

        print("\n");
        ClearBuffer();
        break;
    }

    // создание массива
    float* mas = malloc(sizeof *mas * n);
    if (mas == NULL)    // ошибка выделения памяти
    {
        print("Ошибка выделения памяти\n");
        return;
    }

    // инициализация массива
    for (int i = 0; i < n; i++)
    {
        *(mas + i) = (float)(rand() % 200) - 100.0;
    }

    print("Массив проинициализирован от -100 до 100\n");

    print("\n");

    // основные функции
    int choice;
	while (1)
	{
		print("/-----------------------------------------------------------\\\n");
		print("| %d - Добавление элемента в указанную позицию (индексы с 0) |\n", z1_1);
        print("| %d - Удаление последнего элемента                          |\n", z1_2);
        print("| %d - Отображение значений элементов массива                |\n", z1_3);
        print("| %d - очистка массива                                       |\n", z1_4);
        print("| %d - поиск указанного пользователем элемента               |\n", z1_5);
        print("| %d - сортировка элементов массива по возрастанию           |\n", z1_6);
        print("| %d - Выход                                                 |\n", z1_off);
		print("\\-----------------------------------------------------------/\n\n");

        // ввод команды
        choice = -1;
        print("Введите команду:\n--> ");
        if (input("%d", &choice))
        {
            print("\n");
            ClearBuffer();
            continue;
        }

        print("\n");

        switch (choice)
        {
            // Добавление элемента в указанную позицию (индексы с 0)
            case z1_1:
            {
                print("Добавление элемента в указанную позицию (индексы с 0)\n");

                int id = -1;
                print("Введите id нового элемента в массиве:\n--> ");
                if (input("%d", &id))
                {
                    print("\n");
                    ClearBuffer();
                    continue;
                }

                float value = 0;
                print("\nВведите значение нового элемента в массиве:\n--> ");
                if (input("%f", &value))
                {
                    print("\n");
                    ClearBuffer();
                    continue;
                }

                AddElement(&mas, &n, id, value);

                print("\n");

                break;
            }
            // Удаление последнего элемента
            case z1_2:
                print("Удаление последнего элемента:\n");

                DeleteLast(&mas, &n);

                break;
            // Отображение значений элементов массива
            case z1_3:
                print("Отображение значений элементов массива:\n");

                PrintMas(mas, &n);

                print("\n");

                break;
            // очистка массива
            case z1_4:
                print("Очистка массива: ");

                free(mas);
                mas = NULL;
                n = 0;

                print("массив очищен!\n");

                print("\n");

                break;
            // поиск указанного пользователем элемента
            case z1_5:
            {
                float value = 0;
                print("\nВведите значение искомого элемента в массиве:\n--> ");
                if (input("%f", &value))
                {
                    print("\n");
                    ClearBuffer();
                    continue;
                }

                print("Поиск указанного элемента: ");

                for (int i = 0; i < n; i++)
                    if (*(mas + i) == value)
                        print("Элемент найден на позиции %d\n", i);

                print("\n");

                break;
            }
            // Cортировка элементов массива по возрастанию
            case z1_6:
            {
                print("сортировка элементов массива по возрастанию:\n");

                QuickSort(mas, n);

                print("\n");

                break;
            }
            // Cортировка элементов массива по возрастанию
            case z1_off:
                print("Выход\n\n");

                free(mas);

                return;
            default:
                print("Неверная команда\n\n");

                break;
        }
	}
}