#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <string>

#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>

#include <algorithm>    // std::shuffle
#include <array>        // std::array
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

using namespace std;


typedef vector<int> deck;
typedef vector<int> player;
typedef vector<int> dealer;
typedef vector<int> dealerHole;



float calculate(int betAmount);

float result;

static float playerCash = 1000.00f;

const int deckSize = 52;
const int deckNumber = 6;
const int suitCount = 4;
const int valueCount = 13;

static deck mainDeck;
static player playerHand;
static dealer dealerHand;
static dealerHole dealerHidden;

char outcome;

int softCheck = 0;
int cardValue;
int betAmount;
int playerSum;
int dealerSum;

string pSoft;
string dSoft;

bool playerSoft;
bool dealerSoft;

//timebased seed
unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();


void shuffle(int deck[], int size);

//Deck is created/reset
void init_deck()
{
	// Number of Decks
	for (int h = 0; h < deckNumber; h++)
	{
		// Number of Suits
		for (int i = 0; i < suitCount; i++)
		{
			// Number of Values
			for (int j = 0; j < valueCount; j++)
			{
				mainDeck.push_back(i * valueCount + j);
			}
		}
	}
	//Shuffling Deck
	random_shuffle(mainDeck.begin(), mainDeck.end());
}

void show_card(int card)
{
	switch (card % valueCount)
	{
	case 1:
		cout << "(A";
		cardValue = 11;
		softCheck = 1;
		break;
	case 2:
		cout << "(2";
		cardValue = 2;
		break;
	case 3:
		cout << "(3";
		cardValue = 3;
		break;
	case 4:
		cout << "(4";
		cardValue = 4;
		break;
	case 5:
		cout << "(5";
		cardValue = 5;
		break;
	case 6:
		cout << "(6";
		cardValue = 6;
		break;
	case 7:
		cout << "(7";
		cardValue = 7;
		break;
	case 8:
		cout << "(8";
		cardValue = 8;
		break;
	case 9:
		cout << "(9";
		cardValue = 9;
		break;
	case 10:
		cout << "(10";
		cardValue = 10;
		break;
	case 11:
		cout << "(J";
		cardValue = 10;
		break;
	case 12:
		cout << "(Q";
		cardValue = 10;
		break;
	case 0:
		cout << "(K";
		cardValue = 10;
		break;
		//Error Message
	default:
		cout << "?";
		cardValue = 1;
		break;
	}

	switch (card / valueCount)
	{
	case 0:
		cout << " of Hearts)" << endl;
		break;
	case 1:
		cout << " of Spades)" << endl;
		break;
	case 2:
		cout << " of Diamonds)" << endl;
		break;
	case 3:
		cout << " of Clubs)" << endl;
		break;
		//Error Message
	default:
		cout << " of Errors)" << endl;
		break;
	}


}

//Check player Soft
void checkPlayerSoft()
{
	if (playerSoft == true && playerSum > 21)
	{
		playerSum = playerSum - 10;
		playerSoft = false;
		pSoft = " ";
	}
	else if (playerSoft == true)
	{
		pSoft = "Soft ";
	}
	else if (playerSoft == false)
	{
		pSoft = " ";
	}
}

//Check dealer Soft
void checkDealerSoft()
{
	if (dealerSoft == true && dealerSum > 21)
	{
		
		dealerSum = dealerSum - 10;
		dealerSoft = false;
		dSoft = " ";
	}
	else if (dealerSoft == true)
	{
		dSoft = "Soft ";
	}
	else if (dealerSoft == false)
	{
		dSoft = " ";
	}
}


//Player draws a card
void player_draw()
{
	int nCard = mainDeck.back();
	mainDeck.pop_back();
	
	playerHand.push_back(nCard);
	cout << "player draws: ";
	show_card(nCard);
	checkPlayerSoft();
	playerSum = playerSum + cardValue;
	if (softCheck == 1)
	{
		playerSoft = true;
		softCheck = 0;
	}
}

//Dealer draws a card
void dealer_draw()
{
	int nCard = mainDeck.back();
	mainDeck.pop_back();

	dealerHand.push_back(nCard);
	cout << "dealer draws: ";
	show_card(nCard);
	checkDealerSoft();
	dealerSum = dealerSum + cardValue;
	if (softCheck == 1)
	{
		dealerSoft = true;
		softCheck = 0;
	}
}

