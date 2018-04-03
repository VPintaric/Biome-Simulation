#ifndef STATE_H
#define STATE_H

#include <vector>
#include <chrono>
#include <random>
#include <minion/factories/MinionGenerator.h>
#include <collision/CollisionInfo.h>
#include "minion/object/MinionObject.h"
#include "objects/Boundary.h"
#include "minion/Minion.h"
#include "minion/selection/Selection.h"

class State {
public:
    static State& getInstance();
    virtual ~State();

    void spawnMinions(int n);
    void initBoundary(float r);

    void draw();
    void update(float dt);
    void controlMinions(float dt);
    
    void endProgram();
    bool getShouldEndProgram() const;

    void setMinionGenerator(std::shared_ptr<MinionGenerator> gen);

    void setSelectionAlg(std::shared_ptr<Selection> sel);

    void setPGenerateRandomMinion(float p);
    float getPGenerateRandomMinion() const;

    std::reference_wrapper< std::default_random_engine > getRng();

    const std::vector< std::shared_ptr<Minion> > &getMinions() const;
    std::shared_ptr<Boundary> getBoundary() const;

private:
    std::default_random_engine rng;

    std::shared_ptr<MinionGenerator> minionGenerator;
    std::vector< std::shared_ptr<Minion> > minions;
    std::shared_ptr<Selection> selectionAlg;
    std::shared_ptr<Boundary> boundary;

    bool shouldEndProgramFlag;

    int nextMinionId;

    float pGenerateRandomMinion;

    std::shared_ptr<Minion> currentBestMinion;

    void initializeMinion(Minion &minion);
    State();
};

#endif /* STATE_H */

