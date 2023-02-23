#include "Astar.h"

namespace Algorithms
{

	int Astar::EuclidHeuristic(int r, int c)
	{
		int endR = endCoord.first;
		int endC = endCoord.second;
		// Euclidean distance Heuristic
		return (int)(sqrt(pow((endR - r),2) + pow((endC - c),2)));
	}
	int Astar::ChebyshevHeuristic(int r, int c)
	{
		int endR = endCoord.first;
		int endC = endCoord.second;
		int dx = abs(endR - r);
		int dy = abs(endC - c);
		return std::max(dx, dy);
	}
	int Astar::ManhattanHeuristic(int r, int c)
	{
		int endR = endCoord.first;
		int endC = endCoord.second;
		// Euclidean distance Heuristic
		return abs((endR - r)) + abs((endC - c));
	}
	
	// Init 
	void Astar::Init(std::pair<int, int> start)
	{
		// start clean
		PathFinder::Init(start);
		// costs hash
		Gcost.clear();
		for (int id = 0; id < m_Grid->getHeight() * m_Grid->getWidth(); id++) {
			Gcost[id] = INT_MAX;
		}
		// Clear Previous Queue if Any
		minQ = {};
		// push Cell ID
		minQ.push({ m_Grid->getCellID(start.first, start.second), std::make_pair(0, 0)});
		// Init Hash with initial distance of zero for the start node
		Gcost[minQ.top().first] = 0;
		// Initialize Coords
		startCoord = m_Grid->getStartCoord();
		endCoord = m_Grid->getEndCoord();
	}
	
	// Update 
	bool Astar::Update()
	{
		std::cout << "RUNNING Astar ALGO..." << std::endl;
		// if q is NOT empty AND end has Not be found yet, keep searching!
		if (!minQ.empty() && !endFound)
		{
			int cell = minQ.top().first;
			int currGscore = Gcost[cell] + 1;
			int r0 = m_Grid->getCellCoord(cell).first;
			int c0 = m_Grid->getCellCoord(cell).second;
			minQ.pop();
			m_Grid->setCellState(r0, c0, cellState::VISITED);
			// iterate every neighbor
			for (int i = 0; i < m_numSearchDirections; i++)
			{
				int r = r0 + dr4[i];
				int c = c0 + dc4[i];
				if (m_numSearchDirections == 8) {
					r = r0 + dr8[i];
					c = c0 + dc8[i];
				}
				// boundary check
				if (r < 0 || c < 0 || r >= m_Grid->getHeight() || c >= m_Grid->getWidth()) { continue; }
				// skip wall cells
				if (m_Grid->getCellType(r, c) == cellType::WALL) continue;
				//if (m_Grid->getCellState(r, c) == cellState::VISITED || m_Grid->getCellState(r, c) == cellState::VISITING) continue;
				// 
				/// Check if this new cell is the end point
				if (m_Grid->getCellType(r, c) == cellType::END)
				{
					InitPath(cell);
					endFound = true;
					break;
				}
				// set unvisited node as visiting
				// save nodes parent
				
				// Calculations
				// update costs of this cell
				if (currGscore < Gcost[m_Grid->getCellID(r, c)]) {
					m_Grid->setCellState(r, c, cellState::VISITING);
					Gcost[m_Grid->getCellID(r, c)] = currGscore + m_Grid->getCellWeight(r, c);
					int GCost = Gcost[m_Grid->getCellID(r, c)];
					int HCost = 0;
					if (m_numSearchDirections == 4) {
						HCost = EuclidHeuristic(r, c);
						//HCost = ManhattanHeuristic(r, c);
					}
					else { HCost = EuclidHeuristic(r, c); }
					int FCost = GCost + HCost;
					parentHash[m_Grid->getCellID(r, c)] = { r0, c0 };
					minQ.push({ m_Grid->getCellID(r, c), std::make_pair(FCost, HCost) });
				}
				
				
				
			}
			return true; // Algorithm continues running
		}
		// Program only reaches this portion if End is found Or Queue was empty
		// is path update returns true, it means we continue to update next frame
		return PathUpdate();
	}

	bool Astar::PathUpdate()
	{
		if (!endFound) return false;
		if (m_Grid->getCellType(currCell) != cellType::START)
		{
			m_Grid->setCellType(m_Grid->getCellCoord(currCell).first, m_Grid->getCellCoord(currCell).second, cellType::PATH);
			currCell = m_Grid->getCellID(parentHash[currCell].first, parentHash[currCell].second);
			return true;
		}
		return false;
	}

}