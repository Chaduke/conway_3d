#include <iostream>
#include "sgd/sgd.h"
using namespace std;

constexpr int gridCols = 200; // x value
constexpr int gridRows = 200; // y value
constexpr float cellSize = 1;

SGD_Bool grid[gridCols][gridRows]; // store the main displayed grid
SGD_Bool nextgrid[gridCols][gridRows]; // store the grid for the following frame
SGD_Model cells[gridCols][gridRows]; // store the cell models 

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
			if (grid[col][row]) ypos = 0; else ypos = -300;
			sgd_SetEntityPosition(cells[col][row], float(col - gridCols / 2) * cellSize, ypos, float(row - gridRows / 2) * cellSize);
		}
	}
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

static void updateNext() {	
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
			if (col < gridCols-1 && row) {
				if (grid[col+1][row - 1]) n++;
			}
			// left
			if (col) {
				if (grid[col - 1][row]) n++;
			}
			// right
			if (col < gridCols-1) {
				if (grid[col + 1][row]) n++;
			}
			// lower left
			if (col && row<gridRows-1) {
				if (grid[col-1][row+1]) n++;
			}
			// lower
			if (row < gridRows - 1) {
				if (grid[col][row + 1]) n++;
			}
			// lower right
			if (col<gridCols-1 && row < gridRows - 1) {
				if (grid[col+1][row + 1]) n++;
			}
			// update nextgrid
			// rule 1 underpopulation death
			if (grid[col][row] && n < 2) nextgrid[col][row] = SGD_FALSE;
			// rule 2 survival 
			if (grid[col][row] && n>1 && n<4) nextgrid[col][row] = SGD_TRUE;
			// rule 3 overpopulation death 
			if (grid[col][row] && n > 3) nextgrid[col][row] = SGD_FALSE;
			// rule 4 rebirth
			if (!grid[col][row] && n==3) nextgrid[col][row] = SGD_TRUE;
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

int main() {
	sgd_Startup();
	// sgd_CreateWindow(1366, 768, "Conway's Game of Life in 3D", 0);
	sgd_CreateWindow(3840,2160, "", 1);
	sgd_SetWebGPUBackend("D3D11");
	sgd_CreateScene();
	SGD_Texture skyTexture = sgd_LoadTexture("sgd://envmaps/nightsky-cube.png", 2, SGD_TEXTURE_FLAGS_CUBE);
	sgd_SetSceneEnvTexture(skyTexture);
	SGD_Skybox skybox = sgd_CreateSkybox();
	sgd_SetSkyboxTexture(skybox, skyTexture);
	sgd_SetSkyboxRoughness(skybox, .25f);

	SGD_Camera cam = sgd_CreatePerspectiveCamera();
	sgd_SetCameraFar(cam, 1000);
	SGD_Model pivot = sgd_CreateModel();
	sgd_SetEntityParent(cam, pivot);	
	SGD_Light p1 = sgd_CreatePointLight();
	sgd_SetLightRange(p1, 200);
	sgd_MoveEntity(p1, 0, 50, 0);
	sgd_SetLightCastsShadow(p1, SGD_TRUE);
	// SGD_Light sunlight = sgd_CreateDirectionalLight();	
	// sgd_TurnEntity(sunlight, -10, 0, 0);

	// load a background for shadows	
	SGD_Model backgroundModel = sgd_LoadModel("assets/background.glb");	
	// SGD_Model sphereModel = sgd_LoadModel("assets/sphere.glb");
	SGD_Material sphereMaterial = sgd_LoadPBRMaterial("sgd://materials/Fabric048_1K-JPG");
	SGD_Mesh sphereMesh = sgd_CreateSphereMesh(0.5, 16, 16, sphereMaterial);
	sgd_SetMeshCastsShadow(sphereMesh, SGD_TRUE);	
	// create a 3D cursor
	SGD_Model cursorModel = sgd_CreateModel();
	SGD_Material cursorMaterial = sgd_CreatePBRMaterial(1.0, 0, 0, 1.0);
	SGD_Mesh cursorMesh = sgd_CreateSphereMesh(0.5, 16, 16, cursorMaterial);
	sgd_SetModelMesh(cursorModel, cursorMesh);
	sgd_SetEntityParent(cursorModel, cam);
	// create a raycasting cylinder
	SGD_Model raycastModel = sgd_LoadModel("assets/raycast_cylinder.glb");

	sgd_SetEntityParent(raycastModel, cam);	
	sgd_MoveEntity(pivot, 0, 100, -110);
	sgd_MoveEntity(cursorModel, 0, 0, 20);
	sgd_MoveEntity(raycastModel, 0, 0, 5);

	//float b = boxSize / 2; // box half size
	//SGD_Material boxMaterial = sgd_LoadPrelitMaterial("assets/box_Albedo.png");
	//SGD_Mesh boxMesh = sgd_CreateBoxMesh(-b, -b, -b, b, b, b, boxMaterial);
	//sgd_SetMeshCastsShadow(boxMesh, SGD_TRUE);
	//SGD_Model boxModel = sgd_CreateModel();
	//sgd_SetModelMesh(boxModel, boxMesh);
	
	// create grid models (cells)
	for (int row = 0; row < gridRows; row++) {
		for (int col = 0; col < gridCols; col++) {
			cells[col][row] = sgd_CreateModel();	
			sgd_SetModelMesh(cells[col][row], sphereMesh);
		}
	}

	randomizeGrid();
	arrangeGrid();	
	
	float m = 0.6; // movement speed
	float r = 1.0; // camera turn speed
	
	float mx = sgd_MouseX(); // for current mouseX 
	float my = sgd_MouseY(); // for current mouseY 	
	float lmx = mx; // for last mouseX
	float lmy = my; // for last mouseY
	float mmx[5]; // array to store mouse movement X axis values over 5 frames
	float mmy[5]; // array to store mouse movement Y axis values over 5 frames

	// initialize mouse movement arrays
	for (int i = 0; i < 5; i++) {
		mmx[i] = 0; mmy[i] = 0;
	}
	float _mmx = 0; // for final mouse movement X value (to update pivot rotation Y-Axis)
	float _mmy = 0; // for final mouse movement Y value (to update camera rotation X-Axis)

	SGD_Bool editmode = SGD_FALSE;
	SGD_Bool mouselook = SGD_TRUE; // enable mouselook
	SGD_Bool mousesmooth = SGD_TRUE; // enable mouse smoothing	  
	SGD_Bool paused = SGD_TRUE;
	SGD_Bool loop = SGD_TRUE; // flag to stay in main loop or exit

	// start main loop
	while (loop) {

		int e = sgd_PollEvents();
		if (sgd_KeyDown(SGD_KEY_ESCAPE) || (e == 1)) loop = SGD_FALSE;  // exit program 

		// reset the grid
		if (sgd_KeyHit(SGD_KEY_SPACE)) {
			randomizeGrid();
			arrangeGrid();
		}
		// clear the grid 
		if (sgd_KeyHit(SGD_KEY_C)) clearGrid();	

		// pause the simulation
		if (sgd_KeyHit(SGD_KEY_P)) {
			if (paused) paused = SGD_FALSE; else paused = SGD_TRUE;
		}
		// move forward just one step when paused
		if (sgd_KeyHit(SGD_KEY_N)) {
			if(paused) updateNext();
		}

		// toggle edit mode
		if (sgd_KeyHit(SGD_KEY_F2)) {
			if (editmode) editmode = SGD_FALSE; else {
				editmode = SGD_TRUE;
				sgd_SetEntityPosition(cam, sgd_EntityX(cam), 20, sgd_EntityZ(cam));
			}
		}
		
		// control camera with keyboard
		if (sgd_KeyDown(SGD_KEY_W)) sgd_MoveEntity(pivot, 0, 0, m);  // move forward
		if (sgd_KeyDown(SGD_KEY_S)) sgd_MoveEntity(pivot, 0, 0, -m); // move forward
		if (sgd_KeyDown(SGD_KEY_A)) sgd_MoveEntity(pivot, -m, 0, 0); // move left
		if (sgd_KeyDown(SGD_KEY_D)) sgd_MoveEntity(pivot, m, 0, 0);  // move right
		if (sgd_KeyDown(SGD_KEY_Q)) sgd_MoveEntity(pivot, 0, m, 0); // move up
		if (sgd_KeyDown(SGD_KEY_E)) sgd_MoveEntity(pivot, 0, -m, 0);  // move down
		if (sgd_KeyDown(SGD_KEY_LEFT)) sgd_TurnEntity(pivot, 0, r, 0); // turn left
		if (sgd_KeyDown(SGD_KEY_RIGHT)) sgd_TurnEntity(pivot, 0, -r, 0); // turn right
		if (sgd_KeyDown(SGD_KEY_UP)) sgd_TurnEntity(cam, r, 0, 0); // turn upwards
		if (sgd_KeyDown(SGD_KEY_DOWN)) sgd_TurnEntity(cam, -r, 0, 0); // turn down
		
		// toggle mouselook to switch to gamepad		
		if (sgd_KeyHit(SGD_KEY_M)) {
			if (mouselook) mouselook = SGD_FALSE; else mouselook = SGD_TRUE;
		}
		if (mouselook) {
			lmx = mx; lmy = my; // update last mouse position
			mx = sgd_MouseX(); my = sgd_MouseY(); // get current mouse position

			// turn off mousesmooth if you prefer choppier mouse movement
			if (mousesmooth) {
				_mmx = 0; _mmy = 0;
				for (int i = 0; i < 4; i++) {
					mmx[i] = mmx[i + 1]; mmy[i] = mmy[i + 1];
					_mmx += mmx[i];
					_mmy += mmy[i];
				}
				mmx[4] = (lmx - mx) * 0.5; mmy[4] = (lmy - my) * 0.5;
				_mmx += mmx[4]; _mmy += mmy[4];
				_mmx /= 5; _mmy /= 5;
			}
			else _mmx = lmx - mx; _mmy = lmy - my; // non smoothed mouse 

			sgd_TurnEntity(pivot, 0, _mmx, 0); // turn the pivot on the y-axis 
			sgd_TurnEntity(cam, _mmy, 0, 0); // turn the camera on the x axis 

			float crx = sgd_EntityRX(cam);  // get camera rotation on x axis (pitch)
			if (crx > 75) crx = 75;  // limit looking up
			if (crx < -75) crx = -75;  // limit looking down					
			float cry = sgd_EntityRY(pivot); // get pivot rotation on y axis (yaw)		
			sgd_SetEntityRotation(cam, crx, cry, 0);  // reset the camera without z rotation (roll)
		}
		else {
			// get gamepad input (tested with PS4 controller)
			sgd_MoveEntity(pivot, sgd_GamepadAxis(0, 0) * 0.2, 0, -sgd_GamepadAxis(0, 1) * 0.2);
			sgd_TurnEntity(pivot, -sgd_GamepadAxis(0, 3) * 1.5, -sgd_GamepadAxis(0, 2) * 1.5, 0);
			sgd_SetEntityRotation(pivot, sgd_EntityRX(pivot), sgd_EntityRY(pivot), 0);
		}
		if (!paused) updateNext();
		/*if (editmode) {
			sgd_SetEntityPosition(cursorModel, sgd_EntityX(pivot), sgd_EntityY(pivot), sgd_EntityZ(pivot));
			sgd_SetEntityRotation(cursorModel, sgd_EntityRX(cam), sgd_EntityRY(pivot), 0);
			sgd_MoveEntity(cursorModel, 0, 0, 20);
			sgd_SetEntityPosition(cursorModel, sgd_EntityX(cursorModel), 0, sgd_EntityZ(cursorModel));
		}
		else {
			sgd_SetEntityPosition(cursorModel, sgd_EntityX(pivot), sgd_EntityY(pivot), sgd_EntityZ(pivot));
			sgd_MoveEntity(cursorModel, 0, 10,0);
		}*/
		sgd_RenderScene();
		sgd_Present();
	}
	sgd_Shutdown();
	return 0;
}