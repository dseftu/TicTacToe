#include "pch.h"
#include "TriStateGameBoard.h"

namespace TicTacToe
{
	TicTacToe::TriStateGameBoard::TriStateGameBoard(int16_t rows, int16_t cols)
	{
		
		SetRowsCount(rows);
		SetColsCount(cols);
		int16_t index = (GetRowsCount()*GetColsCount()) / (sizeof(int16_t) * 4);
		mBoard.resize(index + 1);

	}

	void TicTacToe::TriStateGameBoard::Print()
	{
		cout << "printing..." << endl;
		for (int16_t row = 1; row <= GetRowsCount(); row++)
		{
			
			for (int16_t col = 1; col <= GetColsCount(); col++)
			{
				BoardState tempBoardState = GetBoardState(row, col);
				char c = BoardStateToChar(tempBoardState);
				cout << c;
				if (col != GetColsCount()) cout << '|';
			}
			cout << endl << "------" << endl;
		}

	}

	BoardState TicTacToe::TriStateGameBoard::GetBoardState(int16_t row, int16_t col)
	{
		// need to know the position of the bit pair on the map
		// should be (row number * column number) - (number of total columns - column number)
		// So, row 3, col 5 = 21st bit pair
		int16_t bitPair = GetBitPair(row, col);

		// from this we can get the actual index in the board array
		// (sizeof() is multiplied by 4 to account for the number of bit pairs in a byte
		int16_t index = (bitPair-1) / (sizeof(int16_t)*4);
		uint16_t player1mask = 1 << (bitPair - 2) % (sizeof(int16_t) * 8);
		uint16_t player2mask = 2 << (bitPair - 2) % (sizeof(int16_t) * 8);
		
		if (mBoard[index] & player1mask) return BoardState::Player1;
		if (mBoard[index] & player2mask) return BoardState::Player2;
		return BoardState::Empty;
		
	}
	void TriStateGameBoard::SetBoardState(int16_t row, int16_t col, BoardState state)
	{
		// need to know the position of the bit pair on the map
		// should be (row number * column number) - (number of total columns - column number)
		// So, row 3, col 5 = 21st bit pair
		int16_t bitPair = GetBitPair(row,col);

		// from this we can get the actual index in the board array
		// (sizeof() is multiplied by 4 to account for the number of bit pairs in a byte
		int16_t index = (bitPair - 1) / (sizeof(int16_t) * 4);

		uint16_t mask = 0;
		uint16_t unmask = 3 << (bitPair - 2) % (sizeof(int16_t) * 8);
		mBoard[index] = (mBoard[index] | unmask) ^ unmask;
		switch (state)
		{
		case TicTacToe::Player1:
			mask = 1 << (bitPair - 2) % (sizeof(int16_t) * 8);
			break;
		case TicTacToe::Player2:
			mask = 2 << (bitPair - 2) % (sizeof(int16_t) * 8);
			break;
		case TicTacToe::Empty:
			return;
		default:
			break;
		}
		mBoard[index] = mBoard[index] | mask;
		
	}
	int16_t TriStateGameBoard::GetBitPair(int16_t row, int16_t col)
	{
		return (row * GetColsCount()) - (GetColsCount() - col);
	}
	char TriStateGameBoard::BoardStateToChar(BoardState b)
	{
		switch (b)
		{
		case TicTacToe::Player1:
			return mPlayer1Icon;
			break;
		case TicTacToe::Player2:
			return mPlayer2Icon;
			break;
		default:
			return mEmptyIcon;
			break;
		}
	}
	float TriStateGameBoard::EvaluateGameBoard(BoardState player)
	{
		UNREFERENCED_PARAMETER(player);
		return 0.0f;
	}
	BoardState TriStateGameBoard::GetWinner()
	{
		BoardState winner = BoardState::Empty;

		for (int16_t row = 1; row <= GetRowsCount(); row++)
		{
			for (int16_t col = 1; col <= GetColsCount(); col++)
			{
				BoardState tempBoardState = GetBoardState(row, col);

				if (tempBoardState == BoardState::Empty)
				{
					winner = BoardState::Empty;
					break;
				}
				// if winner is empty, then set it.  otherwise, check it.
				else if (winner == BoardState::Empty) winner = tempBoardState;
				else if (tempBoardState != winner)
				{ 
					winner = BoardState::Empty;
					break;
				}
			}
		}

		return winner;
	}
}
