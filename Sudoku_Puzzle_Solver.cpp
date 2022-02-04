/********************************************************************
Course		:	CO2210 Programming Quest
Quest No	:	01
Title		:	Sudoku Puzzle Solver
Author		:	Kasthuriarachchi K.A.D.G.
Index No	:	19/ENG/053
Date		:	06-09-2021
********************************************************************/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <stack>
#include <fstream>

using namespace std;

//declaring the size of the puzzle and initializing two 2D arrays with 0s
const int puzzle_size = 9;
int sudoku_puzzle[puzzle_size][puzzle_size] = { 0 };
int sudoku_puzzle_copy[puzzle_size][puzzle_size] = { 0 };

//structure to create point objects with their coordinates
struct Point
{
	int x, y;
	Point(int a = 0, int b = 0) :x(a), y(b) {}
};

//stack of Points to store changed points of the puzzle
stack<Point> changed_points;

//initializing the number of steps taken to sole the sudoku puzzle with zero
int step_count = 0;

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
Function            :	read_puzzle
Description         :	reading the puzzle from the text file "Sudoku_Puzzle.txt"
Input Parameters    :	-
Return              :	-
---------------------------------------------------------------------------------*/
void read_puzzle()
{
	ifstream puzzle_file;
	puzzle_file.open("Sudoku_Puzzle.txt");

	for (int h = 0; h < puzzle_size; h++)
	{
		for (int w = 0; w < puzzle_size; w++)
			puzzle_file >> sudoku_puzzle_copy[h][w];
	}
	cout << "Puzzle saved in \"Sudoku_Puzzle.txt\"\n\n";
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
Function            :	copy_puzzle
Description         :	copying the puzzle from the already availiable copy of puzzle
Input Parameters    :	-
Return              :	-
---------------------------------------------------------------------------------*/
void copy_puzzle()
{
	for (int i = 0; i < puzzle_size; i++)
	{
		for (int j = 0; j < puzzle_size; j++)
			sudoku_puzzle[i][j] = sudoku_puzzle_copy[i][j];
	}
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
Function            :	sudoku_solver
Description         :	replacing random values that can be placed on a cell and 
						printing the puzzle step by step with indicating replaced
						cell in each step
Input Parameters    :	-
Return              :	boolean value which denotes whether the puzzle is fully
						replaced and solved or not
---------------------------------------------------------------------------------*/
bool sudoku_solver()
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

				//checking the availability of numbers
				if (num.size() > 0)
				{
					//assigning a random cell as an available value
					sudoku_puzzle[i][j] = num[random_number(0, num.size() - 1)];

					//storing changed position and increasing count
					changed_points.push(Point(i, j));
					step_count++;

					//printing particular step number and printing puzzle after replacing
					cout << "Step " << step_count << endl;
					for (int m = 0; m < puzzle_size; m++)
					{
						for (int n = 0; n < puzzle_size; n++)
						{
							if (m == i && n == j)								
								cout << "(" << sudoku_puzzle[m][n] << ") ";
							else
								cout << sudoku_puzzle[m][n] << " ";
						}
						cout << endl;
					}
					cout << endl;
				}
				else
					return false;
			}
		}
	}
	return true;
}

int main()
{
	srand((unsigned)time(NULL));

	cout << "Sudoku Puzzle Solver\n\n";

	read_puzzle();
	copy_puzzle();

	print_puzzle();
	cout << "\n\nSteps (last changed digit is in the brackets)\n\n";

	//condition denotes whether the sudoku puzzle is solved or not
	bool condition = false;

	//option is the input which is the number of steps that puzzle is going back
	//or zero if the puzzle is resetted with the initial sudoku puzzle
	int option = 0;

	//trying until a correctly solved puzzle is generated
	while (condition == false)
	{
		condition = sudoku_solver();
		if (condition == false)
		{

			do
			{
				//displaying the number of steps completed and the error message when the puzzle is stucked with no numbers to be replaced
				cout << step_count << " steps completed.\nSolving has stucked! No numbers to to be replaced. What do you want to do?";
				
				//asking for the start over or the input of the number of steps to go backward
				cout<<"\nEnter zero to start from the begining or Enter a number to go that much steps backward : ";
				cin >> option;
				cout << endl;

			//taking the input while entering a correct value
			} while (step_count < option);
			
			//when start over is choosen resetting the puzzle to the initial puzzle and setting steps to zero
			if (option == 0)
			{
				copy_puzzle();
				step_count = 0;
			}
			//if a number of steps are entered, going backward for that much of steps
			else
			{
				for (int i = 0; i < option; i++)
				{
					//making the last step backward and removing that point from the stack and decreasing step count
					sudoku_puzzle[changed_points.top().x][changed_points.top().y] = 0;
					changed_points.pop();
					step_count--;
				}
			}
		}
	}

	cout << "\nSolve Sudoku Puzzle\n\n";
	print_puzzle();

	return 0;
}