#define GLM_FORCE_RADIANS

#include <minion/senses/MinionSenses.h>
#include <state/Log.h>

MinionSenses::MinionSenses() = default;
MinionSenses::~MinionSenses() = default;

void MinionSenses::persistToJSON(rjs::Value &root, rjs::Document::AllocatorType &alloc) {
    root.SetObject();

    root.AddMember(rjs::StringRef(JSON_MAX_SENSE_DISTANCE), rjs::Value(getMaxSenseDistance()), alloc);
}

void MinionSenses::initFromJSON(rjs::Value &root) {
    auto distance = root.FindMember(rjs::StringRef(JSON_MAX_SENSE_DISTANCE));
    if(distance == root.MemberEnd() || !distance->value.IsFloat()){
        Log().Get(logWARNING) << "\"" << JSON_MAX_SENSE_DISTANCE << "\" not found or invalid in JSON, can't initialize";
    } else {
        setMaxSenseDistance(distance->value.GetFloat());
    }
}
