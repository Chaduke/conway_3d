#pragma once

void createBackgroundModel() {
	// create a background / floor for the simulation	
	float bx, by;
	if (mode2D) {
		bx = floor((gridCols / 2) + (gridCols / 15));
		by = floor((gridRows / 2) + (gridRows / 15));
	}
	else {
		bx = floor((gridCols3 / 2) + (gridCols3 / 15));
		by = floor((gridLayers3 / 2) + (gridLayers3 / 15));
	}
	SGD_Mesh backgroundMesh = sgd_CreateBoxMesh(-bx, -3, -by, bx, -2, by, backgroundMaterial);
	sgd_TFormMeshTexCoords(backgroundMesh, 4, 4, 0, 0);
	backgroundModel = sgd_CreateModel(backgroundMesh);	
	if (!mode2D) sgd_SetEntityPosition(backgroundModel, 0, -gridRows3 / 2, 0);
}

void loadAssets() {
	skyTexture = sgd_LoadTexture("assets/skybox_night.png", 4, SGD_TEXTURE_FLAGS_CUBE);	
	cursorMaterial = sgd_LoadPBRMaterial("assets/DiamondPlate008C_1K-JPG");
	cellMaterial = sgd_LoadPBRMaterial("assets/Snow010A_1K-JPG");
	backgroundMaterial = sgd_LoadPBRMaterial("assets/Planks037B_1K-JPG");	
}

void init() {
	sgd_ClearScene();
	loadAssets();
	// skybox
	sgd_SetEnvTexture(skyTexture);
	SGD_Skybox skybox = sgd_CreateSkybox(skyTexture);	
	sgd_SetSkyboxRoughness(skybox, .25f);

	cam = sgd_CreatePerspectiveCamera();
	sgd_SetCameraFar(cam, 1000);
	pivot = sgd_CreateModel(0);
	sgd_SetEntityParent(cam, pivot);
	
	// create cursor for edit mode	
	SGD_Mesh cursorMesh = sgd_CreateSphereMesh(0.5, 16, 16, cursorMaterial);
	cursorModel = sgd_CreateModel(cursorMesh);	
	SGD_Light cursorLight = sgd_CreatePointLight();
	sgd_SetEntityParent(cursorLight, cursorModel);
	sgd_MoveEntity(cursorLight, -2, 10, -2);
	sgd_SetLightRange(cursorLight, 50);	
	sgd_SetEntityParent(cursorModel, pivot);
	sgd_MoveEntity(cursorModel, 0, -4.5, -5);

	// environment light
	sgd_SetAmbientLightColor(0.52, 0.1, 0.49, 0.05);
	SGD_Light dirLight = sgd_CreateDirectionalLight();
	sgd_SetLightShadowsEnabled(dirLight, true);
	sgd_TurnEntity(dirLight, -50, -20, 0);

	// create the cell mesh	
	cellMesh = sgd_CreateSphereMesh(0.5, 16, 16, cellMaterial);
	sgd_SetMeshShadowsEnabled(cellMesh, SGD_TRUE);
	
	createBackgroundModel();
	if (mode2D) createGridModels2D(cellMesh); else createGridModels3D(cellMesh);
	readViewState(0);
}
