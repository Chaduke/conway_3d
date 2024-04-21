#pragma once
void processInput() {
	int e = sgd_PollEvents();
	if (sgd_KeyDown(SGD_KEY_ESCAPE) || (e == 1)) loop = false;  // exit program 

	// reset the grid
	if (sgd_KeyHit(SGD_KEY_R)) {
		if (mode2D) {
			randomizeGrid();
			arrangeGrid();
		}
		else {
			randomizeGrid3D();
			arrangeGrid3D();
		}
	}
	// clear the grid 
	if (sgd_KeyHit(SGD_KEY_C)) {
		if (mode2D) clearGrid(); else clearGrid3D();
	}

	// pause the simulation
	if (sgd_KeyHit(SGD_KEY_SPACE)) {
		if (paused) paused = false; else paused = true;
	}
	// move forward just one step when paused
	if (sgd_KeyHit(SGD_KEY_N) && paused) {
		if (mode2D) updateNextGrid(); else updateNextGrid3D();
	}	
	// toggle help window
	if (sgd_KeyHit(SGD_KEY_F1)) if (show_help) show_help=false; else show_help = true;

	// toggle imgui demo window
	if (sgd_KeyHit(SGD_KEY_F4)) if (show_demo_window) show_demo_window = false; else show_demo_window = true;
	
	// toggle edit mode
	if (sgd_KeyHit(SGD_KEY_G)) {
		if (editmode) {
			editmode = false;
			sgd_MoveEntity(cursorModel, 0, 0, -15);			
		}
		else {
			editmode = true;
			mouselook = false;
			if (mode2D) {
				if (sgd_EntityX(pivot) < -gridCols / 2 || sgd_EntityX(pivot) > gridCols / 2 ||
					sgd_EntityZ(pivot) < -gridRows / 2 || sgd_EntityZ(pivot) > gridCols / 2)
				{
					sgd_SetEntityPosition(pivot, 0, 5, 0);
				} else sgd_SetEntityPosition(pivot, floor(sgd_EntityX(pivot)), 5, floor(sgd_EntityZ(pivot)));				
				sgd_SetEntityRotation(pivot, 0, 0, 0);
				sgd_SetEntityRotation(cam, -25, 0, 0);
				sgd_MoveEntity(cursorModel, 0, 0, 15);
			}
		}
	}
	// toggle 2D / 3D mode 
	if (sgd_KeyHit(SGD_KEY_T)) {
		if (mode2D) mode2D = false; else mode2D=true;
		init();
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
		// in edit mode we move in precise units
		if (sgd_KeyHit(SGD_KEY_W)) sgd_MoveEntity(pivot, 0, 0, 1);  // move forward
		if (sgd_KeyHit(SGD_KEY_S)) sgd_MoveEntity(pivot, 0, 0, -1); // move forward
		if (sgd_KeyHit(SGD_KEY_A)) sgd_MoveEntity(pivot, -1, 0, 0); // move left
		if (sgd_KeyHit(SGD_KEY_D)) sgd_MoveEntity(pivot, 1, 0, 0);  // move right
		if (!mode2D) {
			if (sgd_KeyHit(SGD_KEY_Q)) sgd_MoveEntity(pivot, 0, 1, 0); // move up
			if (sgd_KeyHit(SGD_KEY_E)) sgd_MoveEntity(pivot, 0, -1, 0);  // move down
		}
		// toggle the cell under the cursor on / off
		if (sgd_KeyHit(SGD_KEY_ENTER)) {
			if (mode2D) {
				int x = int(gridCols / 2) + floor(sgd_EntityX(cursorModel));
				int y = int(gridRows / 2) + floor(sgd_EntityZ(cursorModel));
				if (x > -1 && x<gridCols && y>-1 && y < gridRows) {
					if (grid[x][y]) grid[x][y] = false; else grid[x][y] = true;
					arrangeGrid();
				}
			}
			else {
				int x = int(gridCols3 / 2) + floor(sgd_EntityX(cursorModel));
				int y = int(gridRows3 / 2) + floor(sgd_EntityY(cursorModel));
				int z = int(gridLayers3 / 2) + floor(sgd_EntityZ(cursorModel));
				if (x > -1 && x<gridCols3 && y>-1 && y < gridRows3 && z>-1 && z < gridLayers3) {
					if (grid3[x][y][z]) grid3[x][y][z] = false; else grid3[x][y][z] = true;
					arrangeGrid3D();
				}
			}			
		}
	}
	// load and save grid states 
	if (sgd_KeyHit(SGD_KEY_KP_0)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeGridState(0); else readGridState(0);
	if (sgd_KeyHit(SGD_KEY_KP_1)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeGridState(1); else readGridState(1);
	if (sgd_KeyHit(SGD_KEY_KP_2)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeGridState(2); else readGridState(2);
	if (sgd_KeyHit(SGD_KEY_KP_3)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeGridState(3); else readGridState(3);
	if (sgd_KeyHit(SGD_KEY_KP_4)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeGridState(4); else readGridState(4);
	if (sgd_KeyHit(SGD_KEY_KP_5)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeGridState(5); else readGridState(5);
	if (sgd_KeyHit(SGD_KEY_KP_6)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeGridState(6); else readGridState(6);
	if (sgd_KeyHit(SGD_KEY_KP_7)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeGridState(7); else readGridState(7);
	if (sgd_KeyHit(SGD_KEY_KP_8)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeGridState(8); else readGridState(8);
	if (sgd_KeyHit(SGD_KEY_KP_9)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeGridState(9); else readGridState(9);

	// load and save view states 
	if (sgd_KeyHit(SGD_KEY_0)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeViewState(0); else readViewState(0);
	if (sgd_KeyHit(SGD_KEY_1)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeViewState(1); else readViewState(1);
	if (sgd_KeyHit(SGD_KEY_2)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeViewState(2); else readViewState(2);
	if (sgd_KeyHit(SGD_KEY_3)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeViewState(3); else readViewState(3);
	if (sgd_KeyHit(SGD_KEY_4)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeViewState(4); else readViewState(4);
	if (sgd_KeyHit(SGD_KEY_5)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeViewState(5); else readViewState(5);
	if (sgd_KeyHit(SGD_KEY_6)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeViewState(6); else readViewState(6);
	if (sgd_KeyHit(SGD_KEY_7)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeViewState(7); else readViewState(7);
	if (sgd_KeyHit(SGD_KEY_8)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeViewState(8); else readViewState(8);
	if (sgd_KeyHit(SGD_KEY_9)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeViewState(9); else readViewState(9);

	// toggle mouselook to switch to gamepad		
	if (sgd_KeyHit(SGD_KEY_F)) {
		if (mouselook) {
			mouselook = false;
			sgd_SetMouseCursorMode(1);
		}
		else {
			mouselook = true;
			sgd_SetMouseCursorMode(3);
		}
	}
	if (mouselook) {
		sgd_TurnEntity(pivot, 0, -sgd_MouseVX(), 0); // turn the pivot on the y-axis 
		sgd_TurnEntity(cam, -sgd_MouseVY(), 0, 0); // turn the camera on the x axis 

		float crx = sgd_EntityRX(cam);  // get camera rotation on x axis (pitch)
		if (crx > 90) crx = 90;  // limit looking up
		if (crx < -90) crx = -90;  // limit looking down					
		float cry = sgd_EntityRY(pivot); // get pivot rotation on y axis (yaw)		
		sgd_SetEntityRotation(cam, crx, 0, 0);  // reset the camera without z rotation (roll)
	}
	else {
		// get gamepad input (tested with PS4 controller)
		sgd_MoveEntity(pivot, sgd_GamepadAxis(0, 0) * 0.2, 0, -sgd_GamepadAxis(0, 1) * 0.2);
		sgd_TurnEntity(pivot, -sgd_GamepadAxis(0, 3) * 1.5, -sgd_GamepadAxis(0, 2) * 1.5, 0);
		sgd_SetEntityRotation(pivot, sgd_EntityRX(pivot), sgd_EntityRY(pivot), 0);
	}
}