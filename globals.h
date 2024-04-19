#pragma once
#include "sgd/sgd.h"

// various runtime flags 
SGD_Bool editmode = SGD_FALSE;
SGD_Bool mouselook = SGD_FALSE; // enable / disable mouselook
SGD_Bool mousesmooth = SGD_TRUE; // enable mouse smoothing	  
SGD_Bool paused = SGD_TRUE;
SGD_Bool showhelp = SGD_FALSE;
SGD_Bool loop = SGD_TRUE; // flag to stay in main loop or exit
SGD_Bool mode2D = SGD_FALSE; // decide early on 2D or full 3D mode
SGD_Bool assetsloaded = SGD_FALSE;

// created assets
SGD_Model backgroundModel;
SGD_Mesh cellMesh;
SGD_Model cursorModel;

// loaded assets 
SGD_Texture skyTexture;
SGD_Material backgroundMaterial;
SGD_Material cursorMaterial;
SGD_Material cellMaterial;
SGD_Model f1_for_help;
SGD_Model help_menu;

// for 2D mode
constexpr int gridCols = 128; // x value
constexpr int gridRows = 72; // y value
constexpr float cellSize = 1;

SGD_Bool grid[gridCols][gridRows]; // store the main displayed grid
SGD_Bool nextgrid[gridCols][gridRows]; // store the grid for the following frame
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

float movespeed = 0.3; // camera movement speed
float turnspeed = 1.0; // camera turn speed

// mouse movement 
float mx = 0; // for current mouseX 
float my = 0; // for current mouseY 	
float lmx = mx; // for last mouseX
float lmy = my; // for last mouseY
float mmx[5]; // array to store mouse movement X axis values over 5 frames
float mmy[5]; // array to store mouse movement Y axis values over 5 frames

float _mmx = 0; // for final mouse movement X value (to update pivot rotation Y-Axis)
float _mmy = 0; // for final mouse movement Y value (to update camera rotation X-Axis)	
