from toolbox.pairinggroup import *
from charm.engine.util import *
import sys, random, string
import sys

group = None
debug = None
bodyKey = 'Body'


def __init__( groupObj ) : 
	global group , debug 
	group= groupObj 
	debug= False 

def verifySigsRecursive(verifyArgsDict, groupObj, incorrectIndices, startSigNum, endSigNum, delta, dotA, dotB):
	z = 0

	group = groupObj


	__init__(group)


	dotA_loopVal = group.init(G1, 1)
	dotB_loopVal = group.init(G1, 1)

	for index in range(startSigNum, endSigNum):
		dotA_loopVal = dotA_loopVal * dotA[index]
		dotB_loopVal = dotB_loopVal * dotB[index]

	if (  pair( dotA_loopVal , verifyArgsDict[z]['pk'][bodyKey] [ 'g^x' ] )== pair( dotB_loopVal , verifyArgsDict[z]['pk'][bodyKey] [ 'g' ] )   ):
		return
	else:
		midWay = int( (endSigNum - startSigNum) / 2)
		if (midWay == 0):
			if startSigNum not in incorrectIndices:
				incorrectIndices.append(startSigNum)
			return
		midSigNum = startSigNum + midWay
		verifySigsRecursive(verifyArgsDict, group, incorrectIndices, startSigNum, midSigNum, delta, dotA, dotB)
		verifySigsRecursive(verifyArgsDict, group, incorrectIndices, midSigNum, endSigNum, delta, dotA, dotB)
