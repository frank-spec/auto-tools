#include "TestWaters05su.h"

int l = 128;

void Waters05::keygen(CharmList & pk, CharmList & sk, CharmList & chpk)
{
    ZR alpha;
    G1 g;
    G1 g1;
    G1 g2;
    G1 ut;
    CharmListZR y;
    CharmListG1 u;
    ZR chK;
    ZR cht;
    G1 ch0;
    G1 ch1;
    G1 sk0;
    alpha = group.random(ZR_t);
    g = group.random(G1_t);
    g1 = group.exp(g, alpha);
    g2 = group.random(G1_t);
    ut = group.random(G1_t);
    for (int i = 0; i < l; i++)
    {
        y[i] = group.random(ZR_t);
        u[i] = group.exp(g, y[i]);
    }
    chK = group.random(ZR_t);
    cht = group.random(ZR_t);
    ch0 = group.random(G1_t);
    ch1 = group.exp(ch0, cht);
    chpk.insert(0, ch0);
    chpk.insert(1, ch1);
    sk0 = group.exp(g2, alpha);
    sk.insert(0, sk0);
    sk.insert(1, chK);
    sk.insert(2, cht);
    pk.insert(0, chK);
    pk.insert(1, g);
    pk.insert(2, g1);
    pk.insert(3, g2);
    pk.insert(4, u);
    pk.insert(5, ut);
    return;
}

void Waters05::sign(CharmList & chpk, CharmList & pk, CharmList & sk, ZR & M, CharmList & sig)
{
    G1 sk0;
    ZR chK;
    ZR cht;
//    ZR chK;
    G1 g;
    G1 g1;
    G1 g2;
    CharmListG1 u;
    G1 ut;
    ZR r;
    G1 S2;
    ZR s0;
    ZR s1;
    ZR Mpr;
    CharmListZR m;
    G1 dotProd1 = group.init(G1_t, 1);
    G1 S1;
    
    sk0 = sk[0].getG1();
    chK = sk[1].getZR();
    cht = sk[2].getZR();
    
    chK = pk[0].getZR();
    g = pk[1].getG1();
    g1 = pk[2].getG1();
    g2 = pk[3].getG1();
    u = pk[4].getListG1();
    ut = pk[5].getG1();
    r = group.random(ZR_t);
    S2 = group.exp(g, r);
    s0 = group.random(ZR_t);
    s1 = group.hashListToZR((Element(chK) + Element(M) + Element(S2)));
    Mpr = chamH(chpk, s1, s0);
    m = intToBits(Mpr, l);
    group.init(dotProd1, 1);
    for (int i = 0; i < l; i++)
    {
        dotProd1 = group.mul(dotProd1, group.exp(u[i], m[i]));
    }
    S1 = group.mul(sk0, group.exp(group.mul(ut, dotProd1), r));
    sig.insert(0, S1);
    sig.insert(1, S2);
    sig.insert(2, s0);
    return;
}

bool Waters05::verify(CharmList & chpk, CharmList & pk, ZR & M, CharmList & sig)
{
    ZR chK;
    G1 g;
    G1 g1;
    G1 g2;
    CharmListG1 u;
    G1 ut;
    G1 S1;
    G1 S2;
    ZR s0;
    ZR s1;
    ZR Mpr;
    CharmListZR m;
    G1 dotProd2 = group.init(G1_t, 1);
    
    chK = pk[0].getZR();
    g = pk[1].getG1();
    g1 = pk[2].getG1();
    g2 = pk[3].getG1();
    u = pk[4].getListG1();
    ut = pk[5].getG1();
    
    S1 = sig[0].getG1();
    S2 = sig[1].getG1();
    s0 = sig[2].getZR();
    s1 = group.hashListToZR((Element(chK) + Element(M) + Element(S2)));
    Mpr = chamH(chpk, s1, s0);
    m = intToBits(Mpr, l);
    group.init(dotProd2, 1);
    for (int i = 0; i < l; i++)
    {
        dotProd2 = group.mul(dotProd2, group.exp(u[i], m[i]));
    }
    if ( ( (group.div(group.pair(S1, g), group.pair(S2, group.mul(ut, dotProd2)))) == (group.pair(g1, g2)) ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}

ZR Waters05::chamH(CharmList & chpk, ZR & t0, ZR & t1)
{
    G1 ch0;
    G1 ch1;
    G1 chVal;
    ZR chZr;
    
    ch0 = chpk[0].getG1();
    ch1 = chpk[1].getG1();
    chVal = group.mul(group.exp(ch0, t0), group.exp(ch1, t1));
    chZr = group.hashListToZR((Element(1) + Element(chVal)));
    return chZr;
}

