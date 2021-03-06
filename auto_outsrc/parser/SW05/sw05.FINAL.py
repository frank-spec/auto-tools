from userFuncs import *

from charm.toolbox.pairinggroup import PairingGroup,ZR,G1,G2,GT,pair
from charm.core.engine.util import *
from charm.core.math.integer import randomBits

group = None

N = 2

secparam = 80


def setup(n, d):
    t = {}

    input = [n, d]
    g = group.random(G1)
    y = group.random(ZR)
    g1 = (g ** y)
    g2 = group.random(G2)
    for i in range(0, n+1):
        t[i] = group.random(G2)
    dummyVar = group.random(ZR)
    pk = [g, g1, g2, t]
    mk = [y, dummyVar]
    output = (pk, mk)
    return output

def evalT(pk, n, x):
    Nint = {}
    N = {}

    input = [pk, n, x]
    t = pk[3]
    for i in range(0, n+1):
        N[i] = group.init(ZR, (i+1) )
        Nint[i] = (i + 1)
    coeffs = recoverCoefficientsDict(N)
    prodResult = group.init(G2)
    lenNint = len(Nint)
    for i in range(0, lenNint):
        loopVar = Nint[i]
        j = group.init(ZR, (loopVar) )
        loopVarMinusOne = (loopVar - 1)
        prodResult = (prodResult * (t[loopVarMinusOne] ** coeffs[j]))
    T = ((pk[2] ** (x * n)) * prodResult)
    output = T
    return output

def intersectionSubset(w, wPrime, d):
    SSub = {}
    S = {}

    input = [w, wPrime, d]
    wLen = len(w)
    wPrimeLen = len(wPrime)
    SIndex = 0
    for i in range(0, wLen):
        for j in range(0, wPrimeLen):
            if ( ( (w[i]) == (wPrime[j]) ) ):
                S[SIndex] = w[i]
                SIndex = (SIndex + 1)
    for k in range(0, d):
        SSub[k] = S[k]
    output = SSub
    return output

def extract(mk, ID, pk, dOver, n):
    DBlinded = {}
    wHash = {}
    blindingFactorDBlinded = {}
    q = {}
    D = {}
    d = {}

    input = [mk, ID, pk, dOver, n]
    blindingFactor0Blinded = group.random(ZR)
    zz = group.random(ZR)
    lenID = len(ID)
    for i in range(0, lenID):
        loopVar = ID[i]
        wHash[i] = group.hash(loopVar, ZR)
    wHashBlinded = wHash
    r = group.random(ZR)
    for i in range(0, dOver):
        q[i] = group.random(ZR)
    q[0] = mk[0]
    shares = genShares(mk[0], dOver, n, q, wHash)
    wHashLen = len(wHash)
    for i in range(0, wHashLen):
        loopVar = wHash[i]
        evalTVar = evalT(pk, n, loopVar)
        D[loopVar] = ((pk[2] ** shares[i][1]) * (evalTVar ** r))
        d[loopVar] = (pk[0] ** r)
    dBlinded = d
    yLength = len(D)
    DKeysSuffix = list(D.keys())
    for y in range(0, yLength):
        DKeyLoopVar = DKeysSuffix[y]
        blindingFactorDBlinded[y] = blindingFactor0Blinded
        DBlinded[DKeyLoopVar] = (D[DKeyLoopVar] ** (1 / blindingFactorDBlinded[y]))
    sk = [wHashBlinded, DBlinded, dBlinded]
    skBlinded = [wHashBlinded, DBlinded, dBlinded]
    output = (blindingFactor0Blinded, blindingFactorDBlinded, skBlinded)
    return output

def encrypt(pk, wPrime, M, n):
    E = {}
    wPrimeHash = {}

    input = [pk, wPrime, M, n]
    wPrimeLen = len(wPrime)
    for i in range(0, wPrimeLen):
        loopVar = wPrime[i]
        wPrimeHash[i] = group.hash(loopVar, ZR)
    s = group.random(ZR)
    Eprime = (M * (pair(pk[1], pk[2]) ** s))
    Eprimeprime = (pk[0] ** s)
    wPrimeHashLen = len(wPrimeHash)
    for i in range(0, wPrimeHashLen):
        loopVar = wPrimeHash[i]
        evalTVar = evalT(pk, n, loopVar)
        E[loopVar] = (evalTVar ** s)
    CT = [wPrimeHash, Eprime, Eprimeprime, E]
    output = CT
    return output

def transform(pk, sk, CT, dInputParam):
    transformOutputList = {}

    input = [pk, sk, CT, dInputParam]
    wPrimeHash, Eprime, Eprimeprime, E = CT
    wHash, D, d = sk
    transformOutputList[0] = intersectionSubset(wHash, wPrimeHash, dInputParam)
    S = transformOutputList[0]
    transformOutputList[1] = recoverCoefficientsDict(S)
    coeffs = transformOutputList[1]
    transformOutputList[2] = len(S)
    SLen = transformOutputList[2]
    for i in range(0, SLen):
        transformOutputList[1000+5*i] = S[i]
        loopVar = transformOutputList[1000+5*i]
        transformOutputList[1001+5*i] = pair((d[loopVar] ** coeffs[loopVar]), E[loopVar])
        loopPairings = transformOutputList[1001+5*i]
        transformOutputList[1002+5*i] = pair((Eprimeprime ** -coeffs[loopVar]), D[loopVar])
        loopPairings = transformOutputList[1002+5*i]
    output = transformOutputList
    return output

def decout(pk, sk, CT, dInputParam, transformOutputList, blindingFactor0Blinded, blindingFactorDBlinded):
    input = [pk, sk, CT, dInputParam, transformOutputList, blindingFactor0Blinded, blindingFactorDBlinded]
    wPrimeHash, Eprime, Eprimeprime, E = CT
    wHash, D, d = sk
    S = transformOutputList[0]
    coeffs = transformOutputList[1]
    prod = group.init(GT)
    SLen = transformOutputList[2]
    for i in range(0, SLen):
        loopVar = transformOutputList[1000+5*i]
        loopPairings = (transformOutputList[1001+5*i] * (transformOutputList[1002+5*i] ** blindingFactorDBlinded[i]))
        prod = (prod * loopPairings)
    M = (Eprime * prod)
    output = M
    return output

def SmallExp(bits=80):
    return group.init(ZR, randomBits(bits))

def main():
    global group
    group = PairingGroup(secparam)

    max_attributes = 6
    required_overlap = 4
    (master_public_key, master_key) = setup(max_attributes, required_overlap)
    private_identity = ['insurance', 'id=2345', 'oncology', 'doctor', 'nurse', 'JHU'] #private identity
    public_identity = ['insurance', 'id=2345', 'doctor', 'oncology', 'JHU', 'billing', 'misc'] #public identity for encrypt
    (blindingFactor0Blinded, blindingFactorDBlinded, skBlinded) = extract(master_key, private_identity, master_public_key, required_overlap, max_attributes)
    msg = group.random(GT)
    cipher_text = encrypt(master_public_key, public_identity, msg, max_attributes)
    #decrypted_msg = decrypt(master_public_key, secret_key, cipher_text, required_overlap)
    transformOutputList = transform(master_public_key, skBlinded, cipher_text, required_overlap)
    decrypted_msg = decout(master_public_key, skBlinded, cipher_text, required_overlap, transformOutputList, blindingFactor0Blinded, blindingFactorDBlinded)

    print("msg:  ", msg)
    print("decrypted_msg:  ", decrypted_msg)
    assert msg == decrypted_msg, "failed decryption!"
    print("Successful Decryption!")


if __name__ == '__main__':
    main()

