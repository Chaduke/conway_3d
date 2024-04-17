#include <iostream>
#include "sgd/sgd.h"
using namespace std;

constexpr int gridCols = 128; // x value
constexpr int gridRows = 72; // y value
constexpr float cellSize = 1;

SGD_Bool grid[gridCols][gridRows]; // store the main displayed grid
SGD_Bool nextgrid[gridCols][gridRows]; // store the grid for the following frame
SGD_Model cells[gridCols][gridRows]; // store the cell models 
SGD_Camera cam; 

// a blank model to use as part of the camera rig
// helps keep the camera aligned and behaving properly
// we parent the camera to this
SGD_Model pivot; 

static void reportCamera() {
	cout << "Camera Location | " << sgd_EntityX(pivot) << " " << sgd_EntityY(pivot) << " " << sgd_EntityZ(pivot) << endl;
	cout << "Camera Rotation | " << sgd_EntityRX(cam) << " " << sgd_EntityRY(cam) << " " << sgd_EntityRZ(cam) << endl;
}

static void resetCamera() {
	sgd_SetEntityPosition(pivot,0, 7, -20);
	sgd_SetEntityRotation(pivot, 0, 0, 0);
	sgd_SetEntityRotation(cam, 0, 0, 0);
}

static void resetCamera1() {
	sgd_SetEntityPosition(pivot, 0, gridCols * .82, -gridRows / 20);
	sgd_SetEntityRotation(pivot, 0, 0, 0);
	sgd_SetEntityRotation(cam, -88, 0, 0);
}

static void resetCamera2() {
	sgd_SetEntityPosition(pivot, 0, 50, -gridRows);
	sgd_SetEntityRotation(pivot, 0, 0, 0);
	sgd_SetEntityRotation(cam, -30, 0, 0);
}

