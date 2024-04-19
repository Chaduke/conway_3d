#pragma once



void createBackgroundModel() {
	// create a background / floor for the simulation
	
	backgroundModel = sgd_CreateModel();
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
	sgd_TransformMeshTexCoords(backgroundMesh, 16, 16, 0, 0);
	sgd_SetModelMesh(backgroundModel, backgroundMesh);
	if (!mode2D) sgd_SetEntityPosition(backgroundModel, 0, -gridRows3 / 2, 0);
}

void loadAssets() {
	skyTexture = sgd_LoadTexture("assets/skybox_night.png", 2, SGD_TEXTURE_FLAGS_CUBE);
	f1_for_help = sgd_LoadModel("assets/f1_for_help.glb");
	help_menu = sgd_LoadModel("assets/help_menu.glb");
	cursorMaterial = sgd_LoadPBRMaterial("assets/DiamondPlate008C_1K-JPG");
	cellMaterial = sgd_LoadPBRMaterial("assets/Snow010A_1K-JPG");
	backgroundMaterial = sgd_LoadPBRMaterial("assets/Planks037B_1K-JPG");
}

void init() {
	sgd_ClearScene();
	if(!assetsloaded) loadAssets();
	// skybox
	sgd_SetSceneEnvTexture(skyTexture);
	SGD_Skybox skybox = sgd_CreateSkybox();
	sgd_SetSkyboxTexture(skybox, skyTexture);
	sgd_SetSkyboxRoughness(skybox, .25f);

	initCamera();	
	// create help menu	
	sgd_SetEntityParent(f1_for_help, cam);
	sgd_SetEntityParent(help_menu, cam);
	sgd_MoveEntity(f1_for_help, -0.335, 0.2, 0.5);
	sgd_MoveEntity(help_menu, -0.48, 0.15, 0);
	
	// create cursor for edit mode	
	SGD_Mesh cursorMesh = sgd_CreateSphereMesh(0.5, 16, 16, cursorMaterial);
	cursorModel = sgd_CreateModel();
	sgd_SetModelMesh(cursorModel, cursorMesh);
	SGD_Light cursorLight = sgd_CreatePointLight();
	sgd_SetEntityParent(cursorLight, cursorModel);
	sgd_MoveEntity(cursorLight, -2, 10, -2);
	sgd_SetLightRange(cursorLight, 50);	
	sgd_SetEntityParent(cursorModel, pivot);
	sgd_MoveEntity(cursorModel, 0, -4.5, -5);

	// environment light
	sgd_SetSceneAmbientLightColor(0.52, 0.1, 0.49, 0.05);
	SGD_Light dirLight = sgd_CreateDirectionalLight();

	// middle light
	SGD_Light midLight = sgd_CreatePointLight();
	sgd_SetLightRange(midLight, 20);
	sgd_MoveEntity(midLight, 0, 5, 0);
	sgd_SetLightCastsShadow(midLight, SGD_TRUE);

	// camera light
	SGD_Light camLight = sgd_CreatePointLight();
	sgd_SetLightRange(camLight, 1.5);
	sgd_SetEntityParent(camLight, cam);
	sgd_MoveEntity(camLight, -0.5, 0, 0);

	// create the cell mesh	
	cellMesh = sgd_CreateSphereMesh(0.5, 16, 16, cellMaterial);
	sgd_SetMeshCastsShadow(cellMesh, SGD_TRUE);
	
	createBackgroundModel();
	if (mode2D) createGridModels2D(cellMesh); else createGridModels3D(cellMesh);
	
	resetCamera4(); // switch to close view
}