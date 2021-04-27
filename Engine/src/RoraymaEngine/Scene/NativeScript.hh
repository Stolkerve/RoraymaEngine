#pragma once

#include "Entity.hh"

namespace rym
{
	class NativeScript
	{
	public:
		virtual ~NativeScript()
		{}

		template<typename T>
		inline T* GetComponent()
		{
			return m_Entity->GetComponent<T>();
		}

	protected:
		virtual void OnStart() {}
		virtual void OnUpdate(float _delta) {}
		virtual void OnQuit() {}

	private:
		Entity* m_Entity;
		friend class Scene;
	};
}
