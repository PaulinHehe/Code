#include "game_state.h"
#include <algorithm>

GameState::GameState(const Matrix<double> &gAtt, const std::vector<Position<int>> &posS, const std::vector<int> &slotD, const std::vector<int> &nodeD):
    graphAttributes{gAtt}, slotsPos{posS}, slotData{slotD}, nodeData{nodeD}
{
    init();
}

void GameState::init()
{
    if (!slotsPos.empty())
    {
        xMin = xMax = slotsPos[0].getX();
        yMin = yMax = slotsPos[0].getY();
        for (int slot = 1; slot < getNbSlots(); slot++)
        {
            xMin = min(xMin, slotsPos[slot].getX());
            xMax = max(xMax, slotsPos[slot].getX());
            yMin = min(yMin, slotsPos[slot].getY());
            yMax = max(yMax, slotsPos[slot].getY());
        }
    }
    nextNode = 0;
    while (nextNode < getNbNodes() && nodeData[nextNode] != -1)
    {
        nextNode++;
    }
}

Matrix<double> GameState::getAsInputMatrix() const
{
    int sizeInput = graphAttributes.height() + 3 * getNbSlots() + 3 * getNbNodes();
    Matrix<double> input{sizeInput, 1};
    int line = 0;
    for (int x = 0; x < graphAttributes.height(); x++)
    {
        input[line++][0] = graphAttributes[x][0];
    }
    for (int slot = 0; slot < getNbSlots(); slot++)
    {
        if (slotData[slot] == -1) input[line++][0] = 1.0;
        else input[line++][0] = 0.0;
        Position<double> pos;
        _convertCoordDouble(slotsPos[slot], pos);
        input[line++][0] = pos.getX();
        input[line++][0] = pos.getY();
    }
    for (int node = 0; node < getNbNodes(); node++)
    {
        if (node == nextNode) input[line++][0] = 1.0;
        else input[line++][0] = 0.0;
        Position<double> pos = {0, 0};
        if (nodeData[node] != -1) _convertCoordDouble(slotsPos[nodeData[node]], pos);
        input[line++][0] = pos.getX();
        input[line++][0] = pos.getY();
    }
    return input;
}

int GameState::getNbSlots() const
{
    return (int)slotData.size();
}

int GameState::getNbNodes() const
{
    return (int)nodeData.size();
}

int GameState::getNextNode() const
{
    return nextNode;
}

std::vector<int> GameState::getSlotData() const
{
    return slotData;
}

std::vector<int> GameState::getNodeData() const
{
    return nodeData;
}

bool GameState::isGameFinished() const
{
    return nextNode == getNbNodes();
}

bool GameState::isSlotAvailable(int slot) const
{
    return slotData[slot] == -1;
}

bool GameState::placeNode(int slot)
{
    if (nextNode == getNbNodes() || slotData[slot] != -1) return false;
    nodeData[nextNode] = slot;
    slotData[slot] = nextNode;
    nextNode++;
    return true;
}

bool GameState::removeNodeFromSlot()
{
    if (nextNode == 0) return false;
    nextNode--;
    slotData[nodeData[nextNode]] = -1;
    nodeData[nextNode] = -1;
    return true;
}

void GameState::_convertCoordDouble(const Position<int> posInt, Position<double>& posDouble) const
{
    int xDiff = posInt.getX() - xMin;
    int yDiff = posInt.getY() - yMin;
    int totalXDiff = xMax - xMin;
    int totalYDiff = yMax - yMin;
    if (totalXDiff != 0) posDouble.setX((double)xDiff / (double)totalXDiff);
    else posDouble.setX(0.0);
    if (totalYDiff != 0) posDouble.setY((double)yDiff / (double)totalYDiff);
    else posDouble.setY(0.0);
}
