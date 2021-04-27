#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

namespace py = pybind11;

#include <RoraymaEngine/Scene/Components.hh>
#include <RoraymaEngine/Scene/PyScript.hh>
#include <RoraymaEngine/Core/Input.hh>

using namespace rym;

PYBIND11_EMBEDDED_MODULE(PyScripting, m) {
	//m.doc() = ""; // optional module docstring
	py::module m_Math = m.def_submodule("Math");
	py::module m_Input = m.def_submodule("Input");

	py::class_<glm::vec2, std::shared_ptr<glm::vec2>>(m_Math, "Vec2")
		.def(py::init<float>())
		.def(py::init<float, float>())
		.def(py::init<const glm::vec2&>())
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self += py::self)
		.def(py::self -= py::self)
		.def(py::self * py::self)
		.def(py::self == py::self)
		.def(py::self != py::self)
		.def(float() * py::self)
		.def(py::self * float())
		.def(py::self / float())
		.def(-py::self)
		.def("__str__", [](glm::vec2* v) {
			return"[" + std::to_string(v->x) + ", " + std::to_string(v->y) + "]";
			})
		.def("__repr__", [](glm::vec2* v) {
			return"[" + std::to_string(v->x) + ", " + std::to_string(v->y) + "]";
			})
		.def_readwrite("x", &glm::vec2::x)
		.def_readwrite("y", &glm::vec2::y);

	py::class_<glm::vec3, std::shared_ptr<glm::vec3>>(m_Math, "Vec3")
		.def(py::init<float>())
		.def(py::init<float, float, float>())
		.def(py::init<const glm::vec3&>())
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self += py::self)
		.def(py::self -= py::self)
		.def(py::self * py::self)
		.def(py::self == py::self)
		.def(py::self != py::self)
		.def(float() * py::self)
		.def(py::self * float())
		.def(py::self / float())
		.def(-py::self)
		.def("__str__", [](glm::vec3* v) {
			return "[" + std::to_string(v->x) + ", " + std::to_string(v->y) + ", " + std::to_string(v->z) + "]";
			})
		.def("__repr__", [](glm::vec3* v) {
			return "[" + std::to_string(v->x) + ", " + std::to_string(v->y) + ", " + std::to_string(v->z) + "]";
			})
		.def_readwrite("x", &glm::vec3::x)
		.def_readwrite("y", &glm::vec3::y)
		.def_readwrite("z", &glm::vec3::z);

	py::class_<glm::vec4, std::shared_ptr<glm::vec4>>(m_Math, "Vec4")
		.def(py::init<float>())
		.def(py::init<float, float, float, float>())
		.def(py::init<const glm::vec4&>())
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self += py::self)
		.def(py::self -= py::self)
		.def(py::self == py::self)
		.def(py::self != py::self)
		.def(py::self * py::self)
		.def(float() * py::self)
		.def(py::self * float())
		.def(py::self / float())
		.def(-py::self)
		.def("__str__", [](glm::vec4* v) {
			return "[" + std::to_string(v->x) + ", " + std::to_string(v->y) + ", " + std::to_string(v->z) + ", " + std::to_string(v->w) + "]";
			})
		.def("__repr__", [](glm::vec4* v) {
				return "[" + std::to_string(v->x) + ", " + std::to_string(v->y) + ", " + std::to_string(v->z) + ", " + std::to_string(v->w) + "]";
			})
		.def_readwrite("x", &glm::vec4::x)
		.def_readwrite("y", &glm::vec4::y)
		.def_readwrite("z", &glm::vec4::z)
		.def_readwrite("w", &glm::vec4::w);

	py::class_<glm::mat4, std::shared_ptr<glm::mat4>>(m_Math, "Mat4")
		.def(py::init<float>())
		.def(py::init<const glm::mat4&>())
		.def(py::self + py::self)
		.def(py::self - py::self)
		.def(py::self += py::self)
		.def(py::self -= py::self)
		.def(py::self * py::self)
		.def(py::self == py::self)
		.def(py::self != py::self)
		.def(float() * py::self)
		.def(py::self * float())
		.def(py::self / float())
		.def("__str__", [](glm::mat4* m) {
		auto& tmpM = (*m);
		return 
			"[" + 
			std::to_string(tmpM[0].x) + ", " + std::to_string(tmpM[0].y) + ", " + std::to_string(tmpM[0].z) + ", " + std::to_string(tmpM[0].w) + "\n " +
			std::to_string(tmpM[1].x) + ", " + std::to_string(tmpM[1].y) + ", " + std::to_string(tmpM[1].z) + ", " + std::to_string(tmpM[1].w) + "\n " +
			std::to_string(tmpM[2].x) + ", " + std::to_string(tmpM[2].y) + ", " + std::to_string(tmpM[2].z) + ", " + std::to_string(tmpM[2].w) + "\n " +
			std::to_string(tmpM[3].x) + ", " + std::to_string(tmpM[3].y) + ", " + std::to_string(tmpM[3].z) + ", " + std::to_string(tmpM[3].w)
			+ "]";
			})
		.def("__repr__", [](glm::mat4* m) {
				auto& tmpM = (*m);
				return
					"[" +
					std::to_string(tmpM[0].x) + ", " + std::to_string(tmpM[0].y) + ", " + std::to_string(tmpM[0].z) + ", " + std::to_string(tmpM[0].w) + "\n " +
					std::to_string(tmpM[1].x) + ", " + std::to_string(tmpM[1].y) + ", " + std::to_string(tmpM[1].z) + ", " + std::to_string(tmpM[1].w) + "\n " +
					std::to_string(tmpM[2].x) + ", " + std::to_string(tmpM[2].y) + ", " + std::to_string(tmpM[2].z) + ", " + std::to_string(tmpM[2].w) + "\n " +
					std::to_string(tmpM[3].x) + ", " + std::to_string(tmpM[3].y) + ", " + std::to_string(tmpM[3].z) + ", " + std::to_string(tmpM[3].w)
					+ "]";
			})
		.def("__getitem__", [](glm::mat4* m, size_t index) {
		return (*m)[index];
		})
		.def("__settitem__", [](glm::mat4* m, size_t index, glm::vec4* v) {
		return (*m)[index] = *v;
		})
		.def(-py::self);

	py::enum_<KeysCode>(m_Input, "KeyCode")
		.value("A", KeysCode::A)
		.value("B", KeysCode::B)
		.value("C", KeysCode::C)
		.value("D", KeysCode::D)
		.value("E", KeysCode::E)
		.value("F", KeysCode::F)
		.value("G", KeysCode::G)
		.value("H", KeysCode::H)
		.value("I", KeysCode::I)
		.value("J", KeysCode::J)
		.value("K", KeysCode::K)
		.value("L", KeysCode::L)
		.value("M", KeysCode::M)
		.value("N", KeysCode::N)
		.value("O", KeysCode::O)
		.value("P", KeysCode::P)
		.value("Q", KeysCode::Q)
		.value("R", KeysCode::R)
		.value("S", KeysCode::S)
		.value("T", KeysCode::T)
		.value("U", KeysCode::U)
		.value("V", KeysCode::V)
		.value("W", KeysCode::W)
		.value("X", KeysCode::X)
		.value("Y", KeysCode::Y)
		.value("Z", KeysCode::Z)
		.value("Escape", KeysCode::Escape)
		.value("Enter", KeysCode::Enter)
		.value("Tab", KeysCode::Tab)
		.value("Backspace", KeysCode::Backspace)
		.value("Right", KeysCode::Right)
		.value("Left", KeysCode::Left)
		.value("Down", KeysCode::Down)
		.value("Up", KeysCode::Up)
		.value("PrintScreen", KeysCode::PrintScreen)
		.value("F1", KeysCode::F1)
		.value("F2", KeysCode::F2)
		.value("F3", KeysCode::F3)
		.value("F4", KeysCode::F4)
		.value("F5", KeysCode::F5)
		.value("F6", KeysCode::F6)
		.value("F7", KeysCode::F7)
		.value("F8", KeysCode::F8)
		.value("F9", KeysCode::F9)
		.value("F10", KeysCode::F10)
		.value("F11", KeysCode::F11)
		.value("F12", KeysCode::F12);

	py::enum_<MouseCode>(m_Input, "ButtonCode")
		.value("Left", MouseCode::ButtonLeft)
		.value("Right", MouseCode::ButtonRight)
		.value("Middle", MouseCode::ButtonMiddle);

	m_Input.def("IsKeyPressed", &Input::IsKeyPressed);
	m_Input.def("IsKeyRealese", &Input::IsKeyRealese);
	m_Input.def("IsKeyJustPressed", &Input::IsKeyJustPressed);
	m_Input.def("IsButtonPressed", &Input::IsButtonPressed);
	m_Input.def("IsButtonRelease", &Input::IsButtonRelease);
	m_Input.def("IsButtonJustPressed", &Input::IsButtonJustPressed);
	m_Input.def("GetCursorGlobalPosition", &Input::GetCursorPosition);
	m_Input.def("GetScroll", &Input::GetMouseScrolled);

	py::class_<rym::Color, std::shared_ptr<Color>>(m, "Color")
		.def(py::init<float, float, float, float>())
		.def(py::init<const rym::Color&>())
		.def(py::init<const glm::vec4&>())
		.def_static("WHITE", []() {return rym::Color::WHITE; })
		.def_static("BLACK", []() {return rym::Color::BLACK; })
		.def_static("RED", []() {return rym::Color::RED; })
		.def_static("GREEN", []() {return rym::Color::GREEN; })
		.def_static("BLUE", []() {return rym::Color::BLUE; })
		.def_static("YELLOW", []() {return rym::Color::YELLOW; })
		.def_static("CYAN", []() {return rym::Color::CYAN; })
		.def_static("MAGENTA", []() {return rym::Color::MAGENTA; })
		.def_readwrite("rgba", &rym::Color::rgba);

	py::class_<TransformComponent, std::shared_ptr<TransformComponent>>(m, "TransformComponent")
		.def(py::init<>())
		.def_readwrite("position", &TransformComponent::translation)
		.def_readwrite("scale", &TransformComponent::scale)
		.def_readwrite("rotation", &TransformComponent::rotation);

	py::class_<SpriteComponent, std::shared_ptr<SpriteComponent>>(m, "SpriteComponent")
		.def(py::init<>())
		.def_readwrite("flipH", &SpriteComponent::flipH)
		.def_readwrite("flipV", &SpriteComponent::flipV)
		.def_readwrite("size", &SpriteComponent::size)
		.def_readwrite("selfColor", &SpriteComponent::color);

	py::class_<Entity, std::shared_ptr<Entity>>(m, "Entity")
		.def(py::init<>())
		.def("GetTransformComponent", &Entity::GetComponent<TransformComponent>, pybind11::return_value_policy::reference)
		.def("GetSpriteComponent", &Entity::GetComponent<SpriteComponent>, pybind11::return_value_policy::reference)
		.def("GetCameraComponent", &Entity::GetComponent<CameraComponent>, pybind11::return_value_policy::reference)
		.def("HaveTransformComponent", &Entity::HaveComponent<TransformComponent>)
		.def("HaveSpriteComponent", &Entity::HaveComponent<SpriteComponent>)
		.def("HaveCameraComponent", &Entity::HaveComponent<CameraComponent>)
		.def_readwrite("tag", &Entity::Tag)
		.def_readwrite("visible", &Entity::visible);

	py::class_<PyScript, std::shared_ptr<PyScript>>(m, "PyScript")
		.def(py::init<>())
		.def_readonly("entity", &PyScript::m_Entity);
}
