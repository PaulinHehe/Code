#include "edge_mcts.h"

EdgeMCTS::EdgeMCTS(int nodeT, int slotT, int nextNode, double P):
    Pvalue{P}, Nvalue{0}, VsumValue{0}, nodeTransition{nodeT}, slotTransition{slotT}, nextNodeMCTS{nextNode}
{

}

double EdgeMCTS::getPvalue() const
{
    return Pvalue;
}

int EdgeMCTS::getNvalue() const
{
    return Nvalue;
}

double EdgeMCTS::getVsumValue() const
{
    return VsumValue;
}

double EdgeMCTS::getQvalue() const
{
    if (Nvalue == 0) return 0.0;
    double Q = VsumValue / (double)Nvalue; //Val between 0 and 1
    return Q; //Val between -1 and 1 (2Q-1)
}

double EdgeMCTS::getUvalue() const
{
    return Pvalue / (1.0 + (double)Nvalue);
}

double EdgeMCTS::getEdgeValue() const
{
    return getQvalue() + getUvalue();
}

int EdgeMCTS::getNodeTransition() const
{
    return nodeTransition;
}

int EdgeMCTS::getSlotTransition() const
{
    return slotTransition;
}

int EdgeMCTS::getNextNodeMCTS() const
{
    return nextNodeMCTS;
}

void EdgeMCTS::addVvalue(double V)
{
    Nvalue++;
    VsumValue += V;
}

ostream& operator<<(ostream& ost, const EdgeMCTS &edge)
{
    ost << "NN=" << edge.getNextNodeMCTS() << ",NT=" << edge.getNodeTransition() << ",ST=" << edge.getSlotTransition();
    ost << ",P=" << edge.getPvalue() << ",N=" << edge.getNvalue() << ",V=" << edge.getVsumValue();
    ost << ",Q=" << edge.getQvalue() << ",U=" << edge.getUvalue() << ",T=" << edge.getEdgeValue();
    return ost;
}
