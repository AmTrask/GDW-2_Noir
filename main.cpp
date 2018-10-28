//Josh Trask
//100705771

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <cwchar>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>

//global variables
int tempRow;
int tempColumn;
bool turnDone = true; // checks if turn is finished
static bool who = false; // keeps track of who's turn it is(True for detective, false for killer)
char det, kil;
bool runGame = true;
std::vector <char> dead;//array to keep track of dead.
std::vector <char> inno = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y' };
std::vector <char> hand; //hand for the detective
char board[5][5] = 
{
{'a','b','c','d','e'},
{'f','g','h','i','j'}, 
{'k','l','m','n','o'}, 
{'p','q','r','s','t'}, 
{'u','v','w','x','y'}
};

//checks to see if char is in string
bool contain(std::vector<char> list, char letter)
{
	for (int i = 0; i < list.size(); i++)
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
bool shift_up(int column)
{
	char temp = board[0][column];
	
	for (int i = 0; i < 4; i++)
	{
		board[i][column] = board[i+1][column];
	}
	board[4][column] = temp;
	return true;
}

//shifts a column downwards, returns true if succeeds so that the turn can change
bool shift_down(int column)
{
	char temp = board[4][column];

	for (int i = 4; i > 0; i--)
	{
		board[i][column] = board[i - 1][column];
	}
	board[0][column] = temp;
	return true;
}

//shifts a row left
bool shift_left(int row)
{
	char temp = board[row][0];

	for (int i = 0; i < 4; i++)
	{
		board[row][i] = board[row][i+1];
	}
	board[row][4] = temp;
	return true;
}

//shifts a row right
bool shift_right(int row)
{
	char temp = board[row][4];

	for (int i = 4; i > 0; i--)
	{
		board[row][i] = board[row][i - 1];
	}
	board[row][0] = temp;
	return true;
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
	if (isAdj(detective, accused) && !contain(dead, accused))
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
		std::cout << "You cannot accuse " << accused;
		return false;
	}
}

//kills player, similar to accuse
bool kill(char killed, char killer, char detective)
{
	if (isAdj(killer, killed) && killer != killed && !contain(dead, killed))
	{
		std::cout << killed << " was killed!!!\n";
		dead.push_back(killed);//adds dead person to array
		if (killed == detective)
		{
			std::cout << "The detective has been killed!!! the killer wins!!!\n";
			GameEnd(detective, killer);
		}
		
		return true;
	}
	else
	{
		std::cout << "You cannot kill " << killed;
		return false;
	}
}
//alternate kill function for first turn only, cannot kill detective
bool kill(char killed, char killer)
{
	if (isAdj(killer, killed) && killer != killed && !contain(dead, killed))
	{
		std::cout << killed << " was killed!!!\n";
		dead.push_back(killed);//adds dead person to array
		return true;
	}
	else
	{
		std::cout << "You cannot kill " << killed;
		return false;
	}
}

bool disguise(char killer)
{
	if (inno.size() == 0)
	{
		return false;
	}
	int r;
	srand(time(NULL));
	r = rand() % inno.size();
	char newchar = inno[r];
	
	//check if dead
	if (contain(dead, newchar))
	{
		std::cout << "disquise failed! drew : " << newchar;
	}
	else
	{
		inno.erase(inno.begin() + r);
		kil = newchar;
		std::cout << "new character : " << kil;
	}
	return true;
}

bool exxonerate()
{
	if (inno.size() == 0)
	{
		return false;
	}
	char in[] = "                                   ";
	int r;
	srand(time(NULL));
	r = rand() % inno.size();
	char newchar = inno[r];
	inno.erase(inno.begin() + r);
	hand.push_back(newchar);
	std::cout << "\n choose one of the characters to exonerate :" << hand[0] << " , " << hand[1] << " , " << hand[2] << " , " << hand[3];

	bool idk = false;
	while (!idk)
	{
		//std::cout << in;
		std::cin >> in;
		for (int i = 0; i < 4; i++)
		{
			if (in[0] == hand[i])
			{
				if (isAdj(in[0], kil))
				{
					//TODO  make blue
					std::cout << "yes! they are adjacent!";
				}
				else
				{
					std::cout << "is not adjacent to killer";
				}


				idk = true;
				hand.erase(hand.begin() + i);
				break;
			}
		}
	}
	return true;
}

