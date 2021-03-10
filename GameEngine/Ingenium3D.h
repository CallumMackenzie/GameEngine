#include "Modwin.h"
#include "Vector.h"

namespace ingenium_3D {
	struct Vector3F {
		float x = 0;
		float y = 0;
		float z = 0;
	};
	
	struct Matrix4x4F {
		float matrix[4][4] = 
		{
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
			{0, 0, 0, 0}
		};
	};
	
};
