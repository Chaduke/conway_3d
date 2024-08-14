// Conway's Game of Life in 3D
// by Chad Dore'
// chaduke@gmail.com | youtube.com/chaddore | github.com/chaduke

// made with LibSGD - support it and get it at patreon.com/libsgd

// materials from ambientcg.com
// if you have questions, comments, requests, or do anything cool // 
// with this let me know, comment on youtube or the blitzworld forums, email me etc..
// Blitz World forums - https://skirmish-dev.net/forum/
// thanks for checking this out

// TODO : add ImGUI
// TODO : improve edit mode
// TODO : improve lighting / shadows
// TODO : improve mouselook

#include <fstream>
#include <iostream>
#include <string>
#include "globals.h"
// #include "rules.h"
#include "grid2D.h"
#include "grid3D.h"
#include "states.h"
#include "init.h"
#include "input.h"
#include "imgui.h"
#define IMGUI_IMPL_SGD_IMPLEMENTATION 1
#include "sgd/imgui_impl_sgd.h"


int main(int, char**) {	
	sgd_Init();
	// sgd_CreateWindow(1280, 720, "Conway's Game of Life in 3D", 0); // 16 : 9 Window
	// sgd_CreateWindow(3840,2160, "", 1); // UHD Fullscreen
	sgd_CreateWindow(1920, 1080, "", 256); // HD Fullscreen	
	// imgui init
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();
	ImGui_ImplSGD_Init();

	init();		
	// start main loop
	while (loop) {
		processInput();	

		if (!paused) {
			if (mode2D) updateNextGrid(); else updateNextGrid3D();
		}
		sgd_RenderScene();
		// imgui 
		ImGui_ImplSGD_NewFrame();
		ImGui::NewFrame();		
		if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);	
		
		if (show_help) {
			ImGui::Begin("Welcome to Conway's Game of Life in 3D!\n");			
			ImGui::BulletText("To minimize this window you can double click\nthe title bar\n");
			ImGui::BulletText("When it's not in focus you can use the F1 key\nto toggle its visiblity.\n");
			if (ImGui::CollapsingHeader("Options")) {
				ImGui::Text("These can be toggled with the keyboard shortcuts\nonly when the window is not in focus");
				ImGui::Checkbox("Mouselook (Keyboard F)", &mouselook);
				ImGui::Checkbox("Pause (Keyboard SPACE)", &paused);
				ImGui::Checkbox("Edit Mode (Keyboard G)", &editmode);
				ImGui::Checkbox("2D Mode (Keyboard T)", &mode2D);
				if (ImGui::Button("Clear Grid", ImVec2(100, 20))) {
					if (mode2D) clearGrid(); else clearGrid3D();
				}
				ImGui::Text("(Keyboard C)");
				if (ImGui::Button("Reset / Randomize Grid", ImVec2(200, 20))) {
					if (mode2D) {
						randomizeGrid();
						arrangeGrid();
					}
					else {
						randomizeGrid3D();
						arrangeGrid3D();
					}
				}
				ImGui::Text("(Keyboard R)");

				if (ImGui::Button("Next Grid Step", ImVec2(150, 20))) {
					if (paused) if (mode2D) updateNextGrid(); else updateNextGrid3D();
				}
				ImGui::Text("(Keyboard N)");				
			}				
			if (ImGui::CollapsingHeader("Cellular Automata Rules")) {
				ImGui::Text("Ruleset 1- Conway's standard");
				ImGui::SeparatorText("Rule 1 - Loneliness");
				ImGui::Text("IF Currently Alive");
				ImGui::Text("AND Neighbors less than 2");
				ImGui::Text("THEN After Dead");
				ImGui::SeparatorText("Rule 2 - Survival");
				ImGui::Text("IF Currently Alive");
				ImGui::Text("AND Neighbors greater than 2");
				ImGui::Text("AND Neighbors less than 4");
				ImGui::Text("THEN After Alive");
				ImGui::SeparatorText("Rule 3 - Rebirth");
				ImGui::Text("IF Currently Dead");
				ImGui::Text("AND Neighbors equal to 3");
				ImGui::Text("THEN After Alive");
				ImGui::SeparatorText("Rule 4 - Overpopulation");
				ImGui::Text("IF Currently Alive");
				ImGui::Text("AND Neighbors greater than 3");
				ImGui::Text("THEN After Dead");
			}
			if (ImGui::CollapsingHeader("Info and Stats")) {
				ImGui::Text("Pivot Position X %.2f Y %.2f Z %.2f", sgd_GetEntityX(pivot), sgd_GetEntityY(pivot), sgd_GetEntityZ(pivot));
				ImGui::Text("Pivot Rotation X %.2f Y %.2f Z %.2f", sgd_GetEntityRX(pivot), sgd_GetEntityRY(pivot), sgd_GetEntityRZ(pivot));
				ImGui::Text("Camera Position X %.2f Y %.2f Z %.2f", sgd_GetEntityX(cam), sgd_GetEntityY(cam), sgd_GetEntityZ(cam));
				ImGui::Text("Camera Rotation X %.2f Y %.2f Z %.2f", sgd_GetEntityRX(cam), sgd_GetEntityRY(cam), sgd_GetEntityRZ(cam));
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			
			ImGui::End();
		}			
		ImGui::Render();
		ImGui_ImplSGD_RenderDrawData(ImGui::GetDrawData());
		sgd_Present();
	}
	// Cleanup
	ImGui_ImplSGD_Shutdown();
	ImGui::DestroyContext();
	sgd_Terminate();
	return 0;
}