//changes turn upon action success.
void changeTurn()
{
	//true if detectives turn, false if killer's turn
	//set coursor posistion
	static const HANDLE han = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coords = { 0,0 };
	SetConsoleCursorPosition(han, coords);
	if (turnDone)
	{

		if (who)
		{
			who = false;
			std::cout << "it is the Killer's Turn        \n";
		}

		else
		{
			who = true;
			std::cout << "it is the Detective's Turn      \n";
		}
		turnDone = false;
	}
}

//all actions the detective can do
void DetectTurn(char inp[])
{
	if (inp[1] >= 49 && inp[1] <= 53)
	{
		if (inp[0] == 'd')
		{

			turnDone = shift_down(inp[1] - 49);

		}
		else if (inp[0] == 'u')
		{

			turnDone = shift_up(inp[1] - 49);

		}
		else if (inp[0] == 'l')
		{

			turnDone = shift_left(inp[1] - 49);

		}
		if (inp[0] == 'r')
		{

			turnDone = shift_right(inp[1] - 49);
		}

	}
	else if (inp[0] == 'a')
	{
		turnDone = accuse(inp[1], kil, det);
	}
	else if (inp[0] == 'e')
	{
		turnDone = exxonerate();
	}
}
//all actions for the killer
void killTurn(char inp[])
{
	if (inp[1] >= 49 && inp[1] <= 53)
	{
		if (inp[0] == 'd')
		{

			turnDone = shift_down(inp[1] - 49);

		}
		else if (inp[0] == 'u')
		{

			turnDone = shift_up(inp[1] - 49);

		}
		else if (inp[0] == 'l')
		{

			turnDone = shift_left(inp[1] - 49);

		}
		if (inp[0] == 'r')
		{

			turnDone = shift_right(inp[1] - 49);
		}

	}
	else if (inp[0] == 'k')
	{
		turnDone = kill(inp[1], kil, det);
	}
	else if (inp[0] == 'h')
	{
		turnDone = disguise(kil);
	}
}




void detREROLL(int &r1, int &r2, int &r3, int &r4)
{
	r1 = rand() % 23;
	r2 = rand() % 23;
	r3 = rand() % 23;
	r4 = rand() % 23;
}
void startGame()
{
	//assign role for killer
	srand (time(NULL));
	int r = rand() % 24;
	kil = inno[r];
	inno.erase(inno.begin() + r);
	
	//killers first kill
	std::cout << kil << "  Killer! make your first kill!";
	
	static char in[] = "";
	while (true)
	{
		draw();
		std::cin >> in;
		if (in[0] == 'k')
		{
			turnDone = kill(in[1], kil);
			if (turnDone)
			{
				break;
			}
		}
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << " \nInvalid move, first move must be a kill\n";

	}
	who = false;

	//detectives first turn...

	//get choice of character
	int r1[4];
	srand(time(NULL));
	for (int j = 0; j < 4; j++)
	{
		r1[j] = rand() % inno.size() - 1;
		hand.push_back(inno[r1[j]]);
		inno.erase(inno.begin() + r1[j]);
	}
	//get rid of those cards in inno deck
	
	//choose role
	std::cout << "\n you can choose between the following characters :" << hand[0] << " , " << hand[1] << " , " << hand[2] << " , " << hand[3];
	
	bool idk = false;
	while (!idk)
	{
		//std::cout << in;
		std::cin >> in;
		for (int i = 0; i < 4; i++)
		{
			if (in[0] == hand[i])
			{
				det = hand[i];
				idk = true;
				hand.erase(hand.begin() + i);
				break;
			}
		}
	}
	std::cout << "your role : " << det;

}
//remember to add check to make sure detective doesn't choose a dead person adn become immortal



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
	
	startGame();
	//system("pause");
	while (runGame)
	{
		//draws board
		changeTurn();
		draw();
		//get input
		std::cin >> inp;
		
		//turn actions
		
		
		if (who)
		{
			DetectTurn(inp);
		}
		else if (!who) {
			killTurn(inp);
		}
		

		
		std::cin.clear();
		std::cin.ignore(1000, '\n');
	}
	system("pause");
	return 0;
}