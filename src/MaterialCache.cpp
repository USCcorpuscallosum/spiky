#include "MaterialCache.h"
#include <ofMain.h>

std::unordered_map<string, MaterialCache::MaterialInfo> MaterialCache::mMaterials;

ofCustomMaterial* MaterialCache::load(string name, bool force /*= false*/) {
	auto it = mMaterials.find(name);

	// If already loaded
	if (it != mMaterials.end()) {
		// Reload if forced and not loaded this frame
		if (force && it->second.loadedFrame < ofGetFrameNum()) {
			it->second = MaterialInfo(ofCustomMaterial(), ofGetFrameNum());
			it->second.material.load(name);
		}

		return &it->second.material;
	}

	// Emplace and load
	auto pair = mMaterials.emplace(name, MaterialInfo(ofCustomMaterial(), ofGetFrameNum()));
	auto material = &pair.first->second.material;
	material->load(name);
	return material;
}
