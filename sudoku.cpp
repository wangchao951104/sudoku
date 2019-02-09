#include<iostream>
#include<ctime>
#include<fstream>
using namespace std;

bool rule[3][9][9];		//rule[3][row/col/palace][value],检查每个数字在每行，每列，每宫是否存在

int sudoku[9][9];		//数独存储

bool flag[9][9];		//初始数独每个格子是否为空格，若为空格，则flag为false，随着数独填数，flag不变

void changeRule(int row, int col, bool style);		//函数声明

void initSudoku()		//初始化数独
{
	ifstream in("sudoku.txt");
	if(! in)
	{
		cout << "open sudoku.txt fail!\n";
		exit(1);
	}
	for (int i = 0; i < 9; i++)
	{	
		char buffer[100];
		in.getline(buffer, 100);
		for (int j = 0; j < 9; j++)
		{
			sudoku[i][j] = buffer[j] - '0' - 1;
			flag[i][j] = ~sudoku[i][j] ? 1 : 0;
			if (flag[i][j])
			  changeRule(i, j, 1);
			else
			  changeRule(i, j, 0);
		}
	}
}

void show()			//输出数独
{
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		  cout << sudoku[i][j]+1;
		cout << endl;
	}
}

int findPalace(int row, int col)		//找到该空格所在的宫
{
	return (3 * (row / 3) + (col / 3));
}

int findNextValue(int row, int col, int start)		//找到空格中下一个可填数字
{
	int palace = findPalace(row, col);
	for (int i = start; i < 9; i++)			//从小到大找，超过9后，返回-1，又从0开始找下一个数
	{
		if(rule[0][row][i] || rule[1][col][i] || rule[2][palace][i])
		  continue;
		bool f1 = false;
		if (row == col || row + col == 8)		//对角线上不能存在相同数字
		{
			for (int j = 0; j < 9; j++)
			{
				if (row == col)
				{
					if (sudoku[j][j] == i)
					{
						f1 = true;
						break;
					}
				}
				if (row + col == 8)
				{
					if (sudoku[j][8-j] == i)
					{
						f1 = true;
						break;
					}
				}
			}
			if (f1)
			  continue;
		}
		return i;
	}
	return -1;		//没找到下一个可填数字，返回-1，将sudoku该空格重置为0
}

void changeRule(int row, int col, bool style)		//将sudoku[row][col]在行，列，宫中定为在或不在
{
	int palace = findPalace(row,col);
	int value = sudoku[row][col];
	rule[0][row][value] = style;
	rule[1][col][value] = style;
	rule[2][palace][value] = style;
}

void solve(int row, int col)		//从row, col开始解
{
	if (col == 9)
	{
		row++;
		col = 0;
	}
	while (row < 9 && flag[row][col])
	{
		col++;
		if (col == 9)
		{
			row++;
			col = 0;
		}
	}
	if (row == 9)
	{
		cout << "sudoku\n";
		show();
		return;
	}
	while (~(sudoku[row][col] = findNextValue(row, col, sudoku[row][col]+1)))
	{
		changeRule(row, col, 1);
		solve(row, col+1);
		changeRule(row, col, 0);
	}
}

int main()
{
	long time1, time2;
	initSudoku();
	time1 = clock();
	solve(0,0);
	time2 = clock();
	cout << "求解过程共用" << time2-time1 << "毫秒" << endl;
	return 0;
}

