#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/Event.h"

namespace Hazel {
	class HAZEL_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		// when a layer is pushed into the layerstack, attach it
		virtual void OnAttach() {}
		// when a layer is removed from the layerstack, detach it
		virtual void OnDetach() {}
		// update every frame
		virtual void OnUpdate(Timestep ts) {}
		// received event here when it send to the layer
		virtual void OnEvent(Event& event) {}

		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;

	};
}


