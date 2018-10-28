/*

NOIR: Text-Based
V10.28.2018

Contributors
Josh Trask - 100705771 
Kiragh Iacono - 100689906

*/

#include <iostream>
#include <string>
#include <vector>
#include <Windows.h>
#include <cwchar>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <iomanip>

using namespace std;

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
	{ 'a','b','c','d','e' },
	{ 'f','g','h','i','j' },
	{ 'k','l','m','n','o' },
	{ 'p','q','r','s','t' },
	{ 'u','v','w','x','y' }
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
	std::cout << "The detective's identity was : " << det << "\nThe killer's identity was : " << kil << "\n\nGAME OVER!!\n\n";

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
		board[i][column] = board[i + 1][column];
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
		board[row][i] = board[row][i + 1];
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

//outputs the current board
void draw()
{
	//set coursor position
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
		system("pause");
		if (accused == killer)
		{
			std::cout << "The Killer was caught!!! the detective wins!!!\n";
			GameEnd(detective, killer);
		}
		else
		{
			std::cout << accused << " was not the killer...\n";
			system("pause");
		}

		return true;
	}
	else
	{
		std::cout << "You cannot accuse " << accused<<endl;
		system("pause");
		return false;
	}
	system("pause");
}

//kills player, similar to accuse
bool kill(char killed, char killer, char detective)
{
	if (isAdj(killer, killed) && killer != killed && !contain(dead, killed))
	{
		std::cout << killed << " was killed!!!\n";
		system("pause");
		dead.push_back(killed);//adds dead person to array
		if (killed == detective)
		{
			std::cout << "The detective has been killed!!! The killer wins!!!\n";
			GameEnd(detective, killer);
		}

		return true;
	}
	else
	{
		std::cout << "You cannot kill " << killed<<endl;
		system("pause");
		return false;
	}
}

//alternate kill function for first turn only, cannot kill detective
bool kill(char killed, char killer)
{
	if (isAdj(killer, killed) && killer != killed && !contain(dead, killed))
	{
		std::cout << killed << " was killed!!!\n";
		system("pause");
		dead.push_back(killed);//adds dead person to array
		return true;
	}
	else
	{
		std::cout << "You cannot kill " << killed<<endl;
		system("pause");
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
		std::cout << "Disguise failed! Drew : " << newchar<<endl;
		system("pause");
	}
	else
	{
		inno.erase(inno.begin() + r);
		kil = newchar;
		std::cout << "New character : " << kil<<endl;
		system("pause");
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
	std::cout << "\nChoose one of the following characters to exonerate :\n" << hand[0] << " , " << hand[1] << " , " << hand[2] << " , " << hand[3];

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
					std::cout << "Yes! They are adjacent!";
				}
				else
				{
					std::cout << "\n"<< in << " is not adjacent to killer.";
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
			system("CLS");
			who = false;
			std::cout << "It is the Killer's Turn        \n";
			cout << right << setw(50) << setfill(' ') << "Identity: " << kil << "\n";
			cout << right << setw(50) << setfill(' ') << "Killer actions" << "\n\n";
			cout << right << setw(50) << setfill(' ') << "r - shift right" << "\n";
			cout << right << setw(50) << setfill(' ') << "l - shift left" << "\n";
			cout << right << setw(50) << setfill(' ') << "u - shift up" << "\n";
			cout << right << setw(50) << setfill(' ') << "d - shift down" << "\n";
			cout << right << setw(50) << setfill(' ') << "k - kill" << "\n";
			cout << right << setw(50) << setfill(' ') << "h - disguise" << "\n";
		}

		else
		{
			
			system("CLS");
			who = true;
			std::cout << "It is the Detective's Turn      \n";
			cout << right << setw(50) << setfill(' ') << "Identity: " << det << "\n";
			cout << right << setw(50) << setfill(' ') << "Detective actions" << "\n\n";
			cout << right << setw(50) << setfill(' ') << "r - shift right" << "\n";
			cout << right << setw(50) << setfill(' ') << "l - shift left" << "\n";
			cout << right << setw(50) << setfill(' ') << "u - shift up" << "\n";
			cout << right << setw(50) << setfill(' ') << "d - shift down" << "\n";
			cout << right << setw(50) << setfill(' ') << "a - accuse" << "\n";
			cout << right << setw(50) << setfill(' ') << "e - exonerate" << "\n";
			cout << right << setw(50) << setfill(' ') << "-Hand-" << "\n";
			cout << right << setw(50) << setfill(' ') << hand[0] << "\n";
			cout << right << setw(50) << setfill(' ') << hand[1] << "\n";
			cout << right << setw(50) << setfill(' ') << hand[2] << "\n";

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
	srand(time(NULL));
	int r = rand() % 24;
	kil = inno[r];
	inno.erase(inno.begin() + r);

	system("pause");
	system("CLS");
	cout << "\nKiller! It's your turn.\n";
	system("pause");
	system("CLS");


	//killers first kill 
	std::cout << kil << " is your identity.\n";

	static char in[] = "";
	while (true)
	{
		draw();
		cout << "\nMake your first kill by entering \'k\' followed by the \nletter you intend to kill (exampleinput:kr)\n";
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

	system("pause");
	system("CLS");
	cout << "It's the detective's turn!\n";
	system("pause");
	system("CLS");
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
	std::cout << "\nYou can choose between the following characters who \nyour identity will be :" << hand[0] << " , " << hand[1] << " , " << hand[2] << " , " << hand[3] << endl;

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
	std::cout << "Your role : " << det<<endl;
	system("pause");
	system("CLS");

}
//remember to add check to make sure detective doesn't choose a dead person and become immortal


//mainly output, explains how the game works
//text-heavy bs
void tutorial() {
	std::cout << "\nWelcome to NOIR\n";
	system("pause");
	system("CLS");
	std::cout << "The gameplay is simple! \nOne of you will be playing as a vicious Killer,\nand one of you will play an expert detective.\n";
	std::cout << "The Killer will always go first.\n";
	system("pause");
	system("CLS");
	draw();
	std::cout << "The board looks something like this...\n";
	system("pause");
	system("CLS");
	draw();
	std::cout << "The killer's first turn will always be a kill.\n";
	cout << "Let's suppose the killer is \"g\"...\n";
	cout << "The killer can only kill those adjacent to themselves.\n";
	cout << "In this case, those adjacent to g are: a, b, c, f, h, \nk, l, m.\nKills can be done by typing a letter adjacent.\n";
	cout << "Once the killer makes the first move, it's the \ndetective's turn.\n";
	system("pause");
	system("CLS");
	draw();
	cout << "The first detective turn, the detective learns the\nidentity of 4 innocents.\n";
	cout << "Of these identities, the detective will need to choose \nwho they will play as.\n";
	cout << "For the purpose of this tutorial, the 4 innocents \nwill be: h, r, w, p.\n";
	cout << "The detective will be \"r\".\n";
	system("pause");
	system("CLS");
	cout << "After the first turn, players can take any action \nthey like, so long as it applies to their role.\n";
	cout << "Both the killer and the detective are able to shift \nthe board...\n";
	system("pause");
	system("CLS");
	cout << right << setw(50) << setfill(' ') << "Killer: g\n";
	cout << right << setw(50) << setfill(' ') << "Detective: r\n";
	draw();
	cout << "Shift looks something like this...\n";
	system("pause");
	system("CLS");
	shift_right(2);
	cout << right << setw(50) << setfill(' ') << "Killer: g\n";
	cout << right << setw(50) << setfill(' ') << "Detective: r\n";
	draw();
	cout << "Shift looks something like this...\n";
	cout << "r3 \nThis shifts the third column right. Other \ncommands are: d=down, u=up, l=left, r=right.\nColumns and rows are defined by the numbers 1-5.\n";
	system("pause");
	system("CLS");
	cout << right << setw(50) << setfill(' ') << "Killer: g\n";
	cout << right << setw(50) << setfill(' ') << "Detective: r\n";
	draw();
	cout << "Shift can be useful to the killer if they have already \nkilled the spaces adjacent to them, or if they want to throw the detective off their trail.\n";
	system("pause");
	system("CLS");
	shift_right(0);
	cout << right << setw(50) << setfill(' ') << "Killer: g\n";
	cout << right << setw(50) << setfill(' ') << "Detective: r\n";
	draw();
	cout << "Shift can be useful to the killer if they have already \nkilled the spaces adjacent to them, or if they want to throw the detective off their trail.\n";
	system("pause");
	system("CLS");
	cout << right << setw(50) << setfill(' ') << "Killer: g\n";
	cout << right << setw(50) << setfill(' ') << "Detective: r\n";
	draw();
	cout << "Shift can also be useful to the detective if they wish \nto accuse a space that is not already directly \nadjacent to them.\n";
	system("pause");
	system("CLS");
	shift_up(2);
	cout << right << setw(50) << setfill(' ') << "Killer: g\n";
	cout << right << setw(50) << setfill(' ') << "Detective: r\n";
	draw();
	cout << "By shifting the third row up, the detective may now \naccuse the killer.\n";
	system("pause");
	system("CLS");
	shift_down(2);
	shift_left(0);
	shift_left(2);//resets the board to original state
	cout << "The Killer has two other capabilities...\n";
	cout << "They may choose to kill an adjacent player, as \ndescribed in the first move,or they can disguise \nthemselves as a new letter.\n";
	cout << "Choosing to disguise as another letter immediately \nexonerates the previous identity of the killer. \nIf the detective thinks they have found the killer's \nposition, the killer may choose to disguise as someone\nelse, leaving any previous suspicion behind.\n";
	cout << "Disguising too many times will likely make it easier\nfor the detective to find the killer.\n";
	system("pause");
	system("CLS");
	cout << "The Detective has an action similar to the Killer's \n\"kill\"...\n";
	cout << "The accuse action allows the detective to accuse any \nadjacent letters of being the killer.\n";
	cout << "The killer must honestly say that they are or are not \nthe letter that has been accused.\n";
	cout << "Once the killer is found, the game is over.\n";
	system("pause");
	system("CLS");
	cout << "The other action that the detective can perform is \n\"Exonerate\".\n";
	cout << "As previously mentioned, the detective draws 4 \nletters, all innocent, and one of which being their \nidentity.\n";
	cout << "The remaining 3 can be used to exonerate an innocent.\nThis action eleiminates the possibility of the \nexonerated being the killer.\n";
	cout << "Exonerated letters can still be killed.\n";
	system("pause");
	system("CLS");
	cout << "The killer wins if they kill the detective, or if \nthey kill 14 other letters.\n";
	cout << "The detective wins if they find the killer.\n";
	system("pause");
	system("CLS");
}

void menu() {
	/*
	-play
	-tutorial
	-quit
	*/
	int input = 0;
	int replay = 0;

	while (input < 1) {//gets input from the user until the input is valid
		std::string line1 = "(1)Play";
		std::string line2 = "(2)Tutorial";
		std::string line3 = "(3)Quit";

		std::cout << "NOIR" << std::endl;
		std::cout << right << setw(50) << setfill(' ') << line1 << "\n\n";
		std::cout << right << setw(50) << setfill(' ') << line2 << "\n\n";
		std::cout << right << setw(50) << setfill(' ') << line3 << "\n\n";

		cin >> input;

		if (!cin || input > 3 || input < 1) {
			cin.clear();//clears cin
			cin.ignore(1000, '\n');//ignores all input in cin until the next line
			cout << "\nInvalid Input.\n";
		}
	}

	switch (input)
	{
	case 1:
		runGame = true;
		startGame();
		break;
	case 2:
		//tutorial runs then main runs

		while (replay < 1) {
			tutorial();
			cout << "Would you like to replay the tutorial?";
			cout << "\n(1)Yes\n(2)No\n";
			cin >> replay;
			if (!cin || input > 2 || input < 1) {
				cin.clear();//clears cin
				cin.ignore(1000, '\n');//ignores all input in cin until the next line
				cout << "\nInvalid Input.\n";
			}
			if (replay == 1) {
				replay = 0;
			}
			else {
				runGame = true;
				startGame();
			}
		}
		break;

	case 3:
		runGame = false;
		;break;
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

	//window size/position
	HWND console = GetConsoleWindow();
	RECT newWind;
	GetWindowRect(console, &newWind);
	MoveWindow(console, newWind.left, newWind.top, 1300, 900, TRUE);

	char inp[] = "d2";

	menu();
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
			system("pause");
			system("CLS");
			cout << "\nDetective! It's your turn.\n";
			system("pause");
			system("CLS");
			DetectTurn(inp);
		}
		else if (!who) {
			system("pause");
			system("CLS");
			cout << "\nKiller! It's your turn.\n";
			system("pause");
			system("CLS");
			killTurn(inp);
		}



		std::cin.clear();
		std::cin.ignore(1000, '\n');
	}
	system("pause");
	return 0;
}