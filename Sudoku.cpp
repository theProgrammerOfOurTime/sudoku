#include "Sudoku.h"
#include <set>
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>

//вывод
std::ostream& operator<<(std::ostream& f, Sudoku& sudoku)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    for (int i = 0; i < sudoku.SIZE; i++)
    {
        for (int j = 0; j < sudoku.SIZE; j++)
        {
            if (sudoku.initialField[i][j] == 0)
            {
                if (sudoku.field[i][j] == 0)
                {
                    f << "   ";
                    continue;
                }
                else
                {
                    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 10));
                    f << sudoku.field[i][j] << "  ";
                    SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
                    continue;
                }
            }
            f << sudoku.field[i][j] << "  ";
        }
        f << '\t';
        for (int j = 0; j < sudoku.SIZE; j++)
        {
            std::cout << sudoku.fieldGroups[i][j] << "  ";
        }
        f << std::endl;
    }
    f << std::endl;
    return f;
}

//считывыет геометрическую судоку
bool Sudoku::readSudoku(int SIZE, std::string group, std::string sudoku)
{
    std::ifstream fileSudoku, fileGroup;
    fileSudoku.open(sudoku);
    fileGroup.open(group);
    if (!fileSudoku.is_open() || !fileGroup.is_open()) { return false; }
    creatSudoku(SIZE);
    for (int i = 0; i < SIZE; i++)
    {
        square[i].quantityCell = 0;
        square[i].decided = false;
        for (int j = 0; j < SIZE; j++)
        {
            fileSudoku >> field[i][j];
            initialField[i][j] = field[i][j];
            fileGroup >> fieldGroups[i][j];
            square[i].listNumber[j] = true;
            square[i].quantityRepeat[j] = 0;
        }
    }
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            square[this->fieldGroups[i][j]].listCell[square[this->fieldGroups[i][j]].quantityCell].positionXY.first = i;
            square[this->fieldGroups[i][j]].listCell[square[this->fieldGroups[i][j]].quantityCell].positionXY.second = j;
            if (this->field[i][j] != 0)
            {
                square[this->fieldGroups[i][j]].listCell[square[this->fieldGroups[i][j]].quantityCell].state = false;
                square[this->fieldGroups[i][j]].listNumber[this->field[i][j] - 1] = false;
            }
            square[this->fieldGroups[i][j]].quantityCell++;
        }
    }
    return true;
}

//считывает обычную судоку
bool Sudoku::readSudoku(int SIZE, int length, int height, std::string fileName)
{
    std::ifstream file;
    file.open(fileName);
    if (!file.is_open()) { return false; }
    creatSudoku(SIZE);
    for (int i = 0; i < SIZE; i++)
    {
        square[i].decided = false;
        for (int j = 0; j < SIZE; j++)
        {
            file >> field[i][j];
            initialField[i][j] = field[i][j];
            square[i].listCell[j].state = true;
            square[i].listNumber[j] = true;
            square[i].quantityRepeat[j] = 0;
        }
    }
    int x = 0, y = 0;
    for (int numberSquare = 0; numberSquare < SIZE; numberSquare++)
    {
        int numberCell = 0;
        for (int i = x; i < x + height; i++)
        {
            for (int j = y; j < y + length; j++)
            {
                square[numberSquare].listCell[numberCell].positionXY.first = i;
                square[numberSquare].listCell[numberCell].positionXY.second = j;
                if (this->field[i][j] != 0)
                {
                    square[numberSquare].listCell[numberCell].state = false;
                    square[numberSquare].listNumber[this->field[i][j] - 1] = false;
                }
                fieldGroups[i][j] = numberSquare;
                numberCell++;
            }
        }
        if (y != SIZE - length)
        {
            y += length;
        }
        else
        {
            y = 0;
            x += height;
        }
    }
    return true;
}

//освобождает и инициализирует память
void Sudoku::creatSudoku(int SIZE)
{
    if (used == true)
    {
        delete[] field[0];
        delete[] field;
        delete[] fieldGroups[0];
        delete[] fieldGroups;
        for (int i = 0; i < this->SIZE; i++)
        {
            delete[] square[i].listNumber;
            delete[] square[i].quantityRepeat;
            delete[] square[i].listCell;
        }
        delete[] square;
    }
    depth = 0;
    this->SIZE = SIZE;
    initialization(field, SIZE);
    initialization(fieldGroups, SIZE);
    initialization(initialField, SIZE);
    square = new group[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        square[i].listNumber = new bool[SIZE];
        square[i].quantityRepeat = new int[SIZE];
        square[i].listCell = new cell[SIZE];
    }
    used = true;
    return;
}

//инициализирует массив
void Sudoku::initialization(int**& field, int size)
{
    field = new int* [size];
    field[0] = new int[size * size];
    for (int i = 1; i < size; i++)
    {
        field[i] = field[0] + i * size;
    }
    return;
}

