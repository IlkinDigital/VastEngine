#include "vastpch.h"
#include "LayerStack.h"

namespace Vast {

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.push_back(layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		for (auto it = m_Layers.begin() + m_InsertionIndex; it != m_Layers.end(); ++it)
		{
			if (*it == layer)
			{
				m_Layers.erase(it);
				layer->OnDetach();
			}
		}
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.insert(m_Layers.begin() + m_InsertionIndex, overlay);
		m_InsertionIndex++;
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		for (auto it = m_Layers.begin(); it != m_Layers.begin() + m_InsertionIndex; ++it)
		{
			if (*it == overlay)
			{
				m_Layers.erase(it);
				overlay->OnDetach();
			}
		}

		m_InsertionIndex--;
	}

}
