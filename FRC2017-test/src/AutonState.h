#ifndef AUTOSTATE
#define AUTOSTATE

#include <Commands/Command.h>

class AutonomousState
{
public:
  virtual ~AutonomousState(frc::Command* _command, AutonomousState* nextstate);
  virtual AutonomousState update();
  virtual void InitState();
  virtual void StopState();
private:
  frc::Command* command;
  AutonomousState* nextState;
};

#endif
