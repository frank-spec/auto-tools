#include "TestBGW.h"

void Bgw05::setup(int n, CharmList & pk, CharmList & msk)
{
    G1 g = group.init(G1_t);
    ZR alpha = group.init(ZR_t);
    int endIndexOfList = 0;
    CharmListG1 giValues;
    ZR gamma = group.init(ZR_t);
    G1 v = group.init(G1_t);
    g = group.random(G1_t);
    alpha = group.random(ZR_t);
    endIndexOfList = group.add((2 * n), 1);
    for (int i = 1; i < endIndexOfList; i++)
    {
        giValues.insert(i, group.exp(g, group.exp(alpha, i)));
    }
    gamma = group.random(ZR_t);
    v = group.exp(g, gamma);
    pk.insert(0, g);
    pk.insert(1, giValues);
    pk.insert(2, v);
    msk.insert(0, gamma);
    return;
}

void Bgw05::keygen(CharmList & pk, CharmList & msk, int n, CharmMetaListG1 & skComplete)
{
    G1 g;
    CharmListG1 giValues;
    G1 v;
    ZR gamma;
    CharmListG1 sk;
    
    g = pk[0].getG1();
    giValues = pk[1].getListG1();
    v = pk[2].getG1();
    
    gamma = msk[0].getZR();
    for (int i = 1; i < n+1; i++)
    {
        sk.insert(i, group.exp(giValues[i], gamma));
    }
    skComplete.insert(0, sk);
    return;
}

void Bgw05::encrypt(CharmListInt & S, CharmList & pk, int n, CharmList & ct)
{
    G1 g;
    CharmListG1 giValues;
    G1 v;
    ZR t = group.init(ZR_t);
    GT K = group.init(GT_t);
    G1 dotProdEncrypt = group.init(G1_t, 1);
    G1 Hdr2 = group.init(G1_t);
    G1 Hdr1 = group.init(G1_t);
    CharmList Hdr;
    
    g = pk[0].getG1();
    giValues = pk[1].getListG1();
    v = pk[2].getG1();
    t = group.random(ZR_t);
    K = group.exp(group.pair(giValues[n], giValues[1]), t);
    group.init(dotProdEncrypt, 1);
    CharmListInt S_keys = S; //.keys();
    int S_len = S_keys.length();
    for (int jEncrypt_var = 0; jEncrypt_var < S_len; jEncrypt_var++)
    {
        int jEncrypt = S_keys[jEncrypt_var];
        dotProdEncrypt = group.mul(dotProdEncrypt, giValues[n+1-jEncrypt]);
    }
    Hdr2 = group.exp(group.mul(v, dotProdEncrypt), t);
    Hdr1 = group.exp(g, t);
    Hdr.insert(0, Hdr1);
    Hdr.insert(1, Hdr2);
    ct.insert(0, Hdr);
    ct.insert(1, K);
    return;
}

void Bgw05::decrypt(CharmListInt & S, int i, int n, CharmList & Hdr, CharmList & pk, CharmMetaListG1 & skComplete, GT & KDecrypt)
{
    G1 Hdr1;
    G1 Hdr2;
    G1 g;
    CharmListG1 giValues;
    G1 v;
    CharmListG1 sk;
    GT numerator = group.init(GT_t);
    G1 dotProdDecrypt = group.init(G1_t, 1);
    int lenS = 0;
    int jDecrypt = 0;
    GT denominator = group.init(GT_t);
    
    Hdr1 = Hdr[0].getG1();
    Hdr2 = Hdr[1].getG1();
    
    g = pk[0].getG1();
    giValues = pk[1].getListG1();
    v = pk[2].getG1();
    
    sk = skComplete[0];
    numerator = group.pair(giValues[i], Hdr2);
    group.init(dotProdDecrypt, 1);
    lenS = S.length();
    for (int decryptLoopVar = 0; decryptLoopVar < lenS; decryptLoopVar++)
    {
        jDecrypt = S[decryptLoopVar];
        if ( ( (jDecrypt) != (i) ) )
        {
            dotProdDecrypt = group.mul(dotProdDecrypt, giValues[n+1-jDecrypt+i]);
        }
    }
    denominator = group.pair(group.mul(sk[i], dotProdDecrypt), Hdr1);
    KDecrypt = group.div(numerator, denominator);
    return;
}

