#include "AutonState.h"

class BasicState : public AutonomousState
{
public:
  BasicState(frc::Command* _command, AutonomousState* nextstate){
	  command = _command;
	  nextState = nextstate;
  }

  virtual AutonomousState* update() {
	  if (command->IsFinished()){
		  StopState();
		  nextState->InitState();
		  return nextState;
	  }
	  return this;
  }

  virtual void InitState(){
   	 command->Start();
  }

  virtual void StopState(){
   	 command->Cancel();
  }

private:
  frc::Command* command;
  AutonomousState* nextState;
};
