#include "Vast/Scene/ScriptableEntity.h"
#include "Scene/Components.h"
#include "Input/Input.h"

namespace Vast {

	class CharacterController : public ScriptableEntity
	{
	public:
		void OnUpdate(Timestep ts) override
		{
			auto& transform = GetComponent<TransformComponent>();

			if (Input::IsPressed(Key::W))
				transform.Translation.y += m_Speed * ts;
			if (Input::IsPressed(Key::S))
				transform.Translation.y -= m_Speed * ts;
			if (Input::IsPressed(Key::D))
				transform.Translation.x += m_Speed * ts;
			if (Input::IsPressed(Key::A))
				transform.Translation.x -= m_Speed * ts;
		}
	private:
		float m_Speed = 3.0f;
	};

}