#ifndef STATE_H
#define STATE_H

class State {
public:
    static State& getInstance();
    virtual ~State();

    void endProgram();
    bool getShouldEndProgram() const;
private:
    bool shouldEndProgramFlag;
    State();
};

#endif /* STATE_H */

