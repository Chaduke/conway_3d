// Conway's Game of Life in 3D
// by Chad Dore'
// chaduke@gmail.com | youtube.com/chaddore | github.com/chaduke

// made with LibSGD - support it and get it at patreon.com/libsgd

// materials from ambientcg.com
// if you have questions, comments, requests, or do anything cool // 
// with this let me know, comment on youtube or the blitzworld forums, email me etc..

// thanks for checking this out

// TODO : add ImGUI
// TODO : improve edit mode
// TODO : improve lighting / shadows
// TODO : improve mouselook

#include <fstream>
#include <iostream>
#include <string>
#include "globals.h"
#include "grid2D.h"
#include "grid3D.h"
#include "states.h"
#include "init.h"
#include "input.h"

int main() {
	sgd_Startup();
	sgd_CreateWindow(1280, 720, "Conway's Game of Life in 3D", 0); // 16 : 9 Window
	// sgd_CreateWindow(3840,2160, "", 1); // UHD Fullscreen
	// sgd_CreateWindow(1920, 1080, "", 1); // HD Fullscreen
	sgd_SetWebGPUBackend("D3D12");
	sgd_CreateScene();
	init();
	
	// start main loop
	while (loop) {
		processInput();		

		if (!paused) {
			if (mode2D) updateNextGrid(); else updateNextGrid3D();
		}
		sgd_RenderScene();
		sgd_Present();
	}
	sgd_Shutdown();
	return 0;
}