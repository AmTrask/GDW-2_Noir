//Josh Trask
//100705771
#include <iostream>


//global variables
int tempRow;
int tempColumn;
char board[5][5] = 
{
{'a','b','c','d','e'},
{'f','g','h','i','j'}, 
{'k','l','m','n','o'}, 
{'p','q','r','s','t'}, 
{'u','v','w','x','y'}
};

//cycles through board, if the characters match then temporary values are set for the row and column.
void findPos(char character)
{
	for (int row = 0; row < 5; row++)
	{
		for (int column = 0; column < 5; column++)
		{
			//std::cout << board[row][column]<< character << std::endl;
			if (board[row][column] == character)
			{
				//std::cout << "\n yes \n";
				tempRow = row;
				tempColumn = column;
			}
		}
		
	}
}

//shifts a column upwards
void shift_up(int column)
{
	char temp = board[0][column];
	
	for (int i = 0; i < 4; i++)
	{
		board[i][column] = board[i+1][column];
	}
	board[4][column] = temp;
}

//shifts a column downwards
void shift_down(int column)
{
	char temp = board[4][column];

	for (int i = 4; i > 0; i--)
	{
		board[i][column] = board[i - 1][column];
	}
	board[0][column] = temp;
}

//shifts a row left
void shift_left(int row)
{
	char temp = board[row][0];

	for (int i = 0; i < 4; i++)
	{
		board[row][i] = board[row][i+1];
	}
	board[row][4] = temp;
}

//shifts a row right
void shift_right(int row)
{
	char temp = board[row][4];

	for (int i = 4; i > 0; i--)
	{
		board[row][i] = board[row][i - 1];
	}
	board[row][0] = temp;
}

//draws board in console
void draw()
{
	for (int row = 0; row < 5; row++)
	{
		for (int column = 0; column < 5; column++)
		{
			std::cout << "   " << board[row][column];
		}
		std::cout << "\n\n";
	}
}

//checks to see if 2 characters are adjacent
bool isAdj(char char1, char char2)
{
	findPos(char1);
	int pos1[2] = { tempRow,tempColumn };
	findPos(char2);
	int pos2[2] = { tempRow,tempColumn };

	//std::cout << pos1[1] << "     " << pos2[1] << "\n";
	if (((pos1[0] == pos2[0] || pos1[0] == pos2[0] + 1) || (pos1[0] == pos2[0] - 1)) && ((pos1[1] == pos2[1] || pos1[1] == pos2[1] + 1) || (pos1[1] == pos2[1] - 1)))
	{
		return true;
	}
	else
	{
		return false;
	}
}


int main()
{
	//std::cout << board[0]<< std::endl;
	shift_right(0);
	draw();
	std::cout << isAdj('a', 'b') << std::endl;

	system("pause");
	return 0;
}