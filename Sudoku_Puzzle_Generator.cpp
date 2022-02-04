/********************************************************************
Course		:	CO2210 Programming Quest
Quest No	:	01
Title		:	Sudoku Puzzle Generator
Author		:	Kasthuriarachchi K.A.D.G.
Index No	:	19/ENG/053
Date		:	06-09-2021
********************************************************************/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <fstream>

using namespace std;

//declaring the size of the puzzle and initializing an 2D array with 0s
const int puzzle_size = 9;
int sudoku_puzzle[puzzle_size][puzzle_size] = {0};

/*---------------------------------------------------------------------------------
Function            :	random_number
Description         :	generating a random number within a given inclusive range
Input Parameters    :	min - lower limit of the random number
						max - upper limit of the random number
Return              :	generated random number 
---------------------------------------------------------------------------------*/
int random_number(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

/*---------------------------------------------------------------------------------
Function            :	print_puzzle
Description         :	printing the puzzle
Input Parameters    :	-
Return              :	-
---------------------------------------------------------------------------------*/
void print_puzzle()
{
	for (int i = 0; i < puzzle_size; i++)
	{
		for (int j = 0; j < puzzle_size; j++)
			cout << sudoku_puzzle[i][j] << " ";
		cout << endl;
	}
}

/*---------------------------------------------------------------------------------
Function            :	set_zero_puzzle
Description         :	setting all the values of the puzzle to zero
Input Parameters    :	-
Return              :	-
---------------------------------------------------------------------------------*/
void set_zero_puzzle()
{
	for (int i = 0; i < puzzle_size; i++)
	{
		for (int j = 0; j < puzzle_size; j++)
			sudoku_puzzle[i][j] = 0;
	}
}

/*---------------------------------------------------------------------------------
Function            :	save_puzzle
Description         :	saving the puzzle to a text file "Sudoku_Puzzle.txt"
Input Parameters    :	-
Return              :	-
---------------------------------------------------------------------------------*/
void save_puzzle()
{
	ofstream puzzle_file;
	puzzle_file.open("Sudoku_Puzzle.txt");
	for (int h = 0; h < puzzle_size; h++)
	{
		for (int w = 0; w < puzzle_size; w++)
			puzzle_file << sudoku_puzzle[h][w] << " ";
		puzzle_file << endl;
	}
	cout << "\nPuzzle saved to \"Sudoku_Puzzle.txt\"\n";
}

/*---------------------------------------------------------------------------------
Function            :	available_nums
Description         :	finding the values that is possible to be placed in a cell
Input Parameters    :	x - x coordinate
						y - y coordinate
Return              :	a vector of the numbers that can be placed on a given cell
---------------------------------------------------------------------------------*/
vector<int> available_nums(int x, int y)
{
	//initializing the vector with all the numbers
	vector<int> nums = { 1,2,3,4,5,6,7,8,9 };

	//replacing values that are in the given cell's row or column with zeros
	for (int i = 0; i < puzzle_size; i++)
	{
		if (sudoku_puzzle[i][y] > 0 && sudoku_puzzle[i][y] < 10)
			nums[sudoku_puzzle[i][y] - 1] = 0;
		if (sudoku_puzzle[x][i] > 0 && sudoku_puzzle[x][i] < 10)
			nums[sudoku_puzzle[x][i] - 1] = 0;
	}

	//initializing the starting points of the sub grids
	int m = 0;
	int n = 0;

	//setting the values of subgrids according to the given cell's values
	if (x < 3) m = 0;
	else if (x < 6) m = 3;
	else if (x < 9) m = 6;

	if (y < 3) n = 0;
	else if (y < 6) n = 3;
	else if (y < 9) n = 6;

	//checking inside the sub grid for already existing values and replacing them with zeros
	for (int i = m; i < (m + 3); i++)
	{
		for (int j = n; j < (n + 3); j++)
		{
			if (sudoku_puzzle[i][j] > 0 && sudoku_puzzle[i][j] < 10)
				nums[sudoku_puzzle[i][j] - 1] = 0;
		}
	}

	//initializing a new vector to return only non-zero values
	vector<int> avail_nums;

	//assigning non-zero values to new vector
	for (int i = 0; i < nums.size(); i++)
	{
		if (nums[i] > 0 && nums[i] < 10)
			avail_nums.push_back(nums[i]);
	}

	return avail_nums;
}

/*---------------------------------------------------------------------------------
Function            :	sudoku_generator
Description         :	replacing random values that can be placed on a cell and
						checking the corectness
Input Parameters    :	-
Return              :	boolean value which denotes whether the puzzle is fully 
						replaced with values or not
---------------------------------------------------------------------------------*/
bool sudoku_generator()
{
	//two loops for checking all the elements of the puzzle
	for (int i = 0; i < puzzle_size; i++)
	{
		for (int j = 0; j < puzzle_size; j++)
		{
			//choosing non filled values
			if (sudoku_puzzle[i][j] == 0)
			{
				//taking available numbers for particular location
				vector<int> num = available_nums(i, j);

				//checking the availability of numbers and replace if availiable
				if (num.size() > 0)
					sudoku_puzzle[i][j] = num[random_number(0, num.size() - 1)];
			}
		}
	}

	//checking the puzzle for empty cells
	for (int i = 0; i < puzzle_size; i++)
	{
		for (int j = 0; j < puzzle_size; j++)
		{
			if (sudoku_puzzle[i][j] == 0)
				return false;
		}
	}
	return true;
}

/*---------------------------------------------------------------------------------
Function            :	cell_remover
Description         :	replacing a given number of random positions of a column 
						and a row with zeros
Input Parameters    :	n - maximum number of positions to replaced in a particular 
							row and a column
Return              :	-
---------------------------------------------------------------------------------*/
void cell_remover(int n)
{
	//array to store number of column changed to zero
	int count[9] = { 0 };

	//two loops for checking all the elements of the puzzle
	for (int i = 0; i < puzzle_size; i++)
	{
		for (int j = 0; j < n; j++)
		{
			//initializing random number
			int rand;
			
			//searching a random position not filled more than the given value 
			do
			{
				rand = random_number(0, puzzle_size - 1);
			} while (count[rand] >= n);

			//replacing that random position with zero
			sudoku_puzzle[i][rand] = 0;

			//increasing the count of particular column
			count[rand]++;
		}
	}
}

int main()
{
	srand((unsigned)time(NULL));
	
	cout << "Sudoku Puzzle Generator\n\n";
	
	//taking input(n) for the number of cells to be removed from the sudoku puzzle
	int n;
	do
	{
		cout << "Enter the number of missing cells (2 to 4): ";
		cin >> n;
	} while (n < 2 || n > 4);

	//condition denotes the correctness of the sudoku puzzle
	//whether it is fully placed with values or not
	bool condition = false;
	
	//generating puzzles until it finds a correct puzzle
	while (condition == false)
	{
		condition = sudoku_generator();
		//if the puzzle is not fully placed with values, it is resetted with zeros
		if (condition == false)
			set_zero_puzzle();
	}
	
	cell_remover(n);
	cout << endl;
	print_puzzle();
	save_puzzle();

	return 0;
}