#pragma once

static void initCamera() {
	cam = sgd_CreatePerspectiveCamera();
	sgd_SetCameraFar(cam, 1000);
	pivot = sgd_CreateModel();
	sgd_SetEntityParent(cam, pivot);
	sgd_MoveEntity(pivot, 0, 7, -20);
}

static void reportCamera() {
	std::cout << "Camera Location | " << sgd_EntityX(pivot) << " " << sgd_EntityY(pivot) << " " << sgd_EntityZ(pivot) << std::endl;
	std::cout << "Camera Rotation | " << sgd_EntityRX(cam) << " " << sgd_EntityRY(cam) << " " << sgd_EntityRZ(cam) << std::endl;
}

static void resetCamera() {
	sgd_SetEntityPosition(pivot, 0, 7, -20);
	sgd_SetEntityRotation(pivot, 0, 0, 0);
	sgd_SetEntityRotation(cam, 0, 0, 0);
}

static void resetCamera1(SGD_Bool mode2D) {
	if (mode2D) {
		sgd_SetEntityPosition(pivot, 0, gridCols * 0.82, -gridRows / 20);
	}
	else {
		sgd_SetEntityPosition(pivot, 0, gridRows3 * 2, -gridRows3 / 20);
	}
	sgd_SetEntityRotation(pivot, 0, 0, 0);
	sgd_SetEntityRotation(cam, -88, 0, 0);
}

static void resetCamera2() {
	sgd_SetEntityPosition(pivot, 0, 50, -gridRows);
	sgd_SetEntityRotation(pivot, 0, 0, 0);
	sgd_SetEntityRotation(cam, -30, 0, 0);
}

static void resetCamera3() {
	sgd_SetEntityPosition(pivot, float(-gridCols - 5), 50, float(- gridRows - 5));
	sgd_SetEntityRotation(pivot, 0, -50, 0);
	sgd_SetEntityRotation(cam, -20, -50, 0);
}
static void resetCamera4() {
	sgd_SetEntityPosition(pivot, -47, 15, -40);
	sgd_SetEntityRotation(pivot, 0, -57, 0);
	sgd_SetEntityRotation(cam, -15, -57, 0);
}