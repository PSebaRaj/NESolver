#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
	string fname;
	cout << "Enter the file name: ";
	cin >> fname;

	vector<vector<float> > row_player_payoffs;
	vector<float> row;
	string line, word;

	fstream file (fname, ios::in);

	if(file.is_open()) {
		while(getline(file, line)) {
			row.clear();

			stringstream str(line);

			while(getline(str, word, ','))
				row.push_back(std::stof(word));
			row_player_payoffs.push_back(row);
		}
	}
	else {
		cout << "Could not open the file\n";
	}

	// print contents of payoff matrix
	for(int i=0;i < row_player_payoffs.size(); i++) {
		for(int j=0; j < row_player_payoffs[i].size(); j++) {
			cout << row_player_payoffs[i][j] << " ";
		}

		cout << "\n";
	}

	return 0;
}
