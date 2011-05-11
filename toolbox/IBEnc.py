# Base class for identity-based encryption
# 
# Notes: This class implements an interface for a standard identity-based encryption scheme.
#        Identity-based encryption consists of three algorithms: (setup, extract, encrypt, and decrypt).
#
from toolbox.schemebase import *

class IBEnc(SchemeBase):
    def __init__(self):
        SchemeBase.__init__(self)
        SchemeBase.setProperty(self, scheme='IBEnc')
        self.baseSecDefs = Enum('IND_sID_CPA', 'IND_ID_CPA', 'IND_ID_CCA1', 'IND_ID_CCA2')
    
    def setup(self):
        raise NotImplementedError
    
    def extract(self, mk, ID):
        raise NotImplementedError
    
    def encrypt(self, pk, ID, message):
        raise NotImplementedError
    
    def decrypt(self, pk, sk, ct):
        raise NotImplementedError
        
