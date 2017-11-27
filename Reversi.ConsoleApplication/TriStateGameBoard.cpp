#include "pch.h"
#include "TriStateGameBoard.h"

namespace TicTacToe
{
	TicTacToe::TriStateGameBoard::TriStateGameBoard(int16_t rows, int16_t cols)
	{
		UNREFERENCED_PARAMETER(rows);
		UNREFERENCED_PARAMETER(cols);
	}

	void TicTacToe::TriStateGameBoard::Print()
	{
		for (int16_t row = 0; row < GetRowsCount(); row++)
		{
			for (int16_t col = 0; col < GetColsCount(); col++)
			{
				BoardState tempBoardState = GetBoardState(row, col);
				cout << BoardStateToChar(tempBoardState);
			}

			cout << endl;
		}

	}

	BoardState TicTacToe::TriStateGameBoard::GetBoardState(int16_t row, int16_t col)
	{
		// need to know the position of the bit pair on the map
		// should be (row number * column number) - (number of total columns - column number)
		// So, row 3, col 5 = 21st bit pair
		int16_t bitPair = (row * col) - (GetColsCount() - col);

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
		UNREFERENCED_PARAMETER(row);
		UNREFERENCED_PARAMETER(col);
		UNREFERENCED_PARAMETER(state);
		
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
		return BoardState();
	}
}
