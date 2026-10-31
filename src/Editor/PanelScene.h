#pragma once

#include "Panel.h"

class Application;

class PanelScene : public Panel
{
public:
	explicit PanelScene(Application& app) : Panel("Scene"), app(app) {}

protected:
	void BeforeBegin() override;
	void DrawContent() override;
	ImGuiWindowFlags WindowFlags() const override 
	{ 
		return ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse; 
	}	

private:
	Application& app;
};

