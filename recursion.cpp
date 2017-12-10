// recursionMaze.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <vector>

//_____________________
//Nathanael Fixx
//CSS 342 - Project 4
//18 hours
//Cygwin
//_____________________

using namespace std;

//_____________________________________________________________________________
//Declaring functions
vector<vector<char>> makeMaze(int rowNum, int colNum, string filename);
void printMaze(vector<vector<char>> maze, int rowNum, int colNum);
bool move(int row, int col, int rowNum, int colNum, vector<vector<char>> &maze,
		vector<int> pathRow, vector<int> pathCol);

void printPath(vector<int> pathRow, vector<int> pathCol);
void checkPossible(int row, int col, vector<vector<char>> maze);
bool isExit(int rowNum, int colNum, vector<vector<char>> maze);
bool isStart(int rowNum, int colNum, int row, int col,
	       	vector<vector<char>> maze);

bool isSize(int rowNum, int colNum, string filename, int &rowCount, 
		int &colCount);
//_______________________________________________________________________________


int main(int argc, char *argv[])
{
	int rowNum; //number of rows in maze
	int colNum; //number of columns in maze
	int row; //starting row
	int col; //starting column
	int colCount = -1;
	int rowCount = 0;
	string filename(argv[1]);


	vector<int> pathRow, pathCol;

	ifstream fin;
	fin.open(filename);

	if (fin.fail()) //checks if the file opens
	{
		cout << "failed";
	}

	//getting the first four numbers in the file
	fin >> rowNum;
	fin >> colNum;
	fin >> row;
	fin >> col;

	isSize(rowNum, colNum, filename, rowCount, colCount);
        
	//checks if the given dimensions were too big
	if (rowCount < rowNum || colCount < colNum) {
	    cout << "The Given Dimensions Do Not Match The Actual Dimensions";
	    cout << endl;
		return 0;
		}
	
	rowNum = rowCount;
	colNum = colCount;
 

		vector<vector<char>> maze = makeMaze(rowNum, colNum, filename);
		 
		//checks if the maze has a valid entry and exit
		if (isStart(rowNum, colNum, row, col, maze) == true) {

		   if (isExit(rowNum, colNum, maze) == true) {
			move(row, col, rowNum, colNum, maze, pathRow, pathCol);
				checkPossible(row, col, maze);
			}
			else {
				cout << "No exit, Sartre was right" << endl;
			}
		}

	   else {
	     cout << "The Starting Point Is Not In A Valid Location" << endl;
		}

	return 0;
}

//_______________________________________________________________________
//Creating the maze
//
//reads in data from a file and pushes it into a two dimensional vector
//based off of the dimensions at the top of the file. 
//_______________________________________________________________________
vector<vector<char>> makeMaze(int rowNum, int colNum, string filename) {
	int count = 0;
	ifstream fin;
	fin.open(filename);
	char fill = ' ';
	fin >> noskipws;

	while (fill != '\n') {	//gets past the line that was already 
            fin >> fill;        //read in main
	}

	vector<vector<char>> maze(rowNum, vector<char>(colNum));

	for (int i = 0; i < rowNum; i++) {

		for (int j = 0; j < colNum; j++) {
			fin >> fill;
			maze[i][j] = fill;
		}
		fin >> fill;
			
			
		    while (fill != '\n') {    //avoids additional whitespace
		    fin >> fill;              //after each row
		}
         }

	return maze;
}

//___________________________________________________________________
//Printing the maze
//
//loops through the two dimensional vector and prints out the char
//in each element.
//___________________________________________________________________
void printMaze(vector<vector<char>> maze, int rowNum, int colNum) {

	cout << endl;
	for (int i = 0; i < rowNum; i++) {
		for (int j = 0; j < colNum; j++) {
			cout << maze[i][j];
		}
		cout << endl;
	}
	cout << endl;

}

//_____________________________________________________________________________
//Moving the player
//
//Checks if the function can be called with certain peramaters. If it can, the
//function is called, if not, the program tries to call the function again
//with different peramaters. If none of the options are possible, the program
//moves back one attempt and tries different options.
//_____________________________________________________________________________
bool move(int row, int col, int rowNum, int colNum, vector<vector<char>> &maze,
	       	vector<int> pathRow, vector<int> pathCol)
{

	if (row > -1 && row < rowNum  && col > -1 && col < colNum && 
			maze[row][col] != '#' && maze[row][col] != '+') {
		
		if (maze[row][col] == 'o') {    //if the char in the array
			pathRow.push_back(row); //is an 'o' the end of the	
			pathCol.push_back(col); //the end of the maze has
			printMaze(maze, rowNum, colNum); //been found
			printPath(pathRow, pathCol);
			return true; 
		}
		pathRow.push_back(row); //every time the program places a 
		pathCol.push_back(col); //'+' it also updates the path vectors
		maze[row][col] = '+';
	}

	else {
		return false;
	}

	if (move(row - 1, col, rowNum, colNum, maze, pathRow, pathCol)) {
		return true;
	}

	if (move(row + 1, col, rowNum, colNum, maze, pathRow, pathCol)) {
		return true;
	}

	if (move(row, col - 1, rowNum, colNum, maze, pathRow, pathCol)) {
		return true;
	}

	if (move(row, col + 1, rowNum, colNum, maze, pathRow, pathCol)) {
		return true;
	}

	maze[row][col] = ' '; //if no moves are possible, replaces the last
	return false;         //'+' with a whitespace

}

//______________________________________________________________________
//Printing out the path taken
//
//loops through the pathRow and pathCol vectors and outputs the values
//int the form [row, column]
//______________________________________________________________________
void printPath(vector<int> pathRow, vector<int> pathCol) {

	cout << "Path taken in the form [row, column]" << endl;

	for (int i = 0; i < pathRow.size() - 1; i++) {
		cout << "[" << pathRow[i] << "," << pathCol[i] << "]";
		cout << "->";
	}

	//gets the last move without the -> after
	cout << "[" << pathRow[pathRow.size() - 1] << ","; 
	cout << pathCol[pathRow.size() - 1] << "]";
	cout << endl;
}

//_______________________________________________________________
//checks if the maze was completed
//_______________________________________________________________
void checkPossible(int row, int col, vector<vector<char>> maze)
{
	//if the maze has been completed, the current location
	//should be a 'o' if it isn't that means that all
	//posibilities were attempted and the exit was not found
	if (maze[row][col] == ' ') { 
		cout << "No exit, Sartre was right" << endl;
	} 
}

//______________________________________________________________
//checks if the exit is in a valid location
//
//returns true if it can find a 'o' character in the maze vector
//______________________________________________________________
bool isExit(int rowNum, int colNum, vector<vector<char>> maze)
{

	for (int i = 0; i < rowNum; i++) {

		for (int j = 0; j < colNum; j++) {
			if (maze[i][j] == 'o') {
				return true;
			}
		}
	}
	return false;
}

//______________________________________________________________________________
//checks if the starting point is in a valid location
//______________________________________________________________________________
bool isStart(int rowNum,int colNum, int row, int col, vector<vector<char>>maze)
{
	if (row > rowNum || row < 0 || col > colNum || col < 0 
			|| maze[row][col] == '#') {
		return false;
	}

	else
		return true;
}

//__________________________________________________
//checks if the dimensions given at the top of the 
//file match the actual dimensions of the maze
//__________________________________________________
bool isSize(int rowNum, int colNum, string filename, int &rowCount,
	       	int &colCount)
{
	int checkCol = 0;
	ifstream fin;
	ifstream fin2;
	fin.open(filename); //for colCount
	fin2.open(filename);//for rowCount
	char skip = ' ';
	char getRow = ' ';
	fin >> noskipws;
	fin2 >> noskipws;

	//calculating colCount	
	while (skip != '\n') {	//gets past the first line of the file
		fin >> skip;
	}
	fin >> skip;
	while (skip != '\n') {  //gets past the second line of the file
		fin >> skip;
	}
		
	for (char fill = ' '; fill != '\n'; fin >> fill) {
			colCount++;
		}
	////////////////////////


	//calculating rowCount
	skip = ' ';
	while (skip != '\n') {	//gets past the first line of the file
		fin2 >> skip;
	}

	
	while (!fin2.eof()) {
		while (getRow != '\n') {
			fin2 >> getRow;
			checkCol++;
		}
	      if (checkCol > 1){		
		rowCount++;
	      }
		checkCol = 0;
		fin2 >> getRow;
		checkCol++;
	}
	//////////////////////////////

	colCount -= 1;
	rowCount -= 1;

	if (colCount != colNum || rowCount != rowNum) { 
		return false;
	}

	return true;
}