static void resetCamera3() {
	sgd_SetEntityPosition(pivot, -gridCols -5, 50, -gridRows-5);
	sgd_SetEntityRotation(pivot, 0, -50, 0);
	sgd_SetEntityRotation(cam, -20, -50, 0);
}
static void resetCamera4() {
	sgd_SetEntityPosition(pivot,0, 3, -25);
	sgd_SetEntityRotation(pivot, 0, 0, 0);
	sgd_SetEntityRotation(cam, -15, 0, 0);
}


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
	// sgd_CreateWindow(1280, 720, "Conway's Game of Life in 3D", 0); // 16 : 9 Window
	// sgd_CreateWindow(3840,2160, "", 1); // UHD Fullscreen
	sgd_CreateWindow(1920, 1080, "", 1); // HD Fullscreen
	sgd_SetWebGPUBackend("D3D11");
	sgd_CreateScene();
	SGD_Texture skyTexture = sgd_LoadTexture("sgd://envmaps/nightsky-cube.png", 2, SGD_TEXTURE_FLAGS_CUBE);
	sgd_SetSceneEnvTexture(skyTexture);
	SGD_Skybox skybox = sgd_CreateSkybox();
	sgd_SetSkyboxTexture(skybox, skyTexture);
	sgd_SetSkyboxRoughness(skybox, .25f);
	cam = sgd_CreatePerspectiveCamera();
	sgd_SetCameraFar(cam, 1000);
	// create a label F1 for Help
	SGD_Model f1_for_help = sgd_LoadModel("assets/f1_for_help.glb");
	SGD_Model help_menu = sgd_LoadModel("assets/help_menu.glb");
	sgd_SetEntityParent(f1_for_help, cam);
	sgd_SetEntityParent(help_menu, cam);
	sgd_MoveEntity(f1_for_help, -0.68, 0.4, 1);
	sgd_MoveEntity(help_menu, -0.4, 0.2, 0);
	pivot = sgd_CreateModel();
	SGD_Material cursorMaterial = sgd_LoadPBRMaterial("assets/DiamondPlate008C_1K-JPG");
	SGD_Mesh cursorMesh = sgd_CreateSphereMesh(0.5, 16, 16, cursorMaterial);
	SGD_Model cursorModel = sgd_CreateModel();
	sgd_SetModelMesh(cursorModel, cursorMesh);
	SGD_Light cursorLight = sgd_CreatePointLight();
	sgd_SetEntityParent(cursorLight, cursorModel);
	sgd_MoveEntity(cursorLight, -2, 10, -2);
	sgd_SetLightRange(cursorLight, 50);
	sgd_SetEntityParent(cam, pivot);
	sgd_SetEntityParent(cursorModel, pivot);
	sgd_MoveEntity(pivot, 0, 7, -20);
	sgd_MoveEntity(cursorModel, 0, -4.5, -5);
	SGD_Light p1 = sgd_CreatePointLight();
	sgd_SetLightRange(p1, 200);
	sgd_MoveEntity(p1, 0, 50, 0);
	sgd_SetLightCastsShadow(p1, SGD_TRUE);
	SGD_Light camLight = sgd_CreatePointLight();
	sgd_SetLightRange(camLight,1.5);
	sgd_SetEntityParent(camLight, cam);
	sgd_MoveEntity(camLight, -0.5, 0, 0);
	
	// create a background for shadows	
	SGD_Material backgroundMaterial = sgd_LoadPBRMaterial("assets/Planks037B_1K-JPG");
	SGD_Model backgroundModel = sgd_CreateModel();
	float bx = floor((gridCols / 2) + (gridCols / 15));
	float by = floor((gridRows / 2) + (gridRows / 15));
	SGD_Mesh backgroundMesh = sgd_CreateBoxMesh(-bx, -3, -by, bx, -2, by, backgroundMaterial);
	sgd_TransformMeshTexCoords(backgroundMesh, 16, 16, 0, 0);
	sgd_SetModelMesh(backgroundModel, backgroundMesh);
	
	// create the cell mesh
	SGD_Material sphereMaterial = sgd_LoadPBRMaterial("assets/Snow010A_1K-JPG");
	SGD_Mesh sphereMesh = sgd_CreateSphereMesh(0.5, 16, 16, sphereMaterial);
	sgd_SetMeshCastsShadow(sphereMesh, SGD_TRUE);	
	
	// create grid models (cells)
	for (int row = 0; row < gridRows; row++) {
		for (int col = 0; col < gridCols; col++) {
			cells[col][row] = sgd_CreateModel();	
			sgd_SetModelMesh(cells[col][row], sphereMesh);
		}
	}

	randomizeGrid();
	arrangeGrid();	
	
	float movespeed = 0.3; // movement speed
	float turnspeed = 1.0; // camera turn speed
	
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
	SGD_Bool mouselook = SGD_FALSE; // enable / disable mouselook
	SGD_Bool mousesmooth = SGD_TRUE; // enable mouse smoothing	  
	SGD_Bool paused = SGD_TRUE;
	SGD_Bool showhelp = SGD_FALSE;
	SGD_Bool loop = SGD_TRUE; // flag to stay in main loop or exit

	resetCamera4(); // switch to top down view 
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
		// reset the camera
		if (sgd_KeyHit(SGD_KEY_0)) resetCamera();
		// reset the camera high up 
		if (sgd_KeyHit(SGD_KEY_1)) resetCamera1();
		// reset the camera midway and back
		if (sgd_KeyHit(SGD_KEY_2)) resetCamera2();
		// reset the camera low and back corner
		if (sgd_KeyHit(SGD_KEY_3)) resetCamera3();
		// reset the camera low 
		if (sgd_KeyHit(SGD_KEY_4)) resetCamera4();

		// report camera location and rotation to the console
		if (sgd_KeyHit(SGD_KEY_R)) reportCamera();

		// pause the simulation
		if (sgd_KeyHit(SGD_KEY_P)) {
			if (paused) paused = SGD_FALSE; else paused = SGD_TRUE;
		}
		// move forward just one step when paused
		if (sgd_KeyHit(SGD_KEY_N)) {
			if(paused) updateNext();
		}
		// toggle show help 
		// toggle edit mode
		if (sgd_KeyHit(SGD_KEY_F1)) {
			if (showhelp) {
				showhelp = SGD_FALSE;
				sgd_MoveEntity(f1_for_help, 0, 0, 1);
				sgd_MoveEntity(help_menu, 0, 0, -1);
			}
			else {
				showhelp = SGD_TRUE;
				sgd_MoveEntity(f1_for_help, 0, 0, -1);
				sgd_MoveEntity(help_menu, 0, 0, 1);
			}
		}
		// toggle edit mode
		if (sgd_KeyHit(SGD_KEY_F2)) {
			if (editmode) {
				editmode = SGD_FALSE;
				sgd_MoveEntity(cursorModel, 0, 0, -15);
			}
			else {
				editmode = SGD_TRUE;
				mouselook = SGD_FALSE;
				sgd_SetEntityPosition(pivot, floor(sgd_EntityX(pivot)), 5, floor(sgd_EntityZ(pivot)));
				sgd_SetEntityRotation(pivot, 0, 0, 0);
				sgd_SetEntityRotation(cam, -25,0, 0);
				sgd_MoveEntity(cursorModel, 0, 0, 15);
			}
		}
		if (!editmode) {
			static float m;
			// control camera with keyboard
			if (sgd_KeyDown(SGD_KEY_LEFT_SHIFT)) m = movespeed * 2; else m = movespeed; 
			if (sgd_KeyDown(SGD_KEY_W)) sgd_MoveEntity(pivot, 0, 0, m);  // move forward
			if (sgd_KeyDown(SGD_KEY_S)) sgd_MoveEntity(pivot, 0, 0, -m); // move forward
			if (sgd_KeyDown(SGD_KEY_A)) sgd_MoveEntity(pivot, -m, 0, 0); // move left
			if (sgd_KeyDown(SGD_KEY_D)) sgd_MoveEntity(pivot, m, 0, 0);  // move right
			if (sgd_KeyDown(SGD_KEY_Q)) sgd_MoveEntity(pivot, 0, m, 0); // move up
			if (sgd_KeyDown(SGD_KEY_E)) sgd_MoveEntity(pivot, 0, -m, 0);  // move down
			if (sgd_KeyDown(SGD_KEY_LEFT)) sgd_TurnEntity(pivot, 0, turnspeed, 0); // turn left
			if (sgd_KeyDown(SGD_KEY_RIGHT)) sgd_TurnEntity(pivot, 0, -turnspeed, 0); // turn right
			if (sgd_KeyDown(SGD_KEY_UP)) sgd_TurnEntity(cam, turnspeed, 0, 0); // turn upwards
			if (sgd_KeyDown(SGD_KEY_DOWN)) sgd_TurnEntity(cam, -turnspeed, 0, 0); // turn down
		}
		else {
			// in edit we move in precise units
			if (sgd_KeyHit(SGD_KEY_W)) sgd_MoveEntity(pivot, 0, 0, 1);  // move forward
			if (sgd_KeyHit(SGD_KEY_S)) sgd_MoveEntity(pivot, 0, 0, -1); // move forward
			if (sgd_KeyHit(SGD_KEY_A)) sgd_MoveEntity(pivot, -1, 0, 0); // move left
			if (sgd_KeyHit(SGD_KEY_D)) sgd_MoveEntity(pivot, 1, 0, 0);  // move right

			// toggle the cell under the cursor on / off
			if (sgd_KeyHit(SGD_KEY_ENTER)) {
				int x = int(gridCols / 2) + floor(sgd_EntityX(cursorModel));
				int y = int(gridRows / 2) + floor(sgd_EntityZ(cursorModel));
				cout << "Cursor at | " << x << " " << y << endl;
				if (x>-1 && x<gridCols && y>-1 && y<gridRows) {
					if (grid[x][y]) grid[x][y]=SGD_FALSE; else grid[x][y]=SGD_TRUE;
					arrangeGrid();
				}				
			}
		}
		
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
				mmx[4] = (lmx - mx) * 1.0; mmy[4] = (lmy - my) *1.0;
				_mmx += mmx[4]; _mmy += mmy[4];
				_mmx /= 5; _mmy /= 5;
			}
			else _mmx = lmx - mx; _mmy = lmy - my; // non smoothed mouse 

			sgd_TurnEntity(pivot, 0, _mmx, 0); // turn the pivot on the y-axis 
			sgd_TurnEntity(cam, _mmy, 0, 0); // turn the camera on the x axis 

			float crx = sgd_EntityRX(cam);  // get camera rotation on x axis (pitch)
			if (crx > 90) crx = 90;  // limit looking up
			if (crx < -90) crx = -90;  // limit looking down					
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
		sgd_RenderScene();
		sgd_Present();
	}
	sgd_Shutdown();
	return 0;
}