//
// Created by brucknem on 09.03.21.
//

#ifndef DATAANNOTATIONTOOLS_FORMATTER_HPP
#define DATAANNOTATIONTOOLS_FORMATTER_HPP

#include <string>
#include <map>
#include <vector>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include "Watersheder.hpp"

namespace dataannotationtools {

	int GetBiggestComponent(const cv::Mat &watershedRegions, int componentCount) {
		std::map<int, int> componentsToPixelAmount;
		for (int i = 0; i < watershedRegions.rows; i++) {
			for (int j = 0; j < watershedRegions.cols; j++) {
				int index = watershedRegions.at<int>(i, j);
				if (index <= 0 || index > componentCount) {
					continue;
				}
				componentsToPixelAmount[index]++;
			}
		}

		int index;
		int maxPixels = -1;
		for (const auto &entry : componentsToPixelAmount) {
			if (entry.second > maxPixels) {
				maxPixels = entry.second;
				index = entry.first;
			}
		}
		return index;
	}

	/**
	 *
	 * @param watershedMarkers
	 * @param componentCount
	 * @return
	 */
	std::string WatershederToYAML(const cv::Mat &watershedRegions, int componentCount, bool keepBiggestComponent =
	false) {
		int biggestComponent = GetBiggestComponent(watershedRegions, componentCount);

		std::map<int, std::vector<std::vector<int>>> componentsToPixels;
		for (int j = 0; j < watershedRegions.cols; j++) {
			for (int i = 0; i < watershedRegions.rows; i++) {
				int index = watershedRegions.at<int>(i, j);
				if (index <= 0 || index > componentCount) {
					continue;
				}
				if (!keepBiggestComponent && (index == biggestComponent)) {
					continue;
				}
				componentsToPixels[index].emplace_back(std::vector<int>{j, i});
			}
		}

		YAML::Emitter yaml;
		yaml << YAML::BeginSeq;
		for (const auto &entry : componentsToPixels) {
			yaml << YAML::BeginMap;
			yaml << YAML::Key << "id" << YAML::Value << entry.first;
			yaml << YAML::Key << "size" << YAML::Value << entry.second.size();
			yaml << YAML::Key << "pixels" << YAML::Value;
			yaml << YAML::BeginSeq;

			for (const auto &pixel : entry.second) {
				yaml << YAML::Flow << pixel;
			}

			yaml << YAML::EndSeq;
			yaml << YAML::EndMap;
		}
		yaml << YAML::EndSeq;
		return yaml.c_str();
	}
}

#endif //DATAANNOTATIONTOOLS_FORMATTER_HPP
