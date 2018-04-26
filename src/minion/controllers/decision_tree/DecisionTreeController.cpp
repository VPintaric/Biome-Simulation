#include <state/Log.h>
#include <minion/senses/simple/SimpleMinionSenses.h>
#include <helpers/MathHelpers.h>
#include "minion/controllers/decision_tree/DecisionTreeController.h"
#include <glm/gtc/constants.hpp>
#include <helpers/RNG.h>
#include <minion/controllers/decision_tree/DTTerminalNode.h>

DecisionTreeController::DecisionTreeController(int maxTreeDepth) : maxTreeDepth(maxTreeDepth) {
    accDecTree = std::make_shared<DecisionTree>(DTConst::__FACT_MAX__, DTConst::__ACCTYPE_MAX__);
    rotDecTree = std::make_shared<DecisionTree>(DTConst::__FACT_MAX__, DTConst::__ROTTYPE_MAX__);
}

std::shared_ptr<Minion> DecisionTreeController::lockMinion() {
    auto locked = m.lock();
    if(locked == nullptr){
        Log().Get(logERROR) << "Can't get a lock on minion pointer";
    }
    return locked;
}

std::shared_ptr<MinionController> DecisionTreeController::copy() {
    auto newCopy = std::make_shared<DecisionTreeController>(maxTreeDepth);

    newCopy->accDecTree = accDecTree->copy();
    newCopy->rotDecTree = rotDecTree->copy();

    return newCopy;
}

std::vector<float> DecisionTreeController::controlMinion(std::vector<float> senseData) {
    std::set<int> facts = preprocess(senseData);

    auto accRes = static_cast<DTConst::AccType>(accDecTree->evaluate(facts));
    auto rotRes = static_cast<DTConst::RotType>(rotDecTree->evaluate(facts));

    std::vector<float> res {
        DTConst::accTypeToVal.find(accRes)->second,
        DTConst::rotTypeToVal.find(rotRes)->second
    };

    return res;
}

std::set<int> DecisionTreeController::preprocess(std::vector<float> &senseData) {
    std::set<int> facts;

    auto minion = lockMinion();
    auto senses = std::dynamic_pointer_cast<SimpleMinionSenses>(minion->getSenses());

    if(senses == nullptr){
        Log().Get(logWARNING) << "DecisionTreeController can only preprocess data from SimpleMinionSenses!";
        return facts;
    }

    auto nSightLines = static_cast<int>((senseData.size() - 5) / 5);

    int leftSightLines, frontSightLines;
    leftSightLines = frontSightLines = nSightLines / 3;
    frontSightLines += nSightLines % 3;

    int sightLine = 0;
    while(sightLine < leftSightLines){
        auto detectedType = senseData[sightLine * 5 + 1];
        if(Math::approx(detectedType, 50.f)){
            facts.insert(DTConst::FACT_BODY_LEFT);
        } else if(Math::approx(detectedType, 100.f)) {
            facts.insert(DTConst::FACT_MINION_LEFT);
        } else if(Math::approx(detectedType, 150.f)) {
            facts.insert(DTConst::FACT_PELLET_LEFT);
        } else if(Math::approx(detectedType, 200.f)) {
            facts.insert(DTConst::FACT_BOUNDARY_LEFT);
        }
        ++sightLine;
    }

    while(sightLine < leftSightLines + frontSightLines){
        auto detectedType = senseData[sightLine * 5 + 1];
        if(Math::approx(detectedType, 50.f)){
            facts.insert(DTConst::FACT_BODY_FRONT);
        } else if(Math::approx(detectedType, 100.f)) {
            facts.insert(DTConst::FACT_MINION_FRONT);
        } else if(Math::approx(detectedType, 150.f)) {
            facts.insert(DTConst::FACT_PELLET_FRONT);
        } else if(Math::approx(detectedType, 200.f)) {
            facts.insert(DTConst::FACT_BOUNDARY_FRONT);
        }
        ++sightLine;
    }

    while(sightLine < nSightLines){
        auto detectedType = senseData[sightLine * 5 + 1];
        if(Math::approx(detectedType, 50.f)){
            facts.insert(DTConst::FACT_BODY_RIGHT);
        } else if(Math::approx(detectedType, 100.f)) {
            facts.insert(DTConst::FACT_MINION_RIGHT);
        } else if(Math::approx(detectedType, 150.f)) {
            facts.insert(DTConst::FACT_PELLET_RIGHT);
        } else if(Math::approx(detectedType, 200.f)) {
            facts.insert(DTConst::FACT_BOUNDARY_RIGHT);
        }
        ++sightLine;
    }

    int attrIdx = sightLine * 5;
    if(senseData[attrIdx] < minion->getSenses()->getRadius() - minion->getObject()->getRadius()){
        facts.insert(DTConst::FACT_SOMETHING_BEHIND);
    }
    ++attrIdx;

    if(senseData[attrIdx] > 75.f){
        facts.insert(DTConst::FACT_HEALTHY);
    } else if(minion->getLife() < 25.f){
        facts.insert(DTConst::FACT_UNHEALTHY);
    }
    ++attrIdx;

    if(senseData[attrIdx] >= 60.f){
        facts.insert(DTConst::FACT_MOVING_FAST);
    } else if(senseData[attrIdx] <= 20.f){
        facts.insert(DTConst::FACT_MOVING_SLOW);
    }
    ++attrIdx;

    if(senseData[attrIdx] >= -glm::quarter_pi<float>() && senseData[attrIdx] <= glm::quarter_pi<float>()){
        facts.insert(DTConst::FACT_FRONT_ALIGNED);
    } else if(senseData[attrIdx] <= -3.f * glm::quarter_pi<float>() && senseData[attrIdx] >= 3.f * glm::quarter_pi<float>()){
        facts.insert(DTConst::FACT_FRONT_MISALIGNED);
    }
    ++attrIdx;

    if(senseData[attrIdx] < 0.f){
        facts.insert(DTConst::FACT_ROTATING_COUNTERCLOCKWISE);
    } else if(senseData[attrIdx] > 0.f){
        facts.insert(DTConst::FACT_ROTATING_CLOCKWISE);
    }

    return facts;
}

void DecisionTreeController::setMinion(std::shared_ptr<Minion> m) {
    this->m = m;
}

void DecisionTreeController::initRandomTree(std::shared_ptr<DTBranchNode> node,
                                            std::uniform_int_distribution<int>& factsDistr,
                                            std::uniform_int_distribution<int>& resDistr, int d) {
    static std::uniform_real_distribution<float> uniform(0.f, 1.f);
    float pForTerminal = (float) d / maxTreeDepth;

    float roll = uniform(RNG::get());
    if(roll <= pForTerminal){
        node->left = std::make_shared<DTTerminalNode>(resDistr(RNG::get()));
    } else {
        auto newBranchNode = std::make_shared<DTBranchNode>(factsDistr(RNG::get()));
        node->left = newBranchNode;
        initRandomTree(newBranchNode, factsDistr, resDistr, d + 1);
    }

    roll = uniform(RNG::get());
    if(roll <= pForTerminal){
        node->right = std::make_shared<DTTerminalNode>(resDistr(RNG::get()));
    } else {
        auto newBranchNode = std::make_shared<DTBranchNode>(factsDistr(RNG::get()));
        node->right = newBranchNode;
        initRandomTree(newBranchNode, factsDistr, resDistr, d + 1);
    }
}

void DecisionTreeController::initRandom() {
    std::uniform_int_distribution<int> factsDistr(0, accDecTree->nFacts - 1);
    std::uniform_int_distribution<int> resDistr(0, accDecTree->nResults - 1);
    auto branchNode = std::make_shared<DTBranchNode>(factsDistr(RNG::get()));
    accDecTree->root = branchNode;
    initRandomTree(branchNode, factsDistr, resDistr, 1);

    factsDistr = std::uniform_int_distribution<int>(0, rotDecTree->nFacts - 1);
    resDistr = std::uniform_int_distribution<int>(0, rotDecTree->nResults - 1);
    branchNode = std::make_shared<DTBranchNode>(resDistr(RNG::get()));
    rotDecTree->root = branchNode;
    initRandomTree(branchNode, factsDistr, resDistr, 1);
}

void DecisionTreeController::persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) {
    root.AddMember(rjs::StringRef(MAX_DEPTH), rjs::Value(maxTreeDepth), alloc);

    rjs::Value accDTJSON(rjs::kObjectType);
    accDecTree->persistToJSON(accDTJSON, alloc);
    root.AddMember(rjs::StringRef(ACC_TREE), accDTJSON, alloc);

    rjs::Value rotDTJSON(rjs::kObjectType);
    rotDecTree->persistToJSON(rotDTJSON, alloc);
    root.AddMember(rjs::StringRef(ROT_TREE), rotDTJSON, alloc);
}

void DecisionTreeController::initFromJSON(rjs::Value &root) {
    maxTreeDepth = root[MAX_DEPTH].GetInt();

    rjs::Value accRoot = root[ACC_TREE].GetObject();
    accDecTree->initFromJSON(accRoot);

    rjs::Value rotRoot = root[ROT_TREE].GetObject();
    rotDecTree->initFromJSON(rotRoot);
}

void DecisionTreeController::pruneTrees() {
    accDecTree->randomTreePruning(maxTreeDepth);
    rotDecTree->randomTreePruning(maxTreeDepth);
}

void DecisionTreeController::printDepth() {
    Log().Get(logINFO) << "Acceleration control tree depth: " << accDecTree->getDepth();
    Log().Get(logINFO) << "Rotation control tree depth: " << rotDecTree->getDepth();
}