#pragma once
#include <string_view>
#include <string>

namespace rym
{
	enum class mLevel
	{
		Info, Warning, error
	};

	class ConsolePanel
	{
	public:
		static ConsolePanel* GetInstace();

		void PushMessage(const std::string_view& str, mLevel level = mLevel::Info);
		void PushString(const std::string_view& str);
		void Render();

	private:
		ConsolePanel() = default;
		void PushCommand(const std::string_view& str);

	private:
		std::string m_Buffer;
		static ConsolePanel* s_Instace;

		int m_StyleCount = 0;
	};
}