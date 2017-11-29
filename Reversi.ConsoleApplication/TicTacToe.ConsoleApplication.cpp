#include "pch.h"
#include "TriStateGameBoard.h"

using namespace std;
using namespace Library;

// the foundation for which this house will be built
int main(int argc, char* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	TicTacToe::TriStateGameBoard board = TicTacToe::TriStateGameBoard(3, 3);

	TicTacToe::BoardState currentPlayer = TicTacToe::BoardState::Player1;
	while (board.GetWinner() == TicTacToe::BoardState::Empty)
	{
		int16_t row = -1;
		int16_t col = -1;
		if (currentPlayer == TicTacToe::BoardState::Player1)
		{
			while (!board.SetBoardState(row, col, currentPlayer))
			{
				cout << "Player 1> ";
				cin >> row >> col;
				cout << endl;
			}
			currentPlayer = TicTacToe::BoardState::Player2;
		}
		else
		{
			while (!board.SetBoardState(row, col, currentPlayer))
			{
				cout << "Player 2> ";
				cin >> row >> col;
				cout << endl;
			}
			currentPlayer = TicTacToe::BoardState::Player1;
		}
		board.Print();
		
	}

	TicTacToe::BoardState winner = board.GetWinner();
	switch (winner)
	{
	case TicTacToe::Player1:
		cout << "Player 1 is a Winner!" << endl;
		break;
	case TicTacToe::Player2:
		cout << "Player 2 is a Winner!" << endl;
		break;
	case TicTacToe::Empty:
		cout << "Nobody has won." << endl;
		break;
	default:
		break;
	}
	string input;
	cin >> input;

	return 0;
}