#ifndef STATE_H
#define STATE_H

#include <vector>
#include "../objects/Minion.h"

class State {
public:
    static State& getInstance();
    virtual ~State();

    void spawnMinions(int n);
    void drawMinions();
    void updateMinions(float deltaT);
    
    void endProgram();
    bool getShouldEndProgram() const;
private:
    std::vector< std::shared_ptr<Minion> > minions;
    int nMinions;
    
    bool shouldEndProgramFlag;
    State();
};

#endif /* STATE_H */

