#pragma once
#include <memory>
#include <vector>


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
		TriStateGameBoard(int rows, int cols);

		void Print();

		BoardState GetBoardState(int x, int y);
		BoardState SetBoardState(int x, int y);

		char BoardStateToChar(BoardState b);

		//evalute game board from perspective of given player
		float EvaluateGameBoard(BoardState player);

		// this needs to return a vector of possible game states from this position, given
		// a particular players turn
		shared_ptr<vector<TriStateGameBoard>> GetFutureBoards;



	private:
		char player1Icon = 'X';
		char player2Icon = 'O';
		char emptyIcon = ' ';

		shared_ptr<vector<char>> board;
	};
}

