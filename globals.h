#pragma once
#define SGD_DYNAMIC 1
#include "sgd/sgd.h"

// various runtime flags 
bool editmode = false;
bool mouselook = false; // enable / disable mouselook 
bool paused = true;
bool show_demo_window = false; // imgui demo window
bool show_help = true; // imgui help window
bool show_stats = true; // imgui stats window
bool show_rules = true; // imgui rules window
bool loop = true; // flag to stay in main loop or exit
bool mode2D = false; // decide early on 2D or full 3D mode

// created assets
SGD_Model backgroundModel;
SGD_Mesh cellMesh;
SGD_Model cursorModel;

// loaded assets 
SGD_Texture skyTexture;
SGD_Material backgroundMaterial;
SGD_Material cursorMaterial;
SGD_Material cellMaterial;

// for 2D mode
constexpr int gridCols = 128; // x value
constexpr int gridRows = 72; // y value
constexpr float cellSize = 1;

bool grid[gridCols][gridRows]; // store the main displayed grid
bool nextgrid[gridCols][gridRows]; // store the grid for the following frame
SGD_Model cells[gridCols][gridRows]; // store the cell models 

// for 3D mode
constexpr int gridCols3 = 35; // x value
constexpr int gridRows3 = 35; // y value
constexpr int gridLayers3 = 35; // z value
constexpr float cellSize3 = 1;

// the main camera
SGD_Camera cam;
// a blank model to use as part of the camera rig
// helps keep the camera aligned and behaving properly
// we parent the camera to this
SGD_Model pivot;

float movespeed = 0.3f; // camera movement speed
float turnspeed = 1.0f; // camera turn speed