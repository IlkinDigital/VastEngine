#pragma once

/**
* Serialization Core contains necessary helper functions and classes
* to assist in type conversions, etc.
*/

#include "Math/Math.h"

#pragma warning(push, 0)
#include <yaml-cpp/yaml.h>
#pragma warning(pop)

namespace YAML {

	// yaml-cpp's to Vector3 conversion
	template<>
	struct convert<Vast::Vector3>
	{
		static Node encode(const Vast::Vector3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, Vast::Vector3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	// yaml-cpp's to Vector4 conversion
	template<>
	struct convert<Vast::Vector4>
	{
		static Node encode(const Vast::Vector4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, Vast::Vector4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};

}

namespace Vast {

	YAML::Emitter& operator << (YAML::Emitter& out, const Vector3& data)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << data.x << data.y << data.z << YAML::EndSeq;
		return out;
	}
	
	YAML::Emitter& operator << (YAML::Emitter& out, const Vector4& data)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << data.x << data.y << data.z << data.w << YAML::EndSeq;
		return out;
	}

}