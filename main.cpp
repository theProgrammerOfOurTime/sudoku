#include <iostream>
#include "Sudoku.h"

using namespace std;

int main()
{
    Sudoku sudoku;
    sudoku.readSudoku(9, 3, 3, "tests\\regularSudoku1.txt");
    sudoku.decide();
    cout << sudoku;
    sudoku.readSudoku(6, 3, 2, "tests\\regularSudoku2.txt");
    sudoku.decide();
    cout << sudoku;
    sudoku.readSudoku(25, 5, 5, "tests\\regularSudoku3.txt");
    sudoku.decide();
    cout << sudoku;
    sudoku.readSudoku(12, "tests\\geometricSudokuGroup2.txt", "tests\\geometricSudoku2.txt");
    sudoku.decide();
    cout << sudoku;
    return 0;
}
