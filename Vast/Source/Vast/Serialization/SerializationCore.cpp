#include "vastpch.h"
#include "SerializationCore.h"

namespace Vast {

	YAML::Emitter& Vast::operator<<(YAML::Emitter& out, const Vector3& data)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << data.x << data.y << data.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const Vector4& data)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << data.x << data.y << data.z << data.w << YAML::EndSeq;
		return out;
	}

}