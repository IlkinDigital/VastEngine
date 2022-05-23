#pragma once

#include "MathCore.h"

namespace Vast::Math {

	const float* ValuePointer(const Mat4& data);
	float* MValuePointer(Mat4& data);
	const float* ValuePointer(const Vector3& data);
	float* MValuePointer(Vector3& data);
	const float* ValuePointer(const Vector4& data);
	float* MValuePointer(Vector4& data);
}