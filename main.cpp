//Josh Trask
//100705771

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <cwchar>

//global variables
int tempRow;
int tempColumn;
bool runGame = true;
char dead[];
char board[5][5] = 
{
{'a','b','c','d','e'},
{'f','g','h','i','j'}, 
{'k','l','m','n','o'}, 
{'p','q','r','s','t'}, 
{'u','v','w','x','y'}
};

//checks to see if char is in string
bool contain(std::string list, char letter)
{
	for (int i = 0; i < list.length(); i++)
	{
		if (list[i] == letter)
		{
			return true;
		}
	}
	return false;
}

bool isDead(char letter)
{
	//if ()
	return false;
}
//ends game
void GameEnd(char det, char kil)
{
	std::cout << "the detective's identity was : " << det << "\nthe killer's identity was : " << kil << "\n\nGAME OVER!!\n\n";

	runGame = false;
}

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
	//set coursor posistion
	static const HANDLE han = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coords = { 0,1 };
	SetConsoleCursorPosition(han, coords);

	for (int row = 0; row < 5; row++)
	{
		std::cout << "";
		for (int column = 0; column < 5; column++)
		{
			std::cout << "   " << board[row][column];
		}
		std::cout << "\n\n";
	}
	
}

//checks to see if 2 characters are adjacent (reads true if same character.
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

//accuses a player, if returns true, then it succeeded and play should pass to the other player
bool accuse(char accused, char killer, char detective)
{
	if (isAdj(detective, accused))
	{
		std::cout << accused << " was accused!!!\n";
		if (accused == killer)
		{
			std::cout << "The Killer was caught!!! the detective wins!!!\n";
			GameEnd(detective, killer);
		}
		else
		{
			std::cout << accused << " was not the killer...\n";
		}

		return true;
	}
	else
	{
		std::cout << "You cannot accuse " << accused << ", they are not adjacent to you\n";
		return false;
	}
}



static bool who = true;
void changeTurn()
{
	//true if detectives turn, false if killer's turn
	//set coursor posistion
	static const HANDLE han = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coords = { 0,0 };
	SetConsoleCursorPosition(han, coords);

	if (who)
	{
		who = false;
		std::cout << "it is the Killer's Turn\n";
	}
	
	else
	{
		who = true;
		std::cout << "it is the Detective's Turn\n";
	}
}




int main()
{
	//change font size
	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(font);
	font.nFont = 0;
	font.dwFontSize.X = 40;
	font.dwFontSize.Y = 40;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font);


	char inp[] = "d2";

	//test identities
	char det('a'), kil('d');
	
	while (runGame)
	{
		//draws board
		changeTurn();
		draw();
		//get input
		std::cin >> inp;
		

		if (inp[1] >= 49 && inp[1] <= 53)
		{
			if (inp[0] == 'd')
			{

				shift_down(inp[1] - 49);

			}
			else if (inp[0] == 'u')
			{

				shift_up(inp[1] - 49);

			}
			else if (inp[0] == 'l')
			{

				shift_left(inp[1] - 49);

			}
			if (inp[0] == 'r')
			{

				shift_right(inp[1] - 49);
			}
			
		}
		else if (inp[0] == 'a' && who == true)
		{
			accuse(inp[1], kil, det);
		}
		else if (inp[0] == 'k' && who == false)
		{
			//accuse(inp[1], kil, det);
		}
		

		
		//system("pause");
		std::cin.clear();
		std::cin.ignore(1000, '\n');
	}
	system("pause");
	return 0;
}