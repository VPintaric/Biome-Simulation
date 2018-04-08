#ifndef STATE_H
#define STATE_H

#include <vector>
#include <chrono>
#include <random>
#include <minion/factories/MinionGenerator.h>
#include <collision/CollisionInfo.h>
#include <config/JSONConfigurable.h>
#include "minion/object/MinionObject.h"
#include "objects/Boundary.h"
#include "minion/Minion.h"
#include "minion/selection/Selection.h"

class State : public JSONConfigurable {
public:
    static State& getInstance();

    virtual ~State();

    void spawnMinions();

    void initBoundary(float r);

    void draw();

    void update(float dt);

    void controlMinions(float dt);
    
    void endProgram();

    bool getShouldEndProgram() const;

    void setMinionGenerator(std::shared_ptr<MinionGenerator> gen);

    void setSelectionAlg(std::shared_ptr<Selection> sel);

    std::reference_wrapper< std::default_random_engine > getRng();

    const std::vector< std::shared_ptr<Minion> > &getMinions() const;

    std::shared_ptr<Boundary> getBoundary() const;

    void setPersistenceDirectory(std::string dirName);

    std::string getPersistenceDirectory();

    void persistCurrentGeneration();

    void loadMinionsFromFolder(std::string dirName);

    void configureFromJSON(rjs::Value &root) override;

    int getNMinions() const;

    void setNMinions(int nMinions);

private:
    std::default_random_engine rng;

    std::shared_ptr<MinionGenerator> minionGenerator;
    std::vector< std::shared_ptr<Minion> > minions;
    std::shared_ptr<Selection> selectionAlg;
    std::shared_ptr<Boundary> boundary;

    int nMinions;

    std::string persistenceDirectory;

    int nextPersistedGeneration;

    bool shouldEndProgramFlag;

    int nextMinionId;

    std::shared_ptr<Minion> currentBestMinion;

    void initializeMinion(Minion &minion);

    State();
};

#endif /* STATE_H */

