#include "AlgoVis.h"

using namespace GLCore;
using namespace GLCore::Utils;

static std::pair<float, float> origin;
float coordLeft = -50;
float coordRight = 50;
float coordBottom = -50;
float coordTop = 50;
int coordWidth = abs(coordLeft) + abs(coordRight);
int coordHeight = abs(coordBottom) + abs(coordTop);
float left = coordLeft;
float right = coordRight;
float bottom = coordBottom;
float top = coordTop;
float width = coordWidth;
float height = coordHeight;

AlgoVis::AlgoVis()
	:Layer("C++ Pathfinding Algorithm Visualizer"), 
	m_CameraController(((float)Application::Get().GetWindow().GetWidth() / 
		(float)Application::Get().GetWindow().GetHeight(), false, 1.0f))
{
	float startZoom = 0.2f;
	m_CameraController.SetZoomLevel(startZoom);

	left = std::round(coordLeft * startZoom);
	right = std::round(coordRight * startZoom);
	bottom = std::round(coordBottom * startZoom);
	top = std::round(coordTop * startZoom);
	width = abs(left) + abs(bottom);
	height = abs(bottom) + abs(top);
	m_CameraController.GetCamera().SetProjection(left, right, bottom, top);
	origin = { left, bottom};
	grid = std::make_shared<Grid>((int)(height), (int)(width), m_CameraController.GetCamera().GetViewProjectionMatrix());
}
void AlgoVis::OnAttach()
{   // AlgoVis's gl prelims 
	EnableGLDebugging();
	// Enable Alpha
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
}
void AlgoVis::OnDetach()
{
}

////////// Event Handling /////////////
void AlgoVis::OnEvent(Event& event)
{ 
	glm::vec3 camPos = m_CameraController.GetCamPos();
	std::cout << camPos.x / m_CameraController.GetZoomLevel() << ", " << camPos.y / m_CameraController.GetZoomLevel() << std::endl;
	EventDispatcher dispatcher(event);
	// Camera
	m_CameraController.OnEvent(event);
	float currZoomLvl = m_CameraController.GetZoomLevel();
			std::cout << "Grid Height: " << grid->GetGridProperties().height << std::endl;
			std::cout << "Grid Height: " << grid->GetGridProperties().height << std::endl;
			
	dispatcher.Dispatch<MouseScrolledEvent>(
		[&](MouseScrolledEvent& e) {
			left = std::round(coordLeft * currZoomLvl);
			right = std::round(coordRight * currZoomLvl);
			bottom = std::round(coordBottom * currZoomLvl);
			top = std::round(coordTop * currZoomLvl);
			width = abs(left) + abs(bottom);
			height = abs(bottom) + abs(top);
			m_CameraController.GetCamera().SetProjection(left,right, bottom, top);
			origin = { left, bottom};
			grid->Resize((int)(height), (int)(width));
			return true;
		});
	dispatcher.Dispatch<WindowResizeEvent>(
		[&](WindowResizeEvent& e) {
			m_CameraController.GetCamera().SetProjection(coordLeft * currZoomLvl, coordRight * currZoomLvl, coordBottom * currZoomLvl, coordTop * currZoomLvl);
			return true;
		});
	dispatcher.Dispatch<MouseMovedEvent>(
		[&](MouseMovedEvent& e) { // Store Transformed (to coordinate System) Mouse Positions
			// Transform Screen Coordinates to Grid Coordinates
			std::cout << "Screen Mouse Coords of (" << e.GetX() << ", " << e.GetY() << ")" << std::endl;
			TransformMousePos(e.GetX(), e.GetY(), currZoomLvl);
			return true;
		});
	dispatcher.Dispatch<MouseButtonReleasedEvent>(
		[&](MouseButtonReleasedEvent& e) {
			if (e.GetMouseButton() == MOUSE_BUTTON_1) {
				grid->SetCellState(state.mouseX, state.mouseY, cellState::VISITED);	
			}
			return true; // event handled
		});

}	
////////// Game Loop Layer /////////////
void AlgoVis::OnUpdate(Timestep ts)
{
	Application::Get().GetWindow().Clear(colors.Background.x, colors.Background.y, 
										 colors.Background.z, colors.Background.w);
	renderer.Clear(false);

	m_CameraController.SetCamTranslationSpeed(10.0f);
	m_CameraController.OnUpdate(ts);
	
	grid->UpdateViewProjection(m_CameraController.GetCamera().GetViewProjectionMatrix());
	grid->Draw(origin);
	
	grid->FillGrid(origin);
	
}
/*  UI IMGUI Render Layer  */
void AlgoVis::OnImGuiRender()
{

}

void AlgoVis::TransformMousePos(float const scrMouseX, float const scrMouseY, float currZoom)
{
	// Transform the mouse positions from screen space into our Coordinate system
	int coordSysWidth = (int)(width * currZoom);
	state.mouseX = (coordSysWidth) * (scrMouseX / 1000);
	state.mouseY = (coordSysWidth) - ((coordSysWidth) * (scrMouseY / 1000));
	std::cout << "Transformed Mouse Coords of (" << state.mouseX << ", " << state.mouseY << ")" << std::endl;
}

















