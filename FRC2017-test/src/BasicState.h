#ifndef AUTOSTATE
#define AUTOSTATE

#include <Commands/Command.h>

class BasicState
{
public:
BasicState(frc::Command* _command, BasicState* nextstate){
	command = _command;
	nextState = nextstate;
}

BasicState* update() {
	//if (command->IsFinished()){
		StopState();
		nextState->InitState();
		return nextState;
	//}else{
	return this;
	//}
}

void InitState(){
	command->Start();
}

void StopState(){
	command->Cancel();
}
private:
  frc::Command* command;
  BasicState* nextState;
};

#endif
