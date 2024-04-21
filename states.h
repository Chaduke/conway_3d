#pragma once
using namespace std;

void writeViewState(int state) {
	string mode;
	if (mode2D) mode = "2D_"; else mode = "3D_";
	string filename = "assets/states/viewstate" + mode + to_string(state) + ".dat";
	ofstream file(filename);
	file << sgd_EntityX(pivot) << endl;
	file << sgd_EntityY(pivot) << endl;
	file << sgd_EntityZ(pivot) << endl;
	file << sgd_EntityRX(pivot) << endl;
	file << sgd_EntityRY(pivot) << endl;
	file << sgd_EntityRZ(pivot) << endl;
	file << sgd_EntityX(cam) << endl;
	file << sgd_EntityY(cam) << endl;
	file << sgd_EntityZ(cam) << endl;
	file << sgd_EntityRX(cam) << endl;
	file << sgd_EntityRY(cam) << endl;
	file << sgd_EntityRZ(cam) << endl;
	file.close();
	cout << "Wrote to file : " << filename << endl;
}

void readViewState(int state) {
	string mode;
	if (mode2D) mode = "2D_"; else mode = "3D_";
	string filename = "assets/states/viewstate" + mode + to_string(state) + ".dat";
	ifstream file;
	file.open(filename);
	if (file.fail()) {
		string s = "Error opening file :" + filename + "\n";
		cout << s;
		return;
	}
	float px, py, pz, prx, pry, prz;
	float cx, cy, cz, crx, cry, crz;
	string s;
	file >> s; px = stof(s);
	file >> s; py = stof(s);
	file >> s; pz = stof(s);
	file >> s; prx = stof(s);
	file >> s; pry = stof(s);
	file >> s; prz = stof(s);
	file >> s; cx = stof(s);
	file >> s; cy = stof(s);
	file >> s; cz = stof(s);
	file >> s; crx = stof(s);
	file >> s; cry = stof(s);
	file >> s; crz = stof(s);	
	sgd_SetEntityPosition(pivot, px, py, pz);
	sgd_SetEntityRotation(pivot, prx, pry, prz);
	sgd_SetEntityRotation(cam,0, 0,0);
	sgd_TurnEntity(cam, crx, 0, 0);
	// sgd_SetEntityPosition(cam, cx, cy, cz);
	// sgd_SetEntityRotation(cam, crx, cry, crz);
	cout << "Read from file : " << filename << endl;
}

void writeGridState(int state) {
	string mode;
	if (mode2D) mode = "2D_"; else mode = "3D_";
	string filename = "assets/states/gridstate" + mode + to_string(state) + ".dat";
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
	cout << "Wrote to file : " << filename << endl;
}

void readGridState(int state) {
	string mode;
	if (mode2D) mode = "2D_"; else mode = "3D_";
	string filename = "assets/states/gridstate" + mode + to_string(state) + ".dat";
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
	cout << "Read from file : " << filename << endl;
	cout << "Stored " << count << " values. " << endl;
	if (mode2D) arrangeGrid(); else arrangeGrid3D();
}
