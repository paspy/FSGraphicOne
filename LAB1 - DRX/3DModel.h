#pragma once
#include "Chen_2DGraphicTools.h"
#include <vector>

// grid

const Vertex4 Grid[44] = {
	{ 0.5f	, 0.f	, -0.50f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },	// 0
	{ 0.5f	, 0.f	, -0.40f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.5f	, 0.f	, -0.30f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.5f	, 0.f	, -0.20f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.5f	, 0.f	, -0.10f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.5f	, 0.f	,  0.00f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.5f	, 0.f	,  0.10f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.5f	, 0.f	,  0.20f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.5f	, 0.f	,  0.30f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.5f	, 0.f	,  0.40f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.5f	, 0.f	,  0.50f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },

	{ -0.5f	, 0.f	, -0.50f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },	// 11
	{ -0.5f	, 0.f	, -0.40f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.5f	, 0.f	, -0.30f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.5f	, 0.f	, -0.20f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.5f	, 0.f	, -0.10f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.5f	, 0.f	,  0.00f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.5f	, 0.f	,  0.10f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.5f	, 0.f	,  0.20f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.5f	, 0.f	,  0.30f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.5f	, 0.f	,  0.40f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.5f	, 0.f	,  0.50f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },

	{ -0.50f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF }, // 22
	{ -0.40f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.30f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.20f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.10f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.00f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.10f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.20f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.30f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.40f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.50f	, 0.f	, 0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },

	{ -0.50f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF }, // 33
	{ -0.40f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.30f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.20f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ -0.10f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.00f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.10f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.20f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.30f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.40f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
	{ 0.50f	, 0.f	, -0.5f	, 1.f	,0.0f,	 0.0f, 0xFFFFFFFF },
};

// Cube
const Vertex4 Cube[8] = {
	{ -0.25f, -0.25f,  0.25f, 1.f,	0.0f, 0.0f, 0xFF00FF00 },	// A
	{ 0.25f, -0.25f,  0.25f, 1.f,	0.0f, 0.0f, 0xFF00FF00 },	// B
	{ 0.25f, -0.25f, -0.25f, 1.f,	0.0f, 0.0f, 0xFF00FF00 },	// C
	{ -0.25f, -0.25f, -0.25f, 1.f,	0.0f, 0.0f, 0xFF00FF00 },	// D

	{ -0.25f,  0.25f,  0.25f, 1.f,	0.0f, 0.0f, 0xFF00FF00 },	// E
	{ 0.25f,   0.25f,  0.25f, 1.f,	0.0f, 0.0f, 0xFF00FF00 },	// F
	{ 0.25f,   0.25f, -0.25f, 1.f,	0.0f, 0.0f, 0xFF00FF00 },	// G
	{ -0.25f,  0.25f, -0.25f, 1.f,	0.0f, 0.0f, 0xFF00FF00 },	// H
};

//Triangle

const Vertex4 Triangle[3] = {
	{ 0,		0.5f,	0,	1.0f,	0.0f,	0.0f,	0xFFFF0000 },
	{ 0.5f,		0,		0,	1.0f,	0.0f,	0.0f,	0xFF00FF00 },
	{ -0.5f,	0,		0,	1.0f,	0.0f,	0.0f,	0xFF0000FF },
};


// Cube - ByTriangle
const Tri Cube2[12] = {
	//	x		y		z		w		u		v	color		   x		y		z		w		u		v	color	  x		y		z		w		u		v	color
	{ { -0.25f, -0.25f,  0.25f, 1.f,	0.0f, 1.0f, 0xFFFF00FF },{ 0.25f, -0.25f,  0.25f, 1.f,	1.0f, 1.0f, 0xFFFF00FF },{ 0.25f, -0.25f, -0.25f, 1.f,	1.0f, 0.0f, 0xFFFF00FF }, },	// ABC
	{ { 0.25f, -0.25f, -0.25f, 1.f,		1.0f, 0.0f, 0xFFFF00FF },{ -0.25f, -0.25f, -0.25f, 1.f,	0.0f, 0.0f, 0xFFFF00FF },{ -0.25f, -0.25f,  0.25f, 1.f,	0.0f, 1.0f, 0xFFFF00FF }, },	// CDA

	{ { -0.25f, -0.25f,  0.25f, 1.f,	0.0f, 1.0f, 0xFFFF0000 },{ 0.25f, -0.25f,  0.25f, 1.f,	1.0f, 1.0f, 0xFFFF0000 },{ 0.25f,   0.25f,  0.25f, 1.f,	1.0f, 0.0f, 0xFFFF0000 }, },	// ABF
	{ { 0.25f,   0.25f,  0.25f, 1.f,	1.0f, 0.0f, 0xFFFF0000 },{ -0.25f,  0.25f,  0.25f, 1.f,	0.0f, 0.0f, 0xFFFF0000 },{ -0.25f, -0.25f,  0.25f, 1.f,	0.0f, 1.0f, 0xFFFF0000 }, },	// FEA

	{ { 0.25f, -0.25f,  0.25f, 1.f,		0.0f, 1.0f, 0xFF00FF00 },{ 0.25f, -0.25f, -0.25f, 1.f,	1.0f, 1.0f, 0xFF00FF00 },{ 0.25f,   0.25f, -0.25f, 1.f,	1.0f, 0.0f, 0xFF00FF00 }, },	// BCG
	{ { 0.25f,   0.25f, -0.25f, 1.f,	1.0f, 0.0f, 0xFF00FF00 },{ 0.25f,   0.25f,  0.25f, 1.f,	0.0f, 0.0f, 0xFF00FF00 },{ 0.25f, -0.25f,  0.25f, 1.f,	0.0f, 1.0f, 0xFF00FF00 }, },	// GFB

	{ { -0.25f, -0.25f, -0.25f, 1.f,	0.0f, 1.0f, 0xFF0000FF },{ 0.25f, -0.25f, -0.25f, 1.f,	1.0f, 1.0f, 0xFF0000FF },{ 0.25f,   0.25f, -0.25f, 1.f,	1.0f, 0.0f, 0xFF0000FF }, },	// DCG
	{ { 0.25f,   0.25f, -0.25f, 1.f,	1.0f, 0.0f, 0xFF0000FF },{ -0.25f,  0.25f, -0.25f, 1.f,	0.0f, 0.0f, 0xFF0000FF },{ -0.25f, -0.25f, -0.25f, 1.f,	0.0f, 1.0f, 0xFF0000FF }, },	// GHD

	{ { -0.25f, -0.25f,  0.25f, 1.f,	0.0f, 1.0f, 0xFFFFFF00 },{ -0.25f, -0.25f, -0.25f, 1.f,	1.0f, 1.0f, 0xFFFFFF00 },{ -0.25f,  0.25f, -0.25f, 1.f,	1.0f, 0.0f, 0xFFFFFF00 }, },	// ADH
	{ { -0.25f,  0.25f, -0.25f, 1.f,	1.0f, 0.0f, 0xFFFFFF00 },{ -0.25f,  0.25f,  0.25f, 1.f,	0.0f, 0.0f, 0xFFFFFF00 },{ -0.25f, -0.25f,  0.25f, 1.f,	0.0f, 1.0f, 0xFFFFFF00 }, },	// HEA

	{ { -0.25f,  0.25f,  0.25f, 1.f,	0.0f, 1.0f, 0xFF00FFFF },{ 0.25f,   0.25f,  0.25f, 1.f,	1.0f, 1.0f, 0xFF00FFFF },{ 0.25f,   0.25f, -0.25f, 1.f,	1.0f, 0.0f, 0xFF00FFFF }, },	// EFG
	{ { 0.25f,   0.25f, -0.25f, 1.f,	1.0f, 0.0f, 0xFF00FFFF },{ -0.25f,  0.25f, -0.25f, 1.f,	0.0f, 0.0f, 0xFF00FFFF },{ -0.25f,  0.25f,  0.25f, 1.f,	0.0f, 1.0f, 0xFF00FFFF }, },	// GHE
};
