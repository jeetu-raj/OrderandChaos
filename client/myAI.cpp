// sample clint which plays randomly c++

#include "stdc++.h"
#define FOR(i, n) for(int i=0; i<n; i++)
#define maxLevelToSearch 2
using namespace std;

int N;
char board[5][5];
int numVacantSpots;


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

struct colorInfo
{
	int numRemaining;
	char color;
};

orderMove nextOrderMove;
chaosMove nextChaosMove;
std::vector<colorInfo> colors;
void print();
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
float chance(int level);
float maxVal(int level); // Order's move
float minVal(int level,char color);  // As of now this is not keeping track of the which move lead to minimum but it should

bool isGameOver();

int main() 
{
	cin>>N;	
	init();
	string s;	cin>>s;
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
	numVacantSpots= N*N;
	colors.resize(N);
	for(int i = 0;i<N;i++)
	{
		colors[i].numRemaining 	= N;
		colors[i].color 		= (char)(65+i);
	}
}

void makeChaosMove(chaosMove move) // Executes chaos' move
{
	if(board[move.x][move.y]=='-' && move.color!='-')  // Just to make sure that chaos' move was valid
	{
		numVacantSpots--;	
		board[move.x][move.y] = move.color;
		colors[((int)(move.color)-65)].numRemaining--;
	}
	else
	{
		fprintf(stderr, "IllegalMove by Chaos at: %d , %d ::%c-->%c\n",move.x,move.y,move.color,board[move.x][move.y]);
	}
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
	else
	{
		fprintf(stderr, "Illegal Move made by order (Dest not empty): %d ,%d ,%d ,%d\n",move.x1,move.y1,move.x2,move.y2);
	}
	if(move.x1 != move.x2 && move.y1 != move.y2)
	{
		fprintf(stderr, "Illegal Move made by order (Not in same row/column): %d ,%d ,%d ,%d\n",move.x1,move.y1,move.x2,move.y2);	
	}
}

void undoOrderMove(orderMove move)// Test if it works
{
	int temp;
	temp 	= move.x1;
	move.x1 = move.x2;
	move.x2 = temp;

	temp 	= move.y1;
	move.y1 = move.y2;
	move.y2 = temp;
	// fprintf(stderr, "Undoing order's move..\n");
	makeOrderMove(move);
}

void undoChaosMove(chaosMove move)
{
	if(board[move.x][move.y]!='-' && board[move.x][move.y]==move.color)  // Just to make sure that chaos' move is undone properly..
	{
		numVacantSpots++;
		char c = board[move.x][move.y];
		colors[((int)(c)-65)].numRemaining++;
		board[move.x][move.y] = '-';
	}
	else
	{
		fprintf(stderr, "Invalid undoing chaos move at: %d,%d colors: %c-->%c\n",move.x,move.y,move.color,board[move.x][move.y]);
	}
}
 
chaosMove findChaosMove(char color) // Use something intelligent here
{
	float score = minVal(maxLevelToSearch,color);
	fprintf(stderr, "MinScore:%f\n",score );
	return nextChaosMove;
	// chaosMove move;
	// move.x = 0;
	// move.y = 0;
	// move.color = color;
	// for(int i=0;i<N;i++)
	// {
	// 	for(int j=0;j<N;j++)
	// 	{
	// 		if (board[i][j] == '-')
	// 		{
	// 			move.x 		= i;
	// 			move.y 		= j;
	// 			move.color 	= color;
	// 			return move;
	// 		}
	// 	}
	// }
	// throw "No Move for chaos\n";
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
	float temp = maxVal(maxLevelToSearch);	
	fprintf(stderr, "MaxScore:%f\n",temp );
	return nextOrderMove;
	// for(int i=0;i<N;i++)
	// {
	// 	for(int j=0;j<N;j++)
	// 	{
	// 		if(board[i][j]!='-')
	// 		{
	// 			std::vector< orderMove > possibleMoves = getPossibleOrderMoves(i,j);
	// 			if(possibleMoves.size()>0)
	// 			{
	// 				return possibleMoves[0];
	// 			}
	// 		}
	// 	}
	// }
	// throw "No Move available\n";
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

void playAsChaos()// Replace first mve with some intelligent start point 
{
	orderMove oMove;
	chaosMove cMove;
	int a, b, c, d; char color;
	cin>>color;	
	cout<<"0 0"<<endl;	// First move :D
	board[0][0] = color;
	numVacantSpots--;
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
			fprintf(stderr, "%s\n",message);
			// No move for chaos available
		}
	}
	// cin>>a>>b>>c>>d;   Probably no need to take input from the server as all the blocks in the board are filled so order cant make a move
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

vector<orderMove> getAllPossibleOrderMoves()
{
	std::vector<orderMove> allMoves;
	allMoves.resize(0);
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(board[i][j]!='-')
			{
				std::vector<orderMove> temp = getPossibleOrderMoves(i,j);
				allMoves.insert(allMoves.begin(),temp.begin(),temp.end());
			}
		}
	}
	return allMoves;
}


float maxVal(int level) // Order's move
{
	if(level > 0 && numVacantSpots>0)// Leaf node is not reached at
	{
		vector<orderMove> allMoves = getAllPossibleOrderMoves();
		float maxSoFar = 0 ,tempMax = 0;
		int bestChild  = -1;
		int size 	   = allMoves.size();
		for(int iter=0 ; iter<size ; iter++)
		{ 	
			makeOrderMove(allMoves[iter]);
			tempMax = chance(level-1);// Now iterate over children
			if(tempMax>=maxSoFar)
			{
				// store the solution
				maxSoFar = tempMax;
				bestChild = iter;
			}
			undoOrderMove(allMoves[iter]);
		}
		nextOrderMove = allMoves[bestChild];
		return maxSoFar;
	}
	else if(numVacantSpots == 0) // Now we'll use scoring fuction instead of heuristic based eval function
	{
		return (float)calculateScore();
	}
	else // Use the eval funtion tha makes use of various features
	{

		return (float)calculateScore(); // This has to be replaced with feature based function
	}
}

float minVal(int level,char color)  // As of now this is not keeping track of the which move lead to minimum but it should
{
	if(level>0 && numVacantSpots>0)
	{
		chaosMove tempMove;
		float minSoFar = 100000.0;
		float tempMin  = 100000.0;
		chaosMove bestMove;
		for(int i=0;i<N;i++)
		{
			for(int j=0;j<N;j++)
			{
				if(board[i][j]=='-')
				{
					tempMove.x 		= i;
					tempMove.y 		= j;
					tempMove.color  = color;
					makeChaosMove(tempMove);
					tempMin = maxVal(level-1);
					if(tempMin <= minSoFar)
					{
						minSoFar = tempMin;
						bestMove = tempMove;
					}
					undoChaosMove(tempMove);
				}
			}
		}
		nextChaosMove = bestMove; // This is to store the next move to make by the player ..
		return minSoFar;
	}
	else if(numVacantSpots==0) // It is the leaf node so does not have to do any min max
	{
		return (float)calculateScore();
	}else // in case of cutoff at intermediate level , we have to use feature based function in this case
	{
		return (float)calculateScore();
	}
}

float chance(int level)
{
	float expectation = 0.0;
	for(int i=0 ; i<N ; i++)
	{
		// Note that here we are not decrementing value of level as it makes no sense to cutoff at a chance node
		float tempProb = (float)(colors[i].numRemaining);
		tempProb = tempProb / ((float)numVacantSpots);
		expectation +=  tempProb * (float)(minVal(level, colors[i].color));
	}
	return expectation;
}

int expectiMiniMax(string nodeType,int level) // No additional parameter need to be passed as the configuration of 
									 // board is changed and undo after each iteration of sub-tree
{
	/*
	TODO -- start maxSoFar , minSoFar from infinity .. define infinity
	  Return also the configuration that returned the most optimal path.. This would be necessary in making actual move
	  Also keep track of how many levels further down do i need to go to

	  level cant get down to 0 when the node for which expectiminimax is being called is chance node
	*/	
	  return 1;
}

void print()
{
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			fprintf(stderr, "%c ",board[i][j]);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "-----------------------------------");
}