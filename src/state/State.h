#ifndef STATE_H
#define STATE_H

#include <vector>
#include <chrono>
#include <random>
#include "../objects/Minion.h"
#include "../objects/Boundary.h"

class State {
public:
    static State& getInstance();
    virtual ~State();

    void spawnMinions(int n);
    void initBoundary(float r);

    void draw();
    void update(float dt);
    
    void endProgram();
    bool getShouldEndProgram() const;

private:
    std::default_random_engine rng;

    std::vector< std::shared_ptr<Minion> > minions;
    std::shared_ptr<Boundary> boundary;

    bool shouldEndProgramFlag;
    State();
};

#endif /* STATE_H */

