#pragma once

#include "pluginapi/Plugin.hpp"
#include <map>
#include <mutex>

//! @brief Manages all applet plugins.
//!
class PluginFactory
{
public:
	PluginFactory() = default;
	~PluginFactory() = default;

	//! @brief	Attempt to register a plugin based on its registration details.
	//!
	//! @return If the operation was successful.
	//!
	bool registerPlugin(const pl::Package& package);

	std::unique_ptr<pl::EditorWindow> create(const std::string& extension, u32 magic);

private:
	std::mutex mMutex;		//!< When performing write operations (registering a plugin)
	std::vector<pl::FileEditor> mPlugins;	//!< Other data here references by index -- be careful to maintain that.
	std::vector<std::pair<std::string, std::size_t>> mExtensions; //!< Maps extension string to mPlugins index.
	std::map<u32, std::size_t> mMagics;		//!< Maps file magic identifiers to mPlugins index.
};
