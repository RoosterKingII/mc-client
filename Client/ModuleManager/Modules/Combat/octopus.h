#include "../Module.h"

class octopus : public Module {
private:
	int ff = 0;
	int ffdl = 0;
	int ternary = false;
	int ternary1 = false;
	bool autos = false;
	bool ss = false;
	bool swap = false;
	bool ownage = false;
	bool ternaryO = false;
	bool toggler = false;

public:
	void Switcher();

	virtual void onAttack(Actor* attackedEnt);
	virtual void onNormalTick(Actor* actor);
	virtual void onEnable();
};
