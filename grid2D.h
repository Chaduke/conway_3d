#pragma once
static void randomizeGrid() {
	for (int row = 0; row < gridRows; row++) {
		for (int col = 0; col < gridCols; col++) {
			SGD_Bool r = rand() % 2;
			grid[col][row] = r;
		}
	}
}

// move the models to the correct places
static void arrangeGrid() {
	float ypos;
	for (int row = 0; row < gridRows; row++) {
		for (int col = 0; col < gridCols; col++) {
			if (grid[col][row]) ypos = 0; else ypos = -200;
			sgd_SetEntityPosition(cells[col][row], float(col - gridCols / 2) * cellSize, ypos, float(row - gridRows / 2) * cellSize);
		}
	}
}

void createGridModels2D(SGD_Mesh mesh) {		
	for (int row = 0; row < gridRows; row++) {
		for (int col = 0; col < gridCols; col++) {
			cells[col][row] = sgd_CreateModel();
			sgd_SetModelMesh(cells[col][row], mesh);
		}
	}
	randomizeGrid();
	arrangeGrid();
}

static void clearGrid() {
	for (int row = 0; row < gridRows; row++) {
		for (int col = 0; col < gridCols; col++) {
			grid[col][row] = 0;
			nextgrid[col][row] = 0;
		}
	}
	arrangeGrid();
}

static void updateNextGrid() {
	for (int row = 0; row < gridRows; row++) {
		for (int col = 0; col < gridCols; col++) {
			int n = 0; // number of neighbors
			// count neighbors first
			// upper left
			if (col && row) {
				if (grid[col - 1][row - 1]) n++;
			}
			// upper
			if (row) {
				if (grid[col][row - 1]) n++;
			}
			// upper right
			if (col < gridCols - 1 && row) {
				if (grid[col + 1][row - 1]) n++;
			}
			// left
			if (col) {
				if (grid[col - 1][row]) n++;
			}
			// right
			if (col < gridCols - 1) {
				if (grid[col + 1][row]) n++;
			}
			// lower left
			if (col && row < gridRows - 1) {
				if (grid[col - 1][row + 1]) n++;
			}
			// lower
			if (row < gridRows - 1) {
				if (grid[col][row + 1]) n++;
			}
			// lower right
			if (col < gridCols - 1 && row < gridRows - 1) {
				if (grid[col + 1][row + 1]) n++;
			}
			// rule 1 underpopulation death
			if (grid[col][row] && n < 2) nextgrid[col][row] = SGD_FALSE;
			// rule 2 survival 
			if (grid[col][row] && n > 1 && n < 4) nextgrid[col][row] = SGD_TRUE;
			// rule 3 overpopulation death 
			if (grid[col][row] && n > 3) nextgrid[col][row] = SGD_FALSE;
			// rule 4 rebirth
			if (!grid[col][row] && n == 3) nextgrid[col][row] = SGD_TRUE;
		}
	}
	// update grid with nextgrid
	for (int row = 0; row < gridRows; row++) {
		for (int col = 0; col < gridCols; col++) {
			grid[col][row] = nextgrid[col][row];
		}
	}
	arrangeGrid();
}
