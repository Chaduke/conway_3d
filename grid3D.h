#pragma 

SGD_Bool grid3[gridCols3][gridRows3][gridLayers3]; // store the main displayed grid
SGD_Bool nextgrid3[gridCols3][gridRows3][gridLayers3]; // store the grid for the following frame
SGD_Model cells3[gridCols3][gridRows3][gridLayers3]; // store the cell models #pragma once

static void randomizeGrid3D() {
	for (int layer = 0; layer < gridLayers3; layer++) {
		for (int row = 0; row < gridRows3; row++) {
			for (int col = 0; col < gridCols3; col++) {
				SGD_Bool r = rand() % 2;
				grid3[col][row][layer] = r;
			}
		}
	}
}

static void arrangeGrid3D() {
	float ypos;
	for (int layer = 0; layer < gridLayers3; layer++) {
		for (int row = 0; row < gridRows3; row++) {
			for (int col = 0; col < gridCols3; col++) {
				if (grid3[col][row][layer]) ypos = float(row - gridRows3 / 2) * cellSize; else ypos = -200;
				sgd_SetEntityPosition(cells3[col][row][layer], float(col - gridCols3 / 2) * cellSize, ypos, float(layer - gridLayers3 / 2) * cellSize);
			}
		}
	}
}

void createGridModels3D(SGD_Mesh mesh) {
	// create grid models (cells) 3D
	for (int layer = 0; layer < gridLayers3; layer++) {
		for (int row = 0; row < gridRows3; row++) {
			for (int col = 0; col < gridCols3; col++) {
				cells3[col][row][layer] = sgd_CreateModel(mesh);				
			}
		}
	}
	randomizeGrid3D();
	arrangeGrid3D();	
}

static void clearGrid3D() {
	for (int layer = 0; layer < gridLayers3; layer++) {
		for (int row = 0; row < gridRows3; row++) {
			for (int col = 0; col < gridCols3; col++) {
				grid3[col][row][layer] = 0;
				nextgrid3[col][row][layer] = 0;
			}
		}
	}
	arrangeGrid3D();
}

static void updateNextGrid3D() {
	for (int layer = 0; layer < gridLayers3; layer++) {
		for (int row = 0; row < gridRows3; row++) {
			for (int col = 0; col < gridCols3; col++) {
				int n = 0; // number of neighbors
				// count neighbors first
				// front upper left
				if (col && row && layer) {
					if (grid3[col - 1][row - 1][layer - 1]) n++;
				}
				// front upper
				if (row && layer) {
					if (grid3[col][row - 1][layer - 1]) n++;
				}
				// front upper right
				if (col < gridCols3 - 1 && row && layer) {
					if (grid3[col + 1][row - 1][layer - 1]) n++;
				}
				// front left
				if (col && layer) {
					if (grid3[col - 1][row][layer - 1]) n++;
				}
				// front middle
				if (layer) {
					if (grid3[col][row][layer - 1]) n++;
				}
				// front right
				if (col < gridCols3 - 1 && layer) {
					if (grid3[col + 1][row][layer - 1]) n++;
				}
				// front lower left
				if (col && row < gridRows3 - 1 && layer) {
					if (grid3[col - 1][row + 1][layer - 1]) n++;
				}
				// front lower
				if (row < gridRows3 - 1 && layer) {
					if (grid3[col][row + 1][layer - 1]) n++;
				}
				// front lower right
				if (col < gridCols3 - 1 && row < gridRows3 - 1 && layer) {
					if (grid3[col + 1][row + 1][layer - 1]) n++;
				}
				// middle upper left
				if (col && row) {
					if (grid3[col - 1][row - 1][layer]) n++;
				}
				// middle upper
				if (row) {
					if (grid3[col][row - 1][layer]) n++;
				}
				// middle upper right
				if (col < gridCols3 - 1 && row) {
					if (grid3[col + 1][row - 1][layer]) n++;
				}
				// middle left
				if (col) {
					if (grid3[col - 1][row][layer]) n++;
				}
				// middle right
				if (col < gridCols3 - 1) {
					if (grid3[col + 1][row][layer]) n++;
				}
				// middle lower left
				if (col && row < gridRows3 - 1) {
					if (grid3[col - 1][row + 1][layer]) n++;
				}
				// middle lower
				if (row < gridRows3 - 1) {
					if (grid3[col][row + 1][layer]) n++;
				}
				// middle lower right
				if (col < gridCols3 - 1 && row < gridRows3 - 1) {
					if (grid3[col + 1][row + 1][layer]) n++;
				}
				// back upper left
				if (col && row && layer < gridLayers3 - 1) {
					if (grid3[col - 1][row - 1][layer + 1]) n++;
				}
				// back upper
				if (row && layer < gridLayers3 - 1) {
					if (grid3[col][row - 1][layer + 1]) n++;
				}
				// back upper right
				if (col < gridCols3 - 1 && row && layer < gridLayers3 - 1) {
					if (grid3[col + 1][row - 1][layer + 1]) n++;
				}
				// back left
				if (col && layer < gridLayers3 - 1) {
					if (grid3[col - 1][row][layer + 1]) n++;
				}
				// back middle
				if (layer < gridLayers3 - 1) {
					if (grid3[col][row][layer + 1]) n++;
				}
				// back right
				if (col < gridCols3 - 1 && layer < gridLayers3 - 1) {
					if (grid3[col + 1][row][layer + 1]) n++;
				}
				// back lower left
				if (col && row < gridRows - 1 && layer < gridLayers3 - 1) {
					if (grid3[col - 1][row + 1][layer + 1]) n++;
				}
				// back lower
				if (row < gridRows3 - 1 && layer < gridLayers3 - 1) {
					if (grid3[col][row + 1][layer + 1]) n++;
				}
				// back lower right
				if (col < gridCols3 - 1 && row < gridRows3 - 1 && layer < gridLayers3 - 1) {
					if (grid3[col + 1][row + 1][layer + 1]) n++;
				}

				// update nextgrid
				// rule 1 underpopulation death
				if (grid3[col][row][layer] && n < 6) nextgrid3[col][row][layer] = SGD_FALSE;
				// rule 2 survival 
				if (grid3[col][row][layer] && n > 5 && n < 12) nextgrid3[col][row][layer] = SGD_TRUE;
				// rule 3 overpopulation death 
				if (grid3[col][row][layer] && n > 11) nextgrid3[col][row][layer] = SGD_FALSE;
				// rule 4 rebirth
				if (!grid3[col][row][layer] && n > 8 && n < 12) nextgrid3[col][row][layer] = SGD_TRUE;
			}
		}
	}
	// update grid with nextgrid
	for (int layer = 0; layer < gridLayers3; layer++) {
		for (int row = 0; row < gridRows3; row++) {
			for (int col = 0; col < gridCols3; col++) {
				grid3[col][row][layer] = nextgrid3[col][row][layer];
			}
		}
	}
	arrangeGrid3D();
}