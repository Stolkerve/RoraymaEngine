#pragma once

#include "../Core/Assert.hh"
#include "Components.hh"
#include "Nodes.hh"
#include <unordered_map>
#include <memory>

namespace rym
{
	class Entity
	{
	public:
		template<class T, typename ... Params>
		T* AddComponent(Params&&... params)
		{
			auto type = T::GetType();
			auto it = m_Components.find(type);
			if (it == m_Components.end())
			{
				m_Components[type] = reinterpret_cast<void*>(new T(std::forward<Params>(params)...));
				return static_cast<T*>(m_Components[type]);
			}
			RYM_CORE_ERROR("The entity {0} already have the {1} component ", Tag, type);
			return nullptr;
		}

		template <class T>
		bool DeleteComponent()
		{
			auto type = T::GetType();
			auto it = m_Components.find(type);
			if (it == m_Components.end())
			{
				RYM_CORE_ERROR("The entity {0} cannot delete the {1} component, this was not found ", Tag, type);
				return false;
			}
			delete it->second;
			m_Components.erase(type);
			return true;
		}

		template <class T>
		T* GetComponent() const
		{
			auto type = T::GetType();
			auto it = m_Components.find(type);
			if (it == m_Components.end())
			{
				//RYM_CORE_ASSERT(false, "The entity " + Tag + " cannot get the " + " component");
				return nullptr;
			}
			return static_cast<T*>(it->second);
		}

		// It is only works in python
		template <class T>
		bool HaveComponent()
		{
			auto type = T::GetType();
			auto it = m_Components.find(type);
			if (it == m_Components.end())
				return false;
			return true;
		}

		size_t NumOfComponents()
		{
			return m_Components.size();
		}

		~Entity()
		{
			RYM_INFO("{} deleted.", Tag);
			for (auto& [first, secount] : m_Components)
			{
				delete secount;
			}
		}

	public:
		std::string Tag;
		uint32_t ID;
		bool visible = true;

		std::unordered_map<uint32_t, std::string> MapIDtoTag;
		std::unordered_map<std::string, uint32_t> MapTagtoID;

	private:
		std::unordered_map<ComponentType, void*> m_Components;
	};

	using EntitysNode = Node<Entity>;
}
