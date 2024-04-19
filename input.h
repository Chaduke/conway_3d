#pragma once
void processInput() {
	int e = sgd_PollEvents();
	if (sgd_KeyDown(SGD_KEY_ESCAPE) || (e == 1)) loop = SGD_FALSE;  // exit program 

	// reset the grid
	if (sgd_KeyHit(SGD_KEY_SPACE)) {
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
	// reset the camera to starting values
	if (sgd_KeyHit(SGD_KEY_0)) resetCamera();
	// reset the camera high up 
	if (sgd_KeyHit(SGD_KEY_1)) resetCamera1(mode2D);
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
	if (sgd_KeyHit(SGD_KEY_N) && paused) {
		if (mode2D) updateNextGrid(); else updateNextGrid3D();
	}
	// toggle show help 		
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
	if (sgd_KeyHit(SGD_KEY_F3)) {
		if (mode2D) mode2D = SGD_FALSE; else mode2D = SGD_TRUE;
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
					if (grid[x][y]) grid[x][y] = SGD_FALSE; else grid[x][y] = SGD_TRUE;
					arrangeGrid();
				}
			}
			else {
				int x = int(gridCols3 / 2) + floor(sgd_EntityX(cursorModel));
				int y = int(gridRows3 / 2) + floor(sgd_EntityY(cursorModel));
				int z = int(gridLayers3 / 2) + floor(sgd_EntityZ(cursorModel));
				if (x > -1 && x<gridCols3 && y>-1 && y < gridRows3 && z>-1 && z < gridLayers3) {
					if (grid3[x][y][z]) grid3[x][y][z] = SGD_FALSE; else grid3[x][y][z] = SGD_TRUE;
					arrangeGrid3D();
				}
			}
			// cout << "Cursor at | " << x << " " << y << endl;			
		}
	}
	// load and save states 
	if (sgd_KeyHit(SGD_KEY_KP_0)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeToState(0); else readFromState(0);
	if (sgd_KeyHit(SGD_KEY_KP_1)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeToState(1); else readFromState(1);
	if (sgd_KeyHit(SGD_KEY_KP_2)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeToState(2); else readFromState(2);
	if (sgd_KeyHit(SGD_KEY_KP_3)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeToState(3); else readFromState(3);
	if (sgd_KeyHit(SGD_KEY_KP_4)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeToState(4); else readFromState(4);
	if (sgd_KeyHit(SGD_KEY_KP_5)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeToState(5); else readFromState(5);
	if (sgd_KeyHit(SGD_KEY_KP_6)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeToState(6); else readFromState(6);
	if (sgd_KeyHit(SGD_KEY_KP_7)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeToState(7); else readFromState(7);
	if (sgd_KeyHit(SGD_KEY_KP_8)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeToState(8); else readFromState(8);
	if (sgd_KeyHit(SGD_KEY_KP_9)) if (sgd_KeyDown(SGD_KEY_LEFT_CONTROL)) writeToState(9); else readFromState(9);	

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
			mmx[4] = (lmx - mx); mmy[4] = (lmy - my);
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
}