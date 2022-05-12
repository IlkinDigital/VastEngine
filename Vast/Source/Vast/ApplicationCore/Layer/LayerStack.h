#pragma once

#include "Layer.h"

namespace Vast {

	class LayerStack
	{
	public:
		LayerStack() = default;
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		DArray<Layer*>::iterator begin() { return m_Layers.begin(); }
		DArray<Layer*>::iterator end() { return m_Layers.end(); }

		DArray<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		DArray<Layer*>::const_iterator end() const { return m_Layers.end(); }
	private:
		DArray<Layer*> m_Layers;
		uint16 m_InsertionIndex = 0;
	};

}