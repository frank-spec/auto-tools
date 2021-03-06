#include "sdlconfig.h"
#include <iostream>
#include <sstream>
#include <string>
#include <list>
using namespace std;

int N = 2;

int secparam = 80;

PairingGroup group(AES_SECURITY);

void setup(G1 & g)
{
    g = group.random(G1_t);
    return;
}

void keygen(G1 & g, G1 & X, G1 & Y, ZR & x, ZR & y)
{
    x = group.random(ZR_t);
    y = group.random(ZR_t);
    X = group.exp(g, x);
    Y = group.exp(g, y);
    return;
}

void sign(ZR & x, ZR & y, string & M, CharmList & sig)
{
    G2 a = group.init(G2_t);
    ZR m = group.init(ZR_t);
    G2 b = group.init(G2_t);
    G2 c = group.init(G2_t);
    a = group.random(G2_t);
    m = group.hashListToZR(M);
    b = group.exp(a, y);
    c = group.exp(a, group.add(x, group.mul(m, group.mul(x, y))));
    sig.append(a);
    sig.append(b);
    sig.append(c);
    return;
}

bool verify(G1 & X, G1 & Y, G1 & g, string & M, G2 & a, G2 & b, G2 & c)
{
    ZR m = group.init(ZR_t);
    m = group.hashListToZR(M);
    if ( ( (( (group.pair(Y, a)) == (group.pair(g, b)) )) && (( (group.mul(group.pair(X, a), group.exp(group.pair(X, b), m))) == (group.pair(g, c)) )) ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool membership(G1 & g, CharmListG2 & alist, CharmListG2 & clist, CharmListG2 & blist, G1 & Y, G1 & X)
{
    if ( ( (group.ismember(g)) == (false) ) )
    {
        return false;
    }
    if ( ( (group.ismember(alist)) == (false) ) )
    {
        return false;
    }
    if ( ( (group.ismember(clist)) == (false) ) )
    {
        return false;
    }
    if ( ( (group.ismember(blist)) == (false) ) )
    {
        return false;
    }
    if ( ( (group.ismember(Y)) == (false) ) )
    {
        return false;
    }
    if ( ( (group.ismember(X)) == (false) ) )
    {
        return false;
    }
    return true;
}

void dividenconquer(CharmListZR & delta1, CharmListZR & delta2, int startSigNum, int endSigNum, list<int> & incorrectIndices, CharmListG2 & dotACache, CharmListG2 & dotBCache, CharmListG2 & dotCCache, G1 & g, G1 & Y, G1 & X)
{
    G2 dotALoopVal = group.init(G2_t, 1);
    G2 dotBLoopVal = group.init(G2_t, 1);
    G2 dotCLoopVal = group.init(G2_t, 1);
    int midwayFloat = 0;
    int midway = 0;
    int midSigNum = 0;
    group.init(dotALoopVal, 1);
    group.init(dotBLoopVal, 1);
    group.init(dotCLoopVal, 1);
    for (int z = startSigNum; z < endSigNum; z++)
    {
        dotALoopVal = group.mul(dotALoopVal, dotACache[z]);
        dotBLoopVal = group.mul(dotBLoopVal, dotBCache[z]);
        dotCLoopVal = group.mul(dotCLoopVal, dotCCache[z]);
    }
    if ( ( (group.mul(group.pair(g, dotALoopVal), group.pair(Y, dotBLoopVal))) == (group.pair(X, dotCLoopVal)) ) )
    {
        return;
    }
    else
    {
        midwayFloat = group.div(group.sub(endSigNum, startSigNum), 2);
        midway = int(midwayFloat);
    }
    if ( ( (midway) == (0) ) )
    {
        incorrectIndices.push_back(startSigNum);
    }
    else
    {
        midSigNum = group.add(startSigNum, midway);
        dividenconquer(delta1, delta2, startSigNum, midSigNum, incorrectIndices, dotACache, dotBCache, dotCCache, g, Y, X);
        dividenconquer(delta1, delta2, midSigNum, endSigNum, incorrectIndices, dotACache, dotBCache, dotCCache, g, Y, X);
    }
    return;
}

bool batchverify(G1 & g, CharmListG2 & alist, CharmListStr & Mlist, CharmListG2 & clist, CharmListG2 & blist, G1 & Y, G1 & X, list<int> & incorrectIndices)
{
    CharmListZR delta1;
    CharmListZR delta2;
    ZR m = group.init(ZR_t);
    CharmListG2 dotACache;
    CharmListG2 dotBCache;
    CharmListG2 dotCCache;
    for (int z = 0; z < N; z++)
    {
        delta1[z] = SmallExp(secparam);
        delta2[z] = SmallExp(secparam);
    }
    if ( ( (membership(g, alist, clist, blist, Y, X)) == (false) ) )
    {
        return false;
    }
    for (int z = 0; z < N; z++)
    {
        m = group.hashListToZR(Mlist[z]);
        dotACache[z] = group.mul(group.exp(blist[z], delta1[z]), group.exp(clist[z], delta2[z]));
        dotBCache[z] = group.exp(alist[z], group.neg(delta1[z]));
        dotCCache[z] = group.mul(group.exp(alist[z], delta2[z]), group.exp(blist[z], group.mul(m, delta2[z])));
    }
    dividenconquer(delta1, delta2, 0, N, incorrectIndices, dotACache, dotBCache, dotCCache, g, Y, X);
    return true;
}

int main()
{
    ZR x, y;
    G1 g, X, Y;
    string m0 = "test message 0.";
    string m1 = "test message 1.";
    CharmList sig0, sig1;

    setup(g);

    keygen(g, X, Y, x, y);

    sign(x, y, m0, sig0);
    sign(x, y, m1, sig1);

    G2 a = sig0[0].getG2();
    G2 b = sig0[1].getG2();
    G2 c = sig0[2].getG2();

    if(verify(X, Y, g, m0, a, b, c)) {
      cout << "Successful Verification!" << endl;
    }
    else {
      cout << "FAILED Verification!" << endl;
    }

    list<int> incorrectIndices;
    CharmListStr Mlist;
    Mlist[0] = m0; // replace with m1 if you want to test invalid sigs
    Mlist[1] = m1;
    CharmListG2 alist, blist, clist;
    alist[0] = sig0[0].getG2();
    alist[1] = sig1[0].getG2();

    blist[0] = sig0[1].getG2();
    blist[1] = sig1[1].getG2();
    clist[0] = sig0[2].getG2();
    clist[1] = sig1[2].getG2();

    batchverify(g, alist, Mlist, clist, blist, Y, X, incorrectIndices);

    cout << "Incorrect indices: ";
    for (list<int>::iterator it = incorrectIndices.begin(); it != incorrectIndices.end(); it++)
	cout << *it << " ";
    cout << endl;

    return 0;
}
