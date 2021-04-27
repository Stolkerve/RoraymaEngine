#pragma once

#include <memory>
namespace rym
{

	template<typename T>
	class Node
	{
	public:
		Node() = default;

		void DeleteChild(class std::vector<std::shared_ptr<Node<T>>>::const_iterator it)
		{
			m_Childs.erase(it);
		}

		void AddChild(const std::shared_ptr<Node<T>>& child)
		{
			m_Childs.push_back(child);
		}

		void Reserve(size_t size) { m_Childs.reserve(size); }
		bool Empty() const { return m_Childs.empty(); }
		size_t Size() const { return m_Childs.size(); }
		const std::shared_ptr<Node<T>>& Back() const { return m_Childs.back(); }

		std::shared_ptr<T>& operator[](size_t index)
		{
			return m_Childs[index];
		}

		class std::vector<std::shared_ptr<Node<T>>>::iterator begin() { return m_Childs.begin(); }
		class std::vector<std::shared_ptr<Node<T>>>::iterator end() { return m_Childs.end(); }
		class std::vector<std::shared_ptr<Node<T>>>::const_iterator begin() const { return m_Childs.begin(); }
		class std::vector<std::shared_ptr<Node<T>>>::const_iterator end() const { return m_Childs.end(); }

		T Data;
		std::shared_ptr<Node<T> > Parent;
	private:
		std::vector<std::shared_ptr<Node<T>>> m_Childs;
	};

};