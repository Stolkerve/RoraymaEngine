#pragma once
#include <pybind11/embed.h>
#include "Entity.hh"


namespace rym
{
	enum class PyComponentsType
	{
		TransformComponent,
		SpriteComponent,
		CameraComponent
	};

	class PyScript
	{
	public:
		~PyScript() = default;
		PyScript() = default;

		PyScript(const std::string& moduleName, const std::shared_ptr<Entity>& entity)
		{
			//Can't add new modules after the interpreter has been initialized
			m_ModuleName = moduleName;
			try {
				m_Entity = entity;
				m_PyScriptModule = pybind11::module::import(m_ModuleName.c_str());
				auto tmp = m_PyScriptModule.attr(m_ModuleName.c_str())();
				pybind11::cast<std::shared_ptr<PyScript>>(tmp)->m_Entity = m_Entity;
				m_PyScriptInstance = tmp;
			}
			catch (pybind11::error_already_set& e) {
				RYM_ERROR(e.what());
			}
			catch (std::runtime_error& e) {
				RYM_ERROR(e.what());
			}
		}

		std::shared_ptr<Entity> GetGameEntity()
		{
			return m_Entity;
		}

		void Reload()
		{
			try {
				m_PyScriptModule.reload();
				auto tmp = m_PyScriptModule.attr(m_ModuleName.c_str())();
				pybind11::cast<std::shared_ptr<PyScript>>(tmp)->m_Entity = m_Entity;
				m_PyScriptInstance = tmp;
			}
			catch (pybind11::error_already_set& e) {
				RYM_ERROR(e.what());
				return;
			}
			catch (std::exception& e) {
				RYM_ERROR(e.what());
			}
		}

		void OnStart()
		{
			try {
				m_PyScriptInstance.attr("OnStart")();
			}
			catch (pybind11::error_already_set& e) {
				RYM_ERROR(e.what());
			}
		}

		void OnUpdate(float _delta)
		{
			try {
				m_PyScriptInstance.attr("OnUpdate")(_delta);
			}
			catch (pybind11::error_already_set& e) {
				RYM_ERROR(e.what());
			}
		}

		void OnQuit()
		{
			try {
				m_PyScriptInstance.attr("OnQuit")();
			}
			catch (pybind11::error_already_set& e) {
				RYM_ERROR(e.what());
			}
		}

		std::shared_ptr<Entity> m_Entity;
	private:
		std::string m_ModuleName;
		pybind11::module m_PyScriptModule;
		pybind11::object m_PyScriptInstance;
		friend class Scene;
	};
}