//Dealer draws Hole Card
void dealer_hidden()
{
	int nCard = mainDeck.back();
	mainDeck.pop_back();
	cout << "dealer draws: (a hole card)" << endl;

	dealerHidden.push_back(nCard);
}

//Dealer reveals Hole Card
void dealer_reveal()
{
	int nCard = dealerHidden.back();
	dealerHidden.pop_back();

	dealerHand.push_back(nCard);
	cout << endl << "dealer reveals:";
	show_card(nCard);
	if (softCheck == 1)
	{
		dealerSoft = true;
		softCheck = 0;
	}
	dealerSum = dealerSum + cardValue;
}



//calculating result
float calculate()
{
	switch (outcome)
	{
		//surrender
	case's':
		result = -betAmount * 0.5f;
		break;
		//win
	case'w':
		result = betAmount;
		break;
		//lose
	case'l':
		result = -betAmount;
		break;
		//draw
	case'd':
		result = 0;
		break;
		//blackjack
	case'b':
		result = betAmount * 1.5f;
		break;

	}

	return result;
}






int main() 
{
	srand(seed);
	
	bool soft17 = true;

	int difficulty;
	string difficultySet;

	init_deck();

	cout << "Welcome to Blackjack!" << endl << "Please choose difficulty(1/2/3)." << endl;

	while (true)
	{
		cin >> difficultySet;

		if (difficultySet == "1")
		{
			difficulty = 17;
			bool soft17 = false;
			cout << "Welcome to Blackjack!" << endl << "Dealer stands at all 17's." << endl << "Bet any integer amount of credits between 5 and 1000" << endl;
			break;
		}
		else if (difficultySet == "2")
		{
			difficulty = 17;
			bool soft17 = true;
			cout << "Welcome to Blackjack!" << endl << "Dealer hits at Soft 17." << endl << "Bet any integer amount of credits between 5 and 1000" << endl;
			break;
		}

		else if (difficultySet == "3")
		{
			difficulty = 20;
			bool soft17 = true;
			cout << "Welcome to Blackjack!" << endl << "Dealer hits at Soft 17." << endl << "Player loses 17-19 ties" << endl << "Bet any integer amount of credits between 5 and 1000" << endl;
			break;
		}
		else
		{
			cout << "Pleasu use 1/2/3 command only." << endl;
		}
	}


	while (true)
	{
		bool roundOver = false;
		bool playerBlackjack = false;
		bool dealerBlackjack = false;
		bool playerSurrender = false;
		playerSoft = false;
		dealerSoft = false;
		playerSum = 0;
		dealerSum = 0;
		pSoft = " ";
		dSoft = " ";

		while (true)
		{
			
			cout << "\n" << endl << "your current credits are: " << playerCash << endl << "Please input the amount you are willing to bet:" << endl;
			cin >> betAmount;

			//if invalid betamount
			if (cin.fail())
			{
				cin.clear();
				cin.ignore(100000, '\n');
				cout << "invalid input." << endl;
			} 
			else if (betAmount > playerCash)
			{
				cout << "Not enough credits" << endl;
			}
			else if (betAmount < 5 || betAmount > 1000.f)
			{
				cout << "Can only bet between 5 and 1000..." << endl;
			}
			else
			{
				break;
			}
		}
		
		//First Deal
		player_draw();
		dealer_hidden();
		player_draw();
		dealer_draw();

		string firstHitStand;
		
		//Player choose hit, stand, double or surrender
		checkPlayerSoft();
		checkDealerSoft();
		cout << "\n" << "Your total is: " << pSoft << playerSum << endl;
		cout << "Dealer Total: " << dSoft << dealerSum << " + ?" << endl;

		if (playerSum < 21)
		{
			cout << "Hit, Stand, Double or Surrender/Retreat? (h/s/d/r)" << endl;
		}
		
		
		while (true)
		{
			//check if blackjack
			if (playerSum == 21)
			{
				playerBlackjack = true;
				break;
			}
			cin >> firstHitStand;

			if (firstHitStand == "h")
			{
				player_draw();
				break;
			}
			else if (firstHitStand == "s")
			{
				break;
			}
			else if (firstHitStand == "d" && betAmount * 2 <= playerCash)
			{
				betAmount = betAmount * 2;
				player_draw();
				break;
			}
			else if (firstHitStand == "d" && betAmount * 2 >= playerCash)
			{
				cout << "Not enough credits to Double." << endl;
			}
			else if (firstHitStand == "r")
			{
				playerSurrender = true;
				break;
			}
			else
			{
				cout << "Pleasu use h/s/d/r command only." << endl;
			}

		}

		//If player hit can no longer double or surrender
		if (firstHitStand == "h")
		{
			while (true)
			{
				checkPlayerSoft();
				//Check if player can choose
				if (playerSum >= 21)
				{
					break;
				}
				//Player choose hit or stand
				cout << "\n" << "Your total is: " << pSoft << playerSum << endl;
				cout << "Hit or Stand? (h/s)" << endl;
				string hitStand;
				while (true)
				{
					cin >> hitStand;

					if (hitStand == "h")
					{
						player_draw();
						break;
					}
					else if (hitStand == "s")
					{
						break;
					}
					else
					{
						cout << "Pleasu use h/s command only." << endl;
					}

				}
				if (hitStand == "s")
				{
					break;
				}
			}
		}

		//Dealer reveals hole card
		dealer_reveal();

		if (dealerSum == 21)
		{
			dealerBlackjack = true;
		}

		while (true)
		{
			checkDealerSoft();

			//If dealer hits Soft17
			if (dealerSum == 17 && dealerSoft == true && soft17 == true)
			{
				dealer_draw();
				continue;
			}
			else if (dealerSum >= 17)
			{
				break;
			}
			dealer_draw();
		}
		
		checkPlayerSoft();
		checkDealerSoft();
		cout << endl;
		cout << "Player Total: " << playerSum << endl;
		cout << "Dealer Total: " << dealerSum << endl;

		roundOver = true;

		//outcomes
		if (playerSurrender == true)
		{
			//Surrender
			outcome = 's';
			cout << "You have surrendered, you lost: " << betAmount * 0.5f << endl;
		}

		else if (playerBlackjack == true && dealerBlackjack == true)
		{
			//Blackjack Push
			outcome = 'd';
			cout << "Both you and dealerscored a Blackjack: status quo..." << endl;
		}
		else if (playerBlackjack == true)
		{
			//Blackjack
			outcome = 'b';
			cout << "Congratulations you've scored a Blackjack: you won " << betAmount * 1.5f << endl;
		}

		else if (dealerBlackjack == true)
		{
			//Dealer Blackjack
			outcome = 'l';
			cout << "The Dealer scored a Blackjack: you lost " << betAmount << endl;
		}

		else  if (playerSum > 21)
		{
			//Bust
			outcome = 'l';
			cout << "You're Bust: you lost " << betAmount << endl;
		}
		else
		{
			if (playerSum > dealerSum)
			{
				//Win
				outcome = 'w';
				cout << "You've Won against the Dealer: you won " << betAmount << endl;
			}
			else if (dealerSum > 21)
			{
				//Win
				outcome = 'w';
				cout << "Dealer Bust. You've Won against the Dealer: you won " << betAmount << endl;
			}
			//Dealer win some ties at difficulty 3
			else if (playerSum == dealerSum && playerSum >= difficulty)
			{
				//draw
				outcome = 'd';
				cout << "You're Tied with the Dealer: status quo..." << endl;
			}
			else if (playerSum <= dealerSum)
			{
				//Loss
				outcome = 'l';
				cout << "You've Lost to the Dealer: you lost " << betAmount << endl;
			}
			
		}

		//aftermath of round
		if (roundOver == true)
		{
			// calculate();
			//playerCash = playerCash + result;
			playerCash += calculate();
			cout << "Your current credits are: " << playerCash << endl;
		}

		//Continue
		cout << "Continue Playing (y/n)" << endl;
		string a;

		//quits game if player has no cash
		if (playerCash <= 5)
		{
			break;
		}

		while (true) 
		{
			cin >> a;
			if (a == "y")
			{
				break;
			}
			else if (a == "n")
			{
				break;
			}
			else
			{
				cout << "Pleasu use y/n command only." << endl;
			}
		}
		//Ends the game
		if (a == "n")
		{
			break;
		}



		//If deck needs shuffle
		if (mainDeck.size() < 100 && roundOver == true)
		{
			cout << "Deck is low on cards, reshuffeling..." << endl;
			init_deck();
		}


	}

	cout << "Entering the castle of" /*<< endl << */" aaaaaaaaaaaaaaaaaaaaaaaaarrrrgh..." << endl;
	return 0;
}

