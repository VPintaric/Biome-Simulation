#include <state/Log.h>
#include "minion/controllers/MinionController.h"

MinionController::MinionController() = default;
MinionController::~MinionController() = default;

void MinionController::persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) {
    Log().Get(logWARNING) << "This minion controller does not implement persistence via JSON";
}

void MinionController::initFromJSON(rjs::Value &root) {
    Log().Get(logWARNING) << "This minion controller does not implement persistence via JSON";
}
