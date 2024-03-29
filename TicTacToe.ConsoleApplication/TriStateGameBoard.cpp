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
		uint16_t bitPair = GetBitPair(row, col);

		// from this we can get the actual index in the board array
		// (sizeof() is multiplied by 4 to account for the number of bit pairs in a byte
		int16_t index = (bitPair-1) / (sizeof(int16_t)*4);
		uint16_t player1mask = 1 << (bitPair*2-2) % (sizeof(int16_t) * 8);
		uint16_t player2mask = 2 << (bitPair*2-2) % (sizeof(int16_t) * 8);
		
		if (mBoard[index] & player1mask) return BoardState::Player1;
		if (mBoard[index] & player2mask) return BoardState::Player2;
		return BoardState::Empty;
		
	}
	bool TriStateGameBoard::SetBoardState(int16_t row, int16_t col, BoardState state)
	{
		// if this state is occupied and the state we are trying to set isn't empty, this fails
		if (row < 1 
			|| col < 1 
			|| row > GetRowsCount() 
			|| col > GetColsCount() 
			|| (GetBoardState(row, col) != BoardState::Empty && state != BoardState::Empty)) return false;

		// need to know the position of the bit pair on the map
		// should be (row number * column number) - (number of total columns - column number)
		// So, row 3, col 5 = 21st bit pair
		uint16_t bitPair = GetBitPair(row,col);

		// from this we can get the actual index in the board array
		// (sizeof() is multiplied by 4 to account for the number of bit pairs in a byte
		int16_t index = (bitPair - 1) / (sizeof(int16_t) * 4);
		uint16_t mask = 0;
		uint16_t unmask = 3 << ((bitPair * 2) - 2) % (sizeof(int16_t) * 8);
				
		mBoard[index] = (mBoard[index] | unmask) ^ unmask;

		switch (state)
		{
		case TicTacToe::Player1:
			mask = 1 << (bitPair*2-2) % (sizeof(int16_t) * 8);
			break;
		case TicTacToe::Player2:
			mask = 2 << (bitPair*2-2) % (sizeof(int16_t) * 8);
			break;
		case TicTacToe::Empty:
			return true;
		default:
			break;
		}
		mBoard[index] = mBoard[index] | mask;


		mLastRowMove = row;
		mLastColMove = col;		

		return true;
		
	}
	float TriStateGameBoard::MiniMax(TriStateGameBoard board, BoardState player, bool maximizngPlayer)
	{
		if (board.IsTerminalNode()) return board.EvaluateGameBoard(player);
		float bestValue = 100;
		if (maximizngPlayer) bestValue = -100;

		vector<TriStateGameBoard> newBoards = board.GetFutureBoards(player);
		for each (TriStateGameBoard newBoard in newBoards)
		{
			float newValue = MiniMax(newBoard, GetOtherPlayer(player), !maximizngPlayer);
			if (maximizngPlayer && newValue >= bestValue) bestValue = newValue;
			if (!maximizngPlayer && newValue <= bestValue) bestValue = newValue;
		}
		return bestValue;	
		
	}
	float TriStateGameBoard::MiniMaxAB(TriStateGameBoard board, float alpha, float beta, BoardState player, bool maximizngPlayer)
	{
		if (board.IsTerminalNode()) return board.EvaluateGameBoard(player);
		float bestValue = 100;
		if (maximizngPlayer) bestValue = -100;

		vector<TriStateGameBoard> newBoards = board.GetFutureBoards(player);
		for each (TriStateGameBoard newBoard in newBoards)
		{
			float newValue = MiniMaxAB(newBoard, alpha, beta, GetOtherPlayer(player), !maximizngPlayer);
			if (maximizngPlayer && newValue >= bestValue) bestValue = newValue;
			if (maximizngPlayer && newValue >= alpha) alpha = newValue;

			if (!maximizngPlayer && newValue <= bestValue) bestValue = newValue;
			if (!maximizngPlayer && newValue <= beta) beta = newValue;

			if (beta <= alpha) break;
		}
		return bestValue;
	}

	void TriStateGameBoard::DetermineBestBoardMove(BoardState player, int16_t & row, int16_t & col)
	{
		assert(!IsTerminalNode());

		vector<TriStateGameBoard> newBoards = GetFutureBoards(player);
		
		float bestValue = -100;

		for each (TriStateGameBoard newBoard in newBoards)
		{
			float newValue = MiniMaxAB(newBoard, -100.0f, 100.0f, player, true);
			
			if (newValue > bestValue)
			{
				bestValue = newValue;
				row = newBoard.GetLastRowMove();
				col = newBoard.GetLastColMove();
			}
		}
	}
	uint16_t TriStateGameBoard::GetBitPair(int16_t row, int16_t col)
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
	BoardState TriStateGameBoard::CompareStatesInRow(const int16_t x[], const int16_t y[], const int16_t n)
	{
		assert(n > 1);

		BoardState theState = GetBoardState(x[0], y[0]);

		if (theState == BoardState::Empty) return BoardState::Empty;
		else
		{
			for (int16_t i = 1; i < n; i++)
			{
				if (GetBoardState(x[i], y[i]) == BoardState::Empty ||
					GetBoardState(x[i], y[i]) != theState)
				{
					return BoardState::Empty;
				}
			}			
		}

		return theState;
		
	}
	TriStateGameBoard TriStateGameBoard::CopyBoard(TriStateGameBoard board)
	{
		TriStateGameBoard newBoard = TriStateGameBoard(board.GetRowsCount(), board.GetColsCount());

		for (int16_t row = 1; row <= GetRowsCount(); row++)
		{
			for (int16_t col = 1; col <= GetColsCount(); col++)
			{
				newBoard.SetBoardState(row, col, board.GetBoardState(row, col));
			}
		}

		return newBoard;
	}
	bool TriStateGameBoard::IsTerminalNode()
	{
		// check for winner
		if (GetWinner() != BoardState::Empty) return true;

		// check to see if there is anywhere else to place a piece
		vector<TriStateGameBoard> testBoards = GetFutureBoards(BoardState::Player1);
		if (testBoards.size() == 0) return true;

		// the above is false, so this is not a terminal node
		return false;
	}
	BoardState TriStateGameBoard::GetOtherPlayer(BoardState player)
	{
		if (player == BoardState::Player1) return BoardState::Player2;
		return BoardState::Player1;
	}
	float TriStateGameBoard::EvaluateGameBoard(BoardState player)
	{
		if (GetWinner() == player)
		{
			return 1.0f;
		}
		else if (GetWinner() == BoardState::Empty)
		{
			return 0.0f;
		}
		else
		{
			return -1.0f;
		}
		
	}
	vector<TriStateGameBoard> TriStateGameBoard::GetFutureBoards(BoardState player)
	{
		vector<TriStateGameBoard> newBoards;

		for (int16_t row = 1; row <= GetRowsCount(); row++)
		{
			for (int16_t col = 1; col <= GetColsCount(); col++)
			{
				if (GetBoardState(row, col) == BoardState::Empty)
				{
					// this is a potential new spot.
					
					TriStateGameBoard newBoard = CopyBoard((*this));
					newBoard.SetBoardState(row, col, player);

					newBoards.push_back(newBoard);
				}
			}
		}
		return newBoards;
	}


	BoardState TriStateGameBoard::GetWinner()
	{	
		for (int16_t i = 0; i < mNumWinways; i++)
		{
			BoardState theState = CompareStatesInRow(mWinCheckX[i], mWinCheckY[i], GetRowsCount());
			if (theState != BoardState::Empty) return theState;
		}
		return BoardState::Empty;
	}
}
