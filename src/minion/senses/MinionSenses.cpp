#include "MinionSenses.h"

MinionSenses::MinionSenses() = default;
MinionSenses::~MinionSenses() = default;

void MinionSenses::addSenseData(const std::shared_ptr<SenseData>& si) {
    senseDatas.push_back(si);
}

std::shared_ptr<MinionSenses::SenseData> MinionSenses::popSenseData() {
    if(senseDatas.empty()){
        return nullptr;
    }

    auto ret = senseDatas.back();
    senseDatas.pop_back();
    return ret;
}

void MinionSenses::clearData() {
    senseDatas.clear();
}
