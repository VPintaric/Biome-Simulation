#ifndef STATE_H
#define STATE_H

#include <vector>
#include <chrono>
#include <random>
#include <minion/factories/MinionGenerator.h>
#include <collision/CollisionInfo.h>
#include <config/JSONConfigurable.h>
#include <set>
#include <minion/fitness/Fitness.h>
#include <minion/crossover_operators/MinionCrossover.h>
#include <minion/mutation_operators/MinionMutation.h>
#include "minion/object/MinionObject.h"
#include "objects/Boundary.h"
#include "minion/Minion.h"
#include "minion/selection/Selection.h"

// This class really need a do-over.......

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

    void setFitnessAlg(std::shared_ptr<Fitness> fitness);

    const std::vector< std::shared_ptr<Minion> > &getMinions() const;

    std::shared_ptr<Boundary> getBoundary() const;

    void setPersistenceDirectory(std::string dirName);

    std::string getPersistenceDirectory();

    void persistCurrentGeneration();

    void loadMinionsFromFolder(std::string dirName);

    void configureFromJSON(rjs::Value &root) override;

    int getNMinions() const;

    void setNMinions(int nMinions);

    void resolveMinionCollisionDamage(Minion& m1, Minion& m2, CollisionInfo& ci);

    void resolveMinionBoundaryCollisionDamage(Minion& m, CollisionInfo& ci);

private:
    std::shared_ptr<MinionGenerator> minionGenerator;

    std::vector< std::shared_ptr<Minion> > minions;

    std::set< std::shared_ptr<Minion> > decayedMinions;

    std::shared_ptr<Selection> selectionAlg;

    std::shared_ptr<Fitness> fitnessAlg;

    std::shared_ptr<MinionCrossover> crossover;

    std::shared_ptr<MinionMutation> mutation;

    std::shared_ptr<Boundary> boundary;

    int nMinions, nElites;

    std::string persistenceDirectory;

    std::string loadDirectory;

    int nextPersistedGeneration;

    bool shouldEndProgramFlag;

    int nextMinionId;

    std::shared_ptr<Minion> currentBestMinion;

    long realTimePassed;

    long printEveryRealTime, persistMinionsEveryRealTime;

    long nextPrintTimestamp, nextPersistTimestamp;

    long lastCalledTimestamp;

    bool useGenerationalGA;

    int currentGeneration;

    float currentFitnessAverage;

    void initializeMinion(Minion &minion);

    void initializeNextGeneration();

    void realTimeUpdate();

    void updateMinionFitness();

    State();
};

#endif /* STATE_H */

