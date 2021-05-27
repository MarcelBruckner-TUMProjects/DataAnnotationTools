//
// Created by brucknem on 09.03.21.
//

#include "DataAnnotationTools/watershed/Formatter.hpp"
#include "yaml-cpp/yaml.h"


namespace data_annotation_tools {
    namespace watersheder {

        int getBiggestComponent(const cv::Mat &watershedRegions, int componentCount) {
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

        std::string toYAML(const cv::Mat &watershedRegions, int componentCount, bool keepBiggestComponent) {
            int biggestComponent = getBiggestComponent(watershedRegions, componentCount);

            std::map<int, std::vector<std::vector<int>>> componentsToPixels;
            for (int i = 0; i < watershedRegions.rows; i++) {
                for (int j = 0; j < watershedRegions.cols; j++) {
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
            yaml << YAML::BeginMap;

            yaml << YAML::Key << "image_size" << YAML::Value;
            yaml << YAML::Comment("rows, columns");
            yaml << YAML::Flow << YAML::BeginSeq << watershedRegions.rows << watershedRegions.cols << YAML::EndSeq;

            yaml << YAML::Key << "component_count" << YAML::Value << componentsToPixels.size();

            yaml << YAML::Key << "regions" << YAML::Value;
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
            yaml << YAML::EndMap;

            return yaml.c_str();
        }
    }
}