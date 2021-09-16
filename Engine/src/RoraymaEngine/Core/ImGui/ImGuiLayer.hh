#pragma once

#include <string>

#include "../Layer.hh"
#include "IconsFontAwesome5.h"
#include "IconsFontAwesome5Brands.h"
//#include "IconsKenney.h"

namespace rym
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual void OnStart() override;
		virtual void OnQuit() override;
		virtual void OnEvent(const Event& events) override;
		void BlockEvents(bool block) { m_BlockEvents = block; }
		void Begin();
		void End();
	private:
		bool m_BlockEvents = true;
	};
}