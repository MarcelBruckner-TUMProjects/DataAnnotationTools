//
// Created by brucknem on 13.01.21.
//
#include "Watersheder.hpp"

int main(int argc, char const *argv[]) {
	auto watersheder = providentia::calibration::Watersheder(
			"../misc/s40_n_cam_far_calibration_test_image.png"
	);
	watersheder.run();

	return 0;
}
