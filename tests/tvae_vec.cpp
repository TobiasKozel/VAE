#include "../src/core/algo/vae_vec.hpp"


bool close(float a, float b, float epsylon = 0.01) {
	if (tklb::abs(a - b) < epsylon) {
		return true;
	} else {
		return false; // Easy to set a breakpoint
	}
}

using namespace vae;
using namespace vae::core::vector;

int main() {
	Vector3 test = {-1, -2, -3};
	auto test2 = Vec4::from(test);
	if (!close(test2.t,   1)) { return 1; }
	if (!close(test2.x,  -1)) { return 2; }
	if (!close(test2.y,  -2)) { return 3; }
	if (!close(test2.z,  -3)) { return 4; }
	if (!close(test2[0], -1)) { return 5; }
	if (!close(test2[1], -2)) { return 6; }
	if (!close(test2[2], -3)) { return 7; }
	if (!close(test2[3],  1)) { return 8; }

	auto identity = Mat4x4::identity();
	Vector3 result = multiply(identity, test);
	if (!close(result.x, -1)) { return 9; }
	if (!close(result.y, -2)) { return 10; }
	if (!close(result.z, -3)) { return 11; }

	vae::ListenerProperties reference;
	Vector3 test3 = {3, 4, 5};
	Mat4x4 camera = lookAt({ 0, 0, 0}, reference.front, reference.up);
	Vector3 result2 = multiply(camera, test3);
	(void) result2;
	if (!close(result.x, -1)) { return 12; }
	if (!close(result.y, -2)) { return 13; }
	if (!close(result.z, -3)) { return 14; }
	return 0;
}
