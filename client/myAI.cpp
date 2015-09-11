// sample clint which plays randomly c++

#include "stdc++.h"
#define FOR(i, n) for(int i=0; i<n; i++)

#define plusInfinity 100000.0
#define minusInfinity -10000.0
using namespace std;

int N;
char board[5][5];
int numVacantSpots;
int maxLevelToSearch;
float alpha,beta;
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
chaosMove nextChaosMoveAB;
orderMove nextOrderMoveAB;
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
int  calculateScore();
int  scoreHelp(int row);

float chance(int level);
float maxVal(int level); // Order's move
float minVal(int level,char color);  // As of now this is not keeping track of the which move lead to minimum but it should

float chanceAlphaBeta ( int level , float &alpha , float &beta );
float maxAlphaBeta( int level , float &alpha , float &beta  );
float minAlphaBeta( int level , float &alpha , float &beta ,  char color );
float evalOrder();
float evalChaos();
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
	maxLevelToSearch = 2;
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
 
chaosMove findChaosMove(char color) // Use something intelligent here
{
	alpha = minusInfinity;
	beta 	= plusInfinity ;
	float score1 = minAlphaBeta(maxLevelToSearch,alpha,beta,color);
	return nextChaosMoveAB;
	// float score2 = minVal(maxLevelToSearch,color);
	// fprintf(stderr, "Chaos:AB vs Minimax: %f,%f\n",score1,score2);	
	// return nextChaosMove;
}

orderMove findOrderMove()
{
	alpha = minusInfinity;
	beta 	= plusInfinity ;
	float temp1 = maxAlphaBeta(maxLevelToSearch,alpha,beta);	
	return nextOrderMoveAB;
	// float temp2 = maxVal(maxLevelToSearch);	
	// fprintf(stderr, "Order:AB vs Minimax: %f,%f\n",temp1,temp2);	
	// return nextOrderMove;
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
			// fprintf(stderr, "%s\n",message);
			// No move for chaos available
		}
	}
	// cin>>a>>b>>c>>d;   Probably no need to take input from the server as all the blocks in the board are filled so order cant make a move
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
			undoOrderMove(allMoves[iter]);
			if(tempMax>=maxSoFar)
			{
				maxSoFar = tempMax;
				bestChild = iter;
			}
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

float chanceAlphaBeta ( int level , float &alpha , float &beta )
{
	float expectation = 0.0;
	for(int i=0 ; i<N ; i++)
	{
		float tempProb = (float)(colors[i].numRemaining);
		tempProb = tempProb / ((float)numVacantSpots);
		if(tempProb!=0.0)	// No need to explore the tree for colors that are already exhausted
			expectation +=  tempProb * ( float )( minAlphaBeta ( level , alpha , beta  , colors[i].color ) );
	}
	return expectation;
}

float maxAlphaBeta( int level , float &alpha , float &beta  )
{
	if( level > 0 && numVacantSpots > 0 )
	{
		orderMove bestMove ;
		vector<orderMove> allMoves = getAllPossibleOrderMoves ( );
		float maxVal = minusInfinity ;
		for( int ind = 0 ; ind < allMoves.size ( ) ; ind++ )
		{
			makeOrderMove( allMoves[ ind ] );
			float tempVal =chanceAlphaBeta( level-1 , alpha , beta );
			undoOrderMove( allMoves[ ind ] );

			if( tempVal >= maxVal )
			{
				bestMove = allMoves[ ind ];
				maxVal = tempVal;
			}
 			if(beta <= alpha)
				break;

			if(alpha>maxVal)
 				alpha = maxVal;
		}
		if(level==maxLevelToSearch)
		{
			nextOrderMoveAB = bestMove;
			fprintf(stderr, "Returning from maxAlphaBeta\n");
		}
		return maxVal;
	}
	else if( numVacantSpots == 0 )
	{
		return ( float ) calculateScore ( ) ;
	}
	else
	{
		return ( float ) calculateScore ( ) ;		 
	}
}

float minAlphaBeta( int level , float &alpha , float &beta ,  char color )	
{
	if( level > 0 && numVacantSpots > 0 )
	{
		chaosMove bestMove ;
		chaosMove tempMove ;
		float minVal = plusInfinity;
		for( int row = 0 ; row < N ; row++ )
		{
			for( int col = 0 ; col < N ; col++ )
			{
				if( board[row][col] == '-' )
				{
					tempMove.x 		= row;
					tempMove.y 		= col;
					tempMove.color  = color;

					makeChaosMove(tempMove);
					float tempMin = maxAlphaBeta( level-1 , alpha , beta );
					undoChaosMove( tempMove );
					if(tempMin <= minVal)
					{
						minVal 		= tempMin;
						bestMove	= tempMove;
					}
					if(beta<=alpha)
					{
						row = N;
						col = N;
						break;
					}
					if(beta<minVal)
						beta = minVal;
					
				}
			}	
		}
		if(level==maxLevelToSearch)
		{
			nextChaosMoveAB = bestMove;
			fprintf(stderr, "Returning from minAlphaBeta\n");
		}
		return minVal;
	}
	else if( numVacantSpots == 0 )
	{
		return ( float ) calculateScore ( ) ;
	}
	else
	{
		/*
		 *this happens when level barrier reach apply suitable heuristics to proceed further
		 */
		 return ( float ) calculateScore ( ) ;		 
		 //throw( "cutoff not configured !! make level = numVacantSpots" );

	}
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


/*  CHANGES  NISHANT... 10 Sep Night...Start*/


int numRowColWithRandomColors()
{
	std::vector<int> numOccurence;
	numOccurence.resize(N,0);
	int num=0;
	bool flag = false;
	char c;
	// Finding number of favorable rows
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			c = board[i][j];
			if(c!='-')
				numOccurence[((int)c)-65]++;
		}
		for(int k=0;k<N;k++)
		{
			if(numOccurence[k]>2)
			{
				flag = true;
				break;
			}
		}
		numOccurence.resize(N,0);
		if(!flag)
		{
			num++;
			flag=false;
		}
	}
	// Finding number of favorable colums
	flag = false;
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			c = board[j][i];
			if(c!='-')
				numOccurence[((int)c)-65]++;
		}
		for(int k=0;k<N;k++)
		{
			if(numOccurence[k]>2)
			{
				flag = true;
				break;
			}
		}
		if(!flag)
		{
			num++;
			flag=false;
		}
	}
	return num++;
}

int numTilesInCentralPlus()
{
	int middle = N/2;
	int ctr = 0;
	for(int i=0;i<N;i++)
	{
		if(board[i][middle]!='-')
			ctr++;
		if(board[middle][i]!='-')
			ctr++;
	}
	return ctr;
}

int getNumberOfPossibleOrderMoves(int x, int y)
{
	int possibleMoves=0;
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
			possibleMoves++;
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
			possibleMoves++;
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
			possibleMoves++;
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
			possibleMoves++;
		}
		else
			break;
	}
	return possibleMoves;
}

int getNumberOfAllPossibleOrderMoves()
{
	int allMoves=0;
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			if(board[i][j]!='-')
			{
				allMoves+= getNumberOfPossibleOrderMoves(i,j);
			}
		}
	}
	return allMoves;
}

int separationBetweenTiles()// Returns manhattan distane between every pair of tiles
{
	int numIter = N*N- numVacantSpots;
	std::vector<int> xCord;
	std::vector<int> yCord;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			if(board[i][j]!='-')
			{
				xCord.push_back(i);
				yCord.push_back(j);
			}
		}
	}
	int dist =0;
	for (int i = 0; i < numIter; ++i)
	{
		for (int j = i+1; j < numIter; ++j)
		{
			dist+= abs(xCord[i]-xCord[j])+abs(yCord[i]-yCord[j]);
		}
	}
	return dist;
}

int numBadConsequtiveTiles()
{
	int ctr=0;
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N-3; ++j)
		{
			char c1,c2,c3;
			c1 = board[i][j+0];
			c2 = board[i][j+1];
			c3 = board[i][j+2];
			if(c1!='-' && c2!='-' && c3!='-' && c1!=c2 && c2!=c3)
			{
				ctr++;
			}
		}
	}
	for (int i = 0; i < N-3; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			char c1,c2,c3;
			c1 = board[i+0][j];
			c2 = board[i+1][j];
			c3 = board[i+2][j];
			if(c1!='-' && c2!='-' && c3!='-' && c1!=c2 && c2!=c3)
			{
				ctr++;
			}
		}
	}
	return ctr;
}

float evalChaosStart() // To be used for first few moves in the game
{
	std::vector<float> features;
	std::vector<float> weights;
	int numFeatures = 5;
	features.resize(numFeatures,0.0);
	weights.resize(numFeatures,0.0);
	features[0] = numRowColWithRandomColors();
	weights[0]  = 1.0;
	
	features[1] = separationBetweenTiles(); // Use this feature for early moves of chaos
	weights[1]	= 1.0;

	float score;
	for (int i = 0; i < numFeatures; ++i)
	{
		score+= weights[i]*features[i];
		/* code */
	}
	return score;
}

float evalChaosMiddle()// To be used for middle part of the game
{
	// since we are in a minimisation world.. these weights must be negative
	std::vector<float> features;
	std::vector<float> weights;
	int numFeatures =10 ;
	features.resize(numFeatures,0.0);
	weights.resize(numFeatures),0.0;
	features[0] = numRowColWithRandomColors();
	weights[0]  = 1.0;
	features[1] = calculateScore();
	weights[1]	= 1.0;
	
	features[2] = numTilesInCentralPlus();
	weights[2]	= 1.0;

	// Use this feature for later half of the game
	features[3] = getNumberOfAllPossibleOrderMoves();
	weights[3]	= 1.0;

	features[4] = separationBetweenTiles(); // Use this feature for early moves of chaos
	weights[4]	= 1.0;

}

float evalChaosEnd()// To be used near death
{
	return calculateScore();	
}

float evalOrderStart() // To be used for first few moves in the game
{
	// if any move brings more than three tiles together even when they arent making any palindrome then 
	// then that move is certainly not good.. Make a function to measure this intuition... Bringing two colors together is OK
	float f1 = (float)numBadConsequtiveTiles();
	return (-1)*f1;
}

float evalOrderMiddle()// To be used for middle part of the game
{
	// since we are in a minimisation world.. these weights must be negative
	std::vector<float> features;
	std::vector<float> weights;
	int numFeatures=10;
	features.resize(numFeatures,0.0);
	weights.resize(numFeatures,0.0);
	features[0] = numRowColWithRandomColors();
	weights[0]  = 1.0;
	features[1] = calculateScore();
	weights[1]	= 1.0;
	
	features[2] = numTilesInCentralPlus();
	weights[2]	= 1.0;

	// Use this feature for later half of the game
	features[3] = getNumberOfAllPossibleOrderMoves();
	weights[3]	= 1.0;

	features[4] = separationBetweenTiles(); // Use this feature for early moves of Order
	weights[4]	= 1.0;

	float score;
	for (int i = 0; i < numFeatures; ++i)
	{
		score += weights[i]*features[i];
	}
	return score;
}

float evalOrderEnd()// To be used near death
{
	return calculateScore();
}

/*  CHANGES  NISHANT... 10 Sep Night...End*/