#pragma once

#include "ofCustomMaterial.h"

class MaterialCache {
public:
	static ofCustomMaterial* load(string name, bool force = false);

private:
	struct MaterialInfo {
		ofCustomMaterial material;
		uint64_t loadedFrame;

		MaterialInfo(ofCustomMaterial material_, uint64_t loadedFrame_)
			: material(material_)
			, loadedFrame(loadedFrame_) {
		}
	};

	static std::unordered_map<string, MaterialInfo> mMaterials;

};
