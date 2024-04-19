#pragma once

using namespace std;

void writeToState(int state) {
	string mode;
	if (mode2D) mode = "2D_"; else mode = "3D_";
	string filename = "assets/states/state" + mode + to_string(state) + ".dat";
	ofstream file(filename);
	if (mode2D) {
		for (int row = 0; row < gridRows; row++) {
			for (int col = 0; col < gridCols; col++) {
				file << grid[col][row];
			}
		}
	}
	else {	
		for (int layer = 0; layer < gridLayers3; layer++) {
			for (int row = 0; row < gridRows3; row++) {
				for (int col = 0; col < gridCols3; col++) {
					file << grid3[col][row][layer];
				}
			}
		}
	}
	file.close();
}

void readFromState(int state) {
	string mode;
	if (mode2D) mode = "2D_"; else mode = "3D_";
	string filename = "assets/states/state" + mode + to_string(state) + ".dat";
	ifstream file;
	file.open(filename);
	if (file.fail()) {
		string s = "Error opening file :"  + filename + "\n";
		cout << s;
		return;
	}

	int count = 0;
	char c;
	SGD_Bool value;
	if (mode2D) {		
		for (int row = 0; row < gridRows; row++) {
			for (int col = 0; col < gridCols; col++) {
				file >> c;
				if (c == '0') value = SGD_FALSE; else value = SGD_TRUE;
				grid[col][row] = value;
				count++;
			}
		}		
	}
	else {
		for (int layer = 0; layer < gridLayers3; layer++) {
			for (int row = 0; row < gridRows3; row++) {
				for (int col = 0; col < gridCols3; col++) {
					file >> c;
					if (c == '0') value = SGD_FALSE; else value = SGD_TRUE;
					grid3[col][row][layer] = value;
					count++;
				}
			}
		}
	}
	file.close();
	cout << "Stored " << count << " values. " << endl;
	if (mode2D) arrangeGrid(); else arrangeGrid3D();
}
