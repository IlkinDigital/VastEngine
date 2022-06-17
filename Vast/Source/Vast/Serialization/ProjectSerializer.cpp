#include "vastpch.h"
#include "ProjectSerializer.h"

#include <yaml-cpp/yaml.h>

namespace Vast {

	String ProjectSerializer::s_ProjectFile = "vast.project";

	void ProjectSerializer::Serialize(const Filepath& filepath)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;

		out << YAML::Key << "ProjectName" << YAML::Value << m_Project->GetName();

		out << YAML::EndMap;

		std::ofstream fs(filepath / s_ProjectFile);
		fs << out.c_str();
		fs.close();
	}

	bool ProjectSerializer::Deserialize(const Filepath& filepath)
	{
		std::ifstream fs(filepath / s_ProjectFile);
		StringStream ss;
		ss << fs.rdbuf();

		YAML::Node data = YAML::Load(ss.str());

		if (!data["ProjectName"])
			return false;

		String projName = data["ProjectName"].as<String>();
		m_Project->Set(projName, std::filesystem::canonical(filepath));
		return true;
	}

}