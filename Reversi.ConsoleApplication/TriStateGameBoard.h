#pragma once
#include <memory>
#include <vector>
#include <iostream>

namespace TicTacToe
{
	using namespace std;

	enum BoardState
	{
		Player1,
		Player2,
		Empty,
	};

	class TriStateGameBoard
	{
	public:
		TriStateGameBoard() = delete;
		TriStateGameBoard(int16_t rows, int16_t cols);

		void Print();

		BoardState GetBoardState(int16_t row, int16_t col);
		bool SetBoardState(int16_t row, int16_t col, BoardState state);		

		//evalute game board from perspective of given player
		float EvaluateGameBoard(BoardState player);

		// this needs to return a vector of possible game states from this position, given
		// a particular players turn
		shared_ptr<vector<TriStateGameBoard>> GetFutureBoards;

		BoardState GetWinner();
		BoardState GetCurrentPlayer() { return mCurrentPlayer; };
		BoardState SetCurrentPlayer(BoardState player) { mCurrentPlayer = player; };
		int16_t GetRowsCount() { return mRowsCount; };
		void SetRowsCount(int16_t rows) { mRowsCount = rows; };
		int16_t GetColsCount() { return mColsCount; };
		void SetColsCount(int16_t cols) { mColsCount = cols; };

	private:
		uint16_t GetBitPair(int16_t row, int16_t col);
		char mPlayer1Icon = 'X';
		char mPlayer2Icon = 'O';
		char mEmptyIcon = ' ';
		BoardState mCurrentPlayer = BoardState::Empty;
		int16_t mRowsCount = 0;
		int16_t mColsCount = 0;
		vector<uint16_t> mBoard;
		char BoardStateToChar(BoardState b);
		BoardState CompareStatesInRow(const int16_t x[], const int16_t y[], const int16_t n);
		const int16_t mWinCheckX[8][3] = { { 1,1,1 },{ 2,2,2 },{ 3,3,3 },{ 1,2,3 },{ 1,2,3 },{ 1,2,3 },{ 1,2,3 },{ 3,2,1 } };
		const int16_t mWinCheckY[8][3] = { { 1,2,3 },{ 1,2,3 },{ 1,2,3 },{ 1,1,1 },{ 2,2,2 },{ 3,3,3 },{ 1,2,3 },{ 1,2,3 } };
		const int16_t mNumWinways = 8;

	};
}

