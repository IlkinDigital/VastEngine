#pragma once

#include "CoreTypes.h"

namespace Vast {

	class UUID
	{
	public:
		UUID();
		UUID(uint64 id)
			: m_UUID(id) {}
		UUID(const UUID&) = default;

		operator uint64() const { return m_UUID; }
		bool operator == (const UUID& other) { return m_UUID == other.m_UUID; }
	private:
		uint64 m_UUID;
	};

}
