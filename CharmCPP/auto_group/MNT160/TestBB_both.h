#ifndef BBIBE04_H
#define BBIBE04_H

#include "Charm.h"
#include <iostream>
#include <sstream>
#include <string>
#include <list>
using namespace std;


class Bbibe04
{
public:
	PairingGroup group;
	Bbibe04() { group.setCurve(MNT160); };
	~Bbibe04() {};
	
	void setup(CharmList & msk, CharmList & pk);
	void keygen(CharmList & pk, CharmList & msk, NO_TYPE & id, CharmList & sk);
	void encrypt(CharmList & pk, GT & M, NO_TYPE & id, CharmList & ct);
	void decrypt(CharmList & pk, CharmList & sk, CharmList & ct, GT & M);
};


#endif
