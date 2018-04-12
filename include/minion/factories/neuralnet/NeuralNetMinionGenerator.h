#ifndef BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H
#define BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H


#include "minion/factories/MinionGenerator.h"
#include <random>
#include <minion/factories/neuralnet/mutation/NeuralNetMutation.h>
#include <minion/factories/neuralnet/crossover/NeuralNetCrossover.h>

class NeuralNetMinionGenerator : public MinionGenerator {
public:
    NeuralNetMinionGenerator(std::vector<int> nnHiddenLayers = {40, 30});
    ~NeuralNetMinionGenerator() override;

    std::shared_ptr<Minion> generateMinion() override;

    std::shared_ptr<Minion> generateChild(std::shared_ptr<Minion> first, std::shared_ptr<Minion> second) override;

    void configureFromJSON(rjs::Value &root) override;

private:
    std::vector<int> nnHiddenLayers;
    std::vector<std::shared_ptr<NeuralNetMutation> > mutationOps;
    std::vector<std::shared_ptr<NeuralNetCrossover> > crossoverOps;
    std::vector<float> mutationOpProbs, crossoverOpProbs;

    std::shared_ptr<Minion> createRawMinion();
    std::shared_ptr<Minion> crossover(std::shared_ptr<Minion> first, std::shared_ptr<Minion> second);
    void mutate(std::shared_ptr<Minion> m);
    void fixProbabilities(std::vector<float> &v);
    void configAddMutationOp(rjs::Value& root);
    void configAddCrossoverOp(rjs::Value& root);
    int getRandomIndexFromProbs(std::vector<float> &probs);
};


#endif //BIOME_SIMULATION_NEURALNETMINIONGENERATOR_H
