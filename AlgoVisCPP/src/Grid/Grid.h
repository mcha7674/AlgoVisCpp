#pragma once
#include <GLCoreUtils.h> 
#include <GLAbstraction.h> 
#include <GLCore.h>
#include <vector>
#include "../Base_Models/Quad.h"
#include "GridRender.h"


enum class cellState { VISITED = 0, UNVISITED, VISITING };
enum class cellType { NORMAL = 0, START, END, WALL, PATH };
 // color static vectors:

// Grid Cell Properties //
struct Cell
{
	Cell() { reset(); }
	void reset()
	{
		weight = 0;
		m_State = cellState::UNVISITED;
		m_Type = cellType::NORMAL;
	}
	int ID;
	int weight;
	cellType m_Type;
	cellState m_State;
};
// Grid Properties //
struct GridProps {
	int width = 0;
	int height = 0;
	glm::vec4 gridColor = { 0.0f,0.0f,0.0f,1.0f };

	bool startPointSet = false;
	bool endPointSet = false;
	std::pair<int, int>startCoord;
	std::pair<int, int>endCoord;
	GridProps() { reset(); }
	void reset() {
		startPointSet = false;
		endPointSet = false;
		startCoord = { 0, 0 };
		endCoord = { 0, 0 };
	}
};

class Grid
{
public:
	Grid(int row, int col, glm::mat4 ViewProjMatrix);
	~Grid() = default;
	// Reset Functions
	void clearBoard();
	void clearPath();
	void reset();
	// Getters
	inline Cell getCell(int r, int c) { return grid[r][c]; }
	inline Cell getCell(std::pair<int,int> coord) { return grid[coord.first][coord.second]; }
	inline cellType getCellType(int cellID) { return grid[cellHash[cellID].first][cellHash[cellID].second].m_Type; }
	inline cellType getCellType(int r, int c) { return grid[r][c].m_Type; }
	inline cellType getCellType(std::pair<int, int> coord) { return grid[coord.first][coord.second].m_Type; }
	inline cellState getCellState(int r, int c) { return grid[r][c].m_State; }

	inline int getCellID(int r, int c) { return grid[r][c].ID; }
	inline std::pair<int, int> getCellCoord(int ID) { return cellHash[ID]; }
	inline int getHeight() { return gridProps->height; }
	inline int getWidth() { return gridProps->width; }
	inline std::pair<int, int> getStartCoord() { return gridProps->startCoord; }
	inline std::pair<int, int> getEndCoord() { return gridProps->endCoord; }
	std::vector<std::vector<Cell>> getGrid() { return grid; }	
	// Setters
	void setCellState(int row, int col, cellState state);
	void setCellType(int row, int col, cellType type);
	inline void setGrid(std::vector<std::vector<Cell>> GRID) { grid = GRID; };
	// States //
	bool isStartSet() { return (gridProps->startPointSet); }
	bool isEndSet() { return (gridProps->endPointSet); }
	bool isStartAndEndSet() { return (gridProps->startPointSet && gridProps->endPointSet); }
	// Render Grid
	void RenderGrid();
	void RenderGridLines();
	void setGridColor(float r, float g, float b, float a = 1.0f);
	void setGridColor(glm::vec3 color);
private:
	std::shared_ptr<GridProps> gridProps;
	std::vector<std::vector<Cell>> grid;
	std::unordered_map <int, std::pair<int, int> > cellHash;
	// Rendering
	GridRender gridRender;
	Renderer renderer;
	Quad fillQuad;
	glm::mat4 ViewProjectionMatrix;
};




