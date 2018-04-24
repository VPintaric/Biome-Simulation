#ifndef BIOME_SIMULATION_DECISIONTREE_H
#define BIOME_SIMULATION_DECISIONTREE_H


#include <copyable/Copyable.h>
#include <set>
#include <persistence/JSONPersistable.h>
#include "DTNode.h"

class DTNode;

class DecisionTree : public Copyable<DecisionTree>, public JSONPersistable {
private:
    const char * N_FACTS = "n_facts";
    const char * N_RESULTS = "n_results";
    const char * NODES = "nodes";

    void randomPruningRec(std::shared_ptr<DTNode> node, int d, int maxBranchDepth);

public:

    int nFacts, nResults;

    std::shared_ptr<DTNode> root;

    DecisionTree(int nUniqueFacts, int nUniqueResults);

    std::shared_ptr<DecisionTree> copy() override;

    int evaluate(const std::set<int>& facts);

    void persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) override;

    void initFromJSON(rjs::Value &root) override;

    void randomTreePruning(int maxDepth);
};


#endif //BIOME_SIMULATION_DECISIONTREE_H
