// sample clint which plays randomly c++

#include "stdc++.h"
#define FOR(i, n) for(int i=0; i<n; i++)
using namespace std;

int N;
char board[5][5];

struct orderMove
{
	// move from x1,y1 to x2,y2
	int x1;
	int y1;
	int x2;
	int y2;
};

struct chaosMove
{
	int x;
	int y;
	char color;
};

void init() ;
vector< orderMove > getPossibleOrderMoves(int x, int y);
void makeChaosMove(chaosMove move); // Executes chaos' move
void makeOrderMove(orderMove move); // Executes order's move
chaosMove findChaosMove(char color);
orderMove findOrderMove();
void playAsOrder();
void playAsChaos();
bool isOk(int x,int row);
int calculateScore();
int scoreHelp(int row);
bool isGameOver();

int main() 
{
	cin>>N;	init();
	string s;	cin>>s;
	// fprintf(stdout, "Testing correctnes\n");
	if (s.compare("ORDER") == 0) {
		playAsOrder();
	} else {
		playAsChaos();
	}
	return 0;
}

void init() 
{
	FOR(i, N){
		FOR(j, N){
			board[i][j] = '-';
		}
	}
}

void makeChaosMove(chaosMove move) // Executes chaos' move
{
	//
	board[move.x][move.y] = move.color;
}

void makeOrderMove(orderMove move)// Executes order's move
{
	if(board[move.x2][move.y2]=='-')
	{
		if(move.x1 != move.x2 || move.y1 != move.y2)
		{	
			board[move.x2][move.y2] = board[move.x1][move.y1];
			board[move.x1][move.y1] = '-';
		}	
	}
}
 
chaosMove findChaosMove(char color) // Use something intelligent here
{
	chaosMove move;
	move.x = 0;
	move.y = 0;
	move.color = color;
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			if (board[i][j] == '-')
			{
				move.x 		= i;
				move.y 		= j;
				move.color 	= color;
				return move;
			}
		}
	}
	throw "No Move for chaos\n";
}

vector< orderMove > getPossibleOrderMoves(int x, int y)
{
	vector< orderMove >	possibleMoves;
	orderMove temp;
	temp.x1=x;
	temp.y1=y;
	int i=0;
	for(i=x+1;i<N;i++) // Looking for moves ahead of the tile
	{
		if(board[i][y]=='-')
		{
			temp.x2 = i;
			temp.y2 = y;
			possibleMoves.push_back(temp);
		}
		else
			break;
	}

	for(i=x-1;i>=0;i--) // Looking for moves behind the tile
	{
		if(board[i][y]=='-')
		{
			temp.x2 = i;
			temp.y2 = y;
			possibleMoves.push_back(temp);
		}
		else
			break;
	}
	for(i=y+1;i<N;i++) // Looking for moves on top of the tile
	{
		if(board[x][i]=='-')
		{
			temp.x2 = x;
			temp.y2 = i;
			possibleMoves.push_back(temp);
		}
		else
			break;
	}

	for(i=y-1;i>=0;i--) // Looking for moves below the tile
	{
		if(board[x][i]=='-')
		{
			temp.x2 = x;
			temp.y2 = i;
			possibleMoves.push_back(temp);
		}
		else
			break;
	}
	return possibleMoves;
}

orderMove findOrderMove()
{
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(board[i][j]!='-')
			{
				std::vector< orderMove > possibleMoves = getPossibleOrderMoves(i,j);
				if(possibleMoves.size()>0)
				{
					return possibleMoves[0];
				}
			}
		}
	}
	throw "No Move available\n";
}

void playAsOrder() 
{
	orderMove oMove;
	chaosMove cMove;
	for(int i = 0; i < N*N ;i++) 
	{
		// First read in the move made by chaos
		cin>> cMove.x >> cMove.y >> cMove.color;
		makeChaosMove(cMove);
		// Now find an intelligent move to make for chaos
		try
		{
			oMove = findOrderMove();
			makeOrderMove(oMove);
			cout<<oMove.x1<<' '<<oMove.y1<<' '<<oMove.x2<<' '<<oMove.y2<<endl;
		}
		catch(string message)
		{
			// No move available for order to make
		}
	}
	// fstream file;
	// file.open("outPut.txt",ios::out|ios::app);
	// file<<calculateScore();
	// file.close();
}

void playAsChaos() 
{
	orderMove oMove;
	chaosMove cMove;
	int a, b, c, d; char color;
	cin>>color;
	cout<<"0 0"<<endl;	// First move :D
	board[0][0] = color;
	for(int i=0;i<N*N-1;i++)
	{
		cin >> oMove.x1 >> oMove.y1 >> oMove.x2 >> oMove.y2;
		cin >> color;
		makeOrderMove(oMove); // on behalf of server
		try
		{
			cMove = findChaosMove(color);
			makeChaosMove( cMove );		
			cout<<cMove.x <<" "<<cMove.y <<endl;
		}
		catch(string message)
		{
			// No move for chaos available
		}
	}
	// cin>>a>>b>>c>>d;   Probably no need to take input from the server as all the blocks in the board are filled so order cant make a move


	// fstream file;
	// file.open("outPut.txt",ios::out|ios::app);
	// file<<calculateScore();
	// file.close();
}

bool isGameOver()
{
	for (int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			if (board[i][j] == '-')
				return false;
		}
	}
	return true;
}

bool isOk(int x,int row)
{
	if(x>=0 && x<N && board[row][x]!='-')
		return true;
	else
		return false;
}

int scoreHelp(int row)
{
	int MAX = N;
	int	score = 0;
	int length=0,scoreX=0,right,left;
	for(int i=0;i<N;i++)
	{
		length = 0;
		scoreX = 0;
		right = i;
		left = i-1;
		while( isOk(right,row) && isOk(left,row) && board[row][left]==board[row][right])
		{
			scoreX += (length+2); 
			length += 2; 
			right += 1; 
			left -= 1;
		}
		score+=scoreX;

		length = 1 ;
		scoreX = 0 ;
		right = i + 1 ;
		left = i - 1 ;
		while(isOk(right,row) && isOk(left,row) && board[row][left] == board[row][right])
		{
			scoreX += (length + 2);
			length += 2;
			right += 1;
			left -= 1;
		}
		score += scoreX;
	}
	return score;
}

int calculateScore()
{
	int score = 0;
	for(int i=0;i<N;i++)
	{
		score += scoreHelp(i);
	}
	for(int i=0;i<N;i++)
	{
		for(int j=i+1;j<N;j++)
		{
			int temp;
			temp = board[i][j];
			board[i][j] = board[j][i];
			board[j][i] = temp;
		}
	}
	
	for(int i=0;i<N;i++)
	{
		score += scoreHelp(i);
	}
	for(int i=0;i<N;i++)
	{
		for(int j=i+1;j<N;j++)
		{
			int temp;
			temp = board[i][j];
			board[i][j] = board[j][i];
			board[j][i] = temp;
		}
	}
	return score;
}