#include <iostream>
#include <ctime>
#include<fstream>
using namespace std;
 
int rule[3][9][9], sudoku[9][9], flag[9][9];
 
int findSmallIndex(int rowIndex, int colIndex) {
    return (3 * (rowIndex / 3) + (colIndex / 3));
}
 
int findNextValue(int rowIndex, int colIndex, int start) {
    int index = findSmallIndex(rowIndex, colIndex);
    for (int i = start; i < 9; i++) {
        if (rule[0][rowIndex][i]) continue;
        if (rule[1][colIndex][i]) continue;
        if (rule[2][index][i]) continue;
        return i;
    }
    return -1;
}
 
void changeRule(int rowIndex, int colIndex, int style) {
    int index = findSmallIndex(rowIndex, colIndex);
    int value = sudoku[rowIndex][colIndex];
    rule[0][rowIndex][value] = style;
    rule[1][colIndex][value] = style;
    rule[2][index][value] = style;
}
 
void initSudoku() {
	ifstream in("sudoku.txt");
	if (! in)
	{
		cout << "open sudoku.txt fail\n";
		exit(1);
	}
    for (int i = 0; i < 9; i++) {
		char buffer[100];
		in.getline(buffer, 100);
        for (int j = 0; j < 9; j++) {
            sudoku[i][j] = buffer[j] - '0' - 1;
            flag[i][j] = ~sudoku[i][j] ? 0 : 1;
            if (!flag[i][j]) changeRule(i, j, 1);
        }
    }
}
 
void show() {
    for (int i = 0; i < 9; i++) {
        string line = "";
        for (int j = 0; j < 9; j++) {
            line += '0' + sudoku[i][j] + 1;
        }
        cout << line << endl;
    }
}
 
void solve(int rowIndex, int colIndex) {
    if (colIndex == 9) {rowIndex++;colIndex = 0;}
    while (rowIndex <= 8 && !flag[rowIndex][colIndex]) {
        colIndex++;if (colIndex == 9) {rowIndex++;colIndex = 0;}
    }
    if (rowIndex > 8) {show();return;}
    while (~(sudoku[rowIndex][colIndex] = findNextValue(rowIndex, colIndex, sudoku[rowIndex][colIndex] + 1))) {
        changeRule(rowIndex, colIndex, 1);
        solve(rowIndex, colIndex + 1);
        changeRule(rowIndex, colIndex, 0);
    }
}
 
int main() {
    long time1, time2;
    initSudoku();
    time1 = clock();
    solve(0, 0);//从(0,0)位置开始填数
    time2 = clock();
    cout << "求解过程共用" << time2 - time1 << "毫秒" << endl;
    return 0;
}
