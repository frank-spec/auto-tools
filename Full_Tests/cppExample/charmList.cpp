#include "sdlconfig.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

ZR & SmallExp(int bits) {
	big t = mirvar(0);
	bigbits(bits, t);

	ZR *z = new ZR(t);
	mr_free(t);
	return *z;
}


void keygen(PairingGroup & group, G2 & pk, ZR & sk, G2 & g) {
    ZR *x = new ZR();
    
    g = group.random(G2_t);
    *x = group.random(ZR_t);
    pk = group.exp(g, *x); // make sure group operations return dynamic memory
    sk = *x;
     
    return;
}

void sign(PairingGroup & group, ZR & sk, string M, G1 & sig) 
{
    G1 h = group.hashListToG1(M);
    sig = group.exp(h, sk); 

    return;
}

void verify(PairingGroup & group, G2 & pk, G2 & g, G1 & sig, string M, bool & output) {
   G1 h = group.hashListToG1(M);
   if(group.pair(sig, g) == group.pair(h, pk)) {
      cout << "Successful Verification" << endl;
      output = true;
   }
   else {
      output = false;
   }
}

int main()
{
	// example for using CharmList* data structures
    PairingGroup group(AES_SECURITY);

    int N = 10;
    CharmListZR sk;
    CharmListG1 g;
    CharmListG2 g2;
    CharmListGT gt;

    for(int z = 0; z < N; z++) {
    	sk[z] = SmallExp(80);
    }

    cout << "list of deltas: " << endl;
    cout << sk << endl;

    for(int z = 0; z < N; z++) {
    	g[z] = group.random(G1_t);
    }

    cout << "list of G1: " << endl;
    cout << g << endl;

    for(int z = 0; z < N; z++) {
    	g2[z] = group.random(G2_t);
    }
    cout << "list of G2: " << endl;
    cout << g2 << endl;

    for(int z = 0; z < N; z++) {
    	gt[z] = group.random(GT_t);
    }

    cout << "list of GT: " << endl;
    cout << gt << endl;


    return 0;
}