//находит кандидатов для клеток группы
void Sudoku::preparationCandidates(group& a, int** Field)
{
    bool decided = true;
    for (int i = 0; i < SIZE; i++)
    {
        if (a.listNumber[i])
        {
            decided = false;
            break;
        }
    }
    if (a.decided = decided)//this is ok
    {
        return;
    }
    for (int numberCell = 0; numberCell < SIZE; numberCell++)
    {
        if (a.listCell[numberCell].state == true)
        {
            a.listCell[numberCell].candidates.clear();
            bool* candidat = new bool[SIZE];
            for (int i = 0; i < SIZE; i++)
            {
                candidat[i] = true;
            }
            for (int i = 0; i < SIZE; i++)
            {
                if (Field[i][a.listCell[numberCell].positionXY.second] != 0)
                {
                    candidat[Field[i][a.listCell[numberCell].positionXY.second] - 1] = false;
                }
            }
            for (int i = 0; i < SIZE; i++)
            {
                if (Field[a.listCell[numberCell].positionXY.first][i] != 0)
                {
                    candidat[Field[a.listCell[numberCell].positionXY.first][i] - 1] = false;
                }
            }
            for (int i = 0; i < SIZE; i++)
            {
                if (candidat[i] == true && a.listNumber[i] == true)
                {
                    a.listCell[numberCell].candidates.insert(i + 1);
                    a.quantityRepeat[i]++;
                }
            }
            delete[] candidat;
        }
    }
    return;
}

//подставляет одиночных кандидатов(метод синглов)
bool Sudoku::check(group& a, int** Field)
{
    bool move = false;
    for (int i = 0; i < SIZE; i++)
    {
        if (a.quantityRepeat[i] == 1)
        {
            for (int numberCell = 0; numberCell < SIZE; numberCell++)
            {
                if (a.listCell[numberCell].state == true)
                {
                    if (a.listCell[numberCell].candidates.find(i + 1) != a.listCell[numberCell].candidates.end())
                    {
                        move = true;
                        a.listNumber[i] = false;
                        Field[a.listCell[numberCell].positionXY.first][a.listCell[numberCell].positionXY.second] = i + 1;
                        a.listCell[numberCell].state = false;
                        break;
                    }
                }
            }
        }
        a.quantityRepeat[i] = 0;
    }
    for (int numberCell = 0; numberCell < SIZE; numberCell++)
    {
        if (a.listCell[numberCell].state == true)
        {
            if (a.listCell[numberCell].candidates.size() == 1)
            {
                move = true;
                a.listNumber[*(a.listCell[numberCell].candidates.begin()) - 1] = false;
                Field[a.listCell[numberCell].positionXY.first][a.listCell[numberCell].positionXY.second] = *(a.listCell[numberCell].candidates.begin());
                a.listCell[numberCell].state = false;
            }
        }
    }
    return move;
}

//возвращвет позицию первой свободной клетки в группе, если такой нет - -1
int Sudoku::searchCell(group& a, int n, int** initialField)
{
    for (int i = n + 1; i < SIZE; i++)
    {
        if (initialField[a.listCell[i].positionXY.first][a.listCell[i].positionXY.second] == 0)
        {
            return i;
        }
    }
    return -1;
}

//откатывает знвчения до предыдущих(в переборе с возвратом используется одна основная структура данных)
void Sudoku::initialValues(group& a, int** initialField)
{
    bool* list = new bool[SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        list[i] = true;
    }
    for (int i = 0; i < SIZE; i++)
    {
        if (initialField[a.listCell[i].positionXY.first][a.listCell[i].positionXY.second] == 0)
        {
            a.listCell[i].state = true;
            a.decided = false;
        }
        else
        {
            list[initialField[a.listCell[i].positionXY.first][a.listCell[i].positionXY.second] - 1] = false;
        }
    }
    for (int i = 0; i < SIZE; i++)
    {
        a.listNumber[i] = list[i];
    }
    preparationCandidates(a, initialField);
    delete[] list;
    return;
}

//копирует значения из одного массива в другой
void Sudoku::copyField(int** copyedField, int** initialField)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            copyedField[i][j] = initialField[i][j];
        }
    }
    return;
}


//метод синглов всегда подставляет одиночных кандидатов. Данный метод даёт верификацию, что значение в клетке не встечается
//в столбце и строке(в группе проверять не надо, там значения не могут повторятся из-за структуры данных)
bool Sudoku::repetitions(int** Field)
{
    for (int h = 0; h < SIZE; h++)
    {
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (Field[i][h] != 0)
                {
                    if (Field[i][h] == Field[i][j] && h != j)
                    {
                        return true;
                    }
                }
            }
            for (int j = 0; j < SIZE; j++)
            {
                if (Field[h][i] != 0)
                {
                    if (Field[h][i] == Field[j][i] && h != j)
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

//находит последнюю не заполненную клетку и её групп(индетификация решения)
void Sudoku::searchTheLastGroupAndCell(int& Group, int& Cell)
{
    for (int numberGroup = SIZE - 1; numberGroup >= 0; numberGroup--)
    {
        if (!square[numberGroup].decided)
        {
            for (int numberCell = SIZE - 1; numberCell >= 0; numberCell--)
            {
                if (square[numberGroup].listCell[numberCell].state)
                {
                    Group = numberGroup;
                    Cell = numberCell;
                    return;
                }
            }
        }
    }
    return;
}

//перенос решения судоку в основной масств
void Sudoku::transferOfValues(int** field)
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            this->field[i][j] = field[i][j];
        }
    }
    return;
}

//перебор с возвратом
void Sudoku::iterationOverValues(int** initialField)
{
    static bool decided;
    decided = false;
    depth++;
    int numberCell = -1, numberSquare = 0, theLastCell, theLastGroup;
    searchTheLastGroupAndCell(theLastGroup, theLastCell);
    int** fieldSecond;
    initialization(fieldSecond, SIZE);
    bool end = false;
    while (!end)
    {
        for (numberSquare = numberSquare; numberSquare < SIZE; numberSquare++)
        {
            if (!square[numberSquare].decided)
            {
                numberCell = searchCell(square[numberSquare], numberCell, initialField);
                if (numberCell == -1)
                {
                    continue;
                }
                break;
            }
        }
        std::set <int> tempSet = square[numberSquare].listCell[numberCell].candidates;//preparationCandidates изменяет множества кандидатов
        for (auto it = tempSet.begin(); it != tempSet.end() && !end; it++)
        {
            int quantityZero = SIZE * SIZE;
            copyField(fieldSecond, initialField);
            fieldSecond[square[numberSquare].listCell[numberCell].positionXY.first][square[numberSquare].listCell[numberCell].positionXY.second] = *(it);
            square[numberSquare].listCell[numberCell].state = false;
            square[numberSquare].listNumber[*(it)-1] = false;
            bool move = true;
            while (move != false)
            {
                move = false;
                for (int numberSquare = 0; numberSquare < SIZE; numberSquare++)
                {
                    if (!square[numberSquare].decided)
                    {
                        preparationCandidates(square[numberSquare], fieldSecond);
                        move = move || check(square[numberSquare], fieldSecond);
                    }
                }
            }
            if (repetitions(fieldSecond))
            {
                if (depth != 1)
                {
                    delete[] fieldSecond[0];
                    delete[] fieldSecond;
                    depth--;
                    return;
                }
            }
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    if (fieldSecond[i][j] != 0)
                    {
                        quantityZero--;
                    }
                }
            }
            if (quantityZero == 0)//решение найдено
            {
                transferOfValues(fieldSecond);
                delete[] fieldSecond[0];
                delete[] fieldSecond;
                decided = true;
                return;
            }
            bool emptyСells = false;//в каждой пустой клетке есть хотя бы один кандидат
            for (int numberSquare = 0; numberSquare < SIZE; numberSquare++)
            {
                if (!square[numberSquare].decided)
                {
                    for (int numberCell = 0; numberCell < SIZE; numberCell++)
                    {
                        if (square[numberSquare].listCell[numberCell].state == true)
                        {
                            if (square[numberSquare].listCell[numberCell].candidates.size() == 0)
                            {
                                emptyСells = true;
                                break;
                            }
                        }
                    }
                }
                if (emptyСells) { break; }
            }
            if (emptyСells == false)
            {
                iterationOverValues(fieldSecond);
                if (decided == true)
                {
                    delete[] fieldSecond[0];
                    delete[] fieldSecond;
                    return;
                }
            }
            for (int i = 0; i < SIZE; i++)
            {
                initialValues(square[i], initialField);
            }
            end = (quantityZero == 0 || (numberCell == theLastCell && numberSquare == theLastGroup));
        }
    }
    if (depth != 1)
    {
        delete[] fieldSecond[0];
        delete[] fieldSecond;
        depth--;
        return;
    }
    transferOfValues(fieldSecond);
    delete[] fieldSecond[0];
    delete[] fieldSecond;
    return;
}

//сначала пробуем найти решение с помощью метода синглов, в случае если его не будет пребегаем к перебору с возвратом
void Sudoku::decide()
{
    bool move = true;
    while (move != false)
    {
        move = false;
        for (int numberSquare = 0; numberSquare < SIZE; numberSquare++)
        {
            if (!square[numberSquare].decided)
            {
                preparationCandidates(square[numberSquare], field);
                move = move || check(square[numberSquare], field);
            }
        }
    }
    int quantityZero = SIZE * SIZE;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (field[i][j] != 0)
            {
                quantityZero--;
            }
        }
    }
    if (quantityZero != 0)
    {
        iterationOverValues(field);
    }
    return;
}