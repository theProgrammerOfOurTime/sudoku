#pragma once
#include <set>
#include <string>
#include <iostream>

class Sudoku
{
private:
    int SIZE = 0;
    int** field = nullptr;
    int** fieldGroups = nullptr;
    int** initialField = nullptr;
    int depth = 0;
    bool used = false;
    struct cell
    {
        std::pair <int, int> positionXY;
        std::set <int> candidates;
        bool state = true;
    };
    struct group
    {
        bool* listNumber;
        int* quantityRepeat;
        cell* listCell;
        bool decided;
        int quantityCell;
    };
    group* square = nullptr;
    void initialization(int**&, int);
    void preparationCandidates(group&, int**);
    bool check(group&, int**);
    int searchCell(group&, int, int**);
    void initialValues(group&, int**);
    void copyField(int**, int**);
    bool repetitions(int**);
    void searchTheLastGroupAndCell(int&, int&);
    void transferOfValues(int**);
    void iterationOverValues(int**);
    void creatSudoku(int);
public:
    Sudoku() { }
    ~Sudoku()
    {
        delete[] field[0];
        delete[] field;
        delete[] fieldGroups[0];
        delete[] fieldGroups;
        for (int i = 0; i < SIZE; i++)
        {
            delete[] square[i].listNumber;
            delete[] square[i].quantityRepeat;
            delete[] square[i].listCell;
        }
        delete[] square;
    }
    bool readSudoku(int, std::string, std::string);
    bool readSudoku(int, int, int, std::string);
    friend std::ostream& operator << (std::ostream&, Sudoku&);
    void decide();
};