/*

    Вариант № 5
    2. Определить целочисленную динамическую матрицу. Найти в каждой нечетной строке матрицы элемент с минимальным по модулю значением и позицию этого элемента.
    Программу представить в двух вариантах:
    1) определить матрицу с использованием обычного указателя;
    2) определить матрицу с использованием двойного указателя.

    ВАЖНО при выполнении заданий 1-3: для обращения к элементам массива использовать операцию разыменования (нельзя использовать [ ]);
    при добавлении и удалении информации грамотно работать с памятью

*/

#include "Zadanie2.h"

typedef enum Zadanie2Menu
{
    z2_off,
    z2_1,
    z2_2
} Zadanie2Menu;

void var1()
{
    int rows = -1;
    int cols = -1;

    while (1)
    {
        // ввод строк
        print("Введите кол-во строк:\n--> ");
        if (input("%d", &rows))
        {
            print("\n");
            ClearBuffer();
            continue;
        }

        print("\n");

        // ввод столбцов
        print("Введите кол-во столбцов:\n--> ");
        if (input("%d", &cols))
        {
            print("\n");
            ClearBuffer();
            continue;
        }

        if (rows <= 0 || cols <= 0) {
            print("Размеры должны быть положительными\n");
            continue;
        }

        break;
    }

    int* matrix = malloc(sizeof *matrix * rows * cols);
    if (matrix == NULL)    // ошибка выделения памяти
    {
        print("Ошибка выделения памяти\n");
        return;
    }

    for (int i = 0; i < rows * cols; i++)
        *(matrix + i) = (rand() % 200) - 100;

    print("\nМатрица:\n");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            print( "| %4d ", *(matrix + i * cols + j) );
        }

        print("|\n");
    }

    print("\n");

    for (int i = 0; i < rows; i++)
    {
        if ((i + 1) % 2 != 0)
        {
            int minItem = *(matrix + i * cols);
            int index = 0;

            for (int j = 0; j < cols; j++)
            {
                int currentItem = *(matrix + i * cols + j);

                if (abs(currentItem) < abs(minItem))
                {
                    minItem = currentItem;
                    index = j;
                }
            }

            print("Минимум в строке %d, на позиции %d: %d\n", i + 1, index, minItem);
        }
    }

    free(matrix);
}

void var2()
{
    int rows = -1;
    int cols = -1;

    while (1)
    {
        // ввод строк
        print("Введите кол-во строк:\n--> ");
        if (input("%d", &rows))
        {
            print("\n");
            ClearBuffer();
            continue;
        }

        print("\n");

        // ввод столбцов
        print("Введите кол-во столбцов:\n--> ");
        if (input("%d", &cols))
        {
            print("\n");
            ClearBuffer();
            continue;
        }

        if (rows <= 0 || cols <= 0) {
            print("Размеры должны быть положительными\n");
            continue;
        }

        break;
    }

    int** matrix = malloc(sizeof *matrix * rows);
    if (matrix == NULL)    // ошибка выделения памяти
    {
        print("Ошибка выделения памяти\n");
        return;
    }

    for (int i = 0; i < rows; i++)
    {
        *(matrix + i) = malloc(sizeof **matrix * cols);

        // в случае ошибки очищается всё
        if (*(matrix + i) == NULL)
        {
            print("Ошибка выделения памяти\n");

            for (int j = 0; j < i; j++)
                free(*(matrix + j));
            
            free(matrix);

            return;
        }

        for (int j = 0; j < cols; j++)
            *(*(matrix + i) + j) = (rand() % 200) - 100;
    }

    print("\nМатрица:\n");

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            print("| %4d ", *(*(matrix + i) + j));
        }

        print("|\n");
    }

    print("\n");

    for (int i = 0; i < rows; i++)
    {
        if ((i + 1) % 2 != 0)
        {
            int minItem = **(matrix + i);
            int index = 0;

            for (int j = 0; j < cols; j++)
            {
                int currentItem = *(*(matrix + i) + j);

                if (abs(currentItem) < abs(minItem))
                {
                    minItem = currentItem;
                    index = j;
                }
            }

            print("Минимум в строке %d, на позиции %d: %d\n", i + 1, index, minItem);
        }
    }

    for (int i = 0; i < rows; i++)
        free(*(matrix + i));

    free(matrix);
}

void Zadanie2Main()
{
    srand((unsigned)time(NULL));

    print("         -------------\n");
    print("         | Задание 2 |\n");
    print("         -------------\n\n");

    // основные функции
    int choice;
    while (1)
    {
        print("/---------------\\\n");
        print("| %d - Вариант 1 |\n", z2_1);
        print("| %d - Вариант 2 |\n", z2_2);
        print("| %d - Выход     |\n", z2_off);
        print("\\---------------/\n\n");

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
            // 1 вариант
            case z2_1:
                print("Вариант 1\n\n ");

                var1();

                break;
            // 2 вариант
            case z2_2:
                print("Вариант 1\n\n ");

                var2();

                break;
            // 2 вариант
            case z2_off:
                print("Выход\n ");

                return;
            default:
                print("Неверная команда\n\n");

                break;
        }
    }
}