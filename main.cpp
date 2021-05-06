#include <iostream>
#include <fstream>
#include <ctime>

using namespace std;

enum state {hidden, found};

struct FieldState {
	char _value;
	state status;
};

FieldState initfield(int range) {
	FieldState tmp;
	tmp._value = 'a'+(rand() % range);
	tmp.status = hidden;
	return tmp;
}

struct GameState {
	int _rowCount;
	int _colCount;
	FieldState** Table;
	int _guessCount;
	int* _charcount;
};

void initgame(GameState& g) {
	int range = (g._rowCount*g._colCount)/2;
	g._charcount = new int[range];
	g.Table = new FieldState*[g._rowCount];
	for(int i=0; i<g._rowCount; i++) {
		g.Table[i] = new FieldState[g._colCount];
		for(int j=0; j<g._colCount; j++) {
			FieldState tmpval;
			do {
				tmpval = initfield(range);
			} while(g._charcount[tmpval._value-'a']==2);
			g.Table[i][j] = tmpval;
			g._charcount[tmpval._value-'a']++;			
		}
	}
}

char statetotext(FieldState& f) {
	if(f.status==found) return f._value;
	return '*';
}

void printgame(GameState& g) {
	for(int row=0; row<g._rowCount; row++) {
		for(int col=0; col<g._colCount; col++) {
			cout << statetotext(g.Table[row][col]);
		}
		cout << endl;
	}
}

bool validinput(GameState& g,int row, int col) {
	if(row<1 || row>7 || col<1 || col>7) return false;
	if(g.Table[row-1][col-1].status == found) return false;
	return true;
}

void guess(GameState& g) {
	int limit = 0;
	int row[2];
	int col[2];
	while(limit<2) {
		cout << "Row: "; cin >> row[limit];
		cout << " Col: "; cin >> col[limit]; cout << endl;
		if(validinput(g, row[limit], col[limit])) {
			g.Table[row[limit]-1][col[limit]-1].status = found;
			printgame(g);
			limit++;
		}
	}
	if(g.Table[row[0]-1][col[0]-1]._value!=
		g.Table[row[1]-1][col[1]-1]._value) 
	{
		g.Table[row[0]-1][col[0]-1].status = hidden;
		g.Table[row[1]-1][col[1]-1].status = hidden;
	}
	for(int i=0; i<100; i++) cout << endl;
}

bool win(GameState& g) {
	int foundcount = 0;
	for(int row=0; row<g._rowCount; row++) {
		for(int col=0; col<g._colCount; col++) {
			if(g.Table[row][col].status == found) foundcount++;
		}
	}
	if(foundcount == (g._rowCount * g._colCount)) return true;
	return false;
}

int main(int argc, char** argv) {
	srand(time(NULL));
	GameState game;
	cout << "Rows: "; cin >> game._rowCount;
	cout << "Cols: "; cin >> game._colCount;
	initgame(game);
	printgame(game);
	while(!win(game)) {
		guess(game);
		printgame(game);
	}
	cout << "\nYou won the game\n";
}
