#pragma once

#include "RendererCore.h"

namespace Vast {

	class Cubemap
	{
	public:
		virtual ~Cubemap() = default;

		virtual RendererID GetRendererID() const = 0;

		virtual void Bind() = 0;

		static Ref<Cubemap> Create(const Filepath& right, const Filepath& left, const Filepath& top, const Filepath& bottom, const Filepath& front, const Filepath& back);
	};

}
