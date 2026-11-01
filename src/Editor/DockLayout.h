#pragma once

#include <imgui.h>

namespace DockLayout {

// Fixed identifier, so the layout stored in imgui.ini is recognized on the next run.
inline constexpr ImGuiID kDockspaceId = 0x4D4F5452;

// When there is no saved layout (first run, or imgui.ini deleted) the windows are placed like this:
//
//   +-----------+---------------------+-----------+
//   | Hierarchy |        Scene        | Inspector |
//   |           |                     | Configur. |
//   +-----------+---------------------+-----------+
//   |                  Console                    |
//   +---------------------------------------------+
//
// After that, ImGui remembers whatever the user does.

void Draw();
}
