schemeType = "PKENC"
#short = "secret-keys"
short = "ciphertext"
#short = "both" 

setupFuncName = "setup"
keygenFuncName = "keygen"
encryptFuncName = "encrypt"
decryptFuncName = "decrypt"

masterPubVars = ["pk"]
masterSecVars = ["mk"]

keygenPubVar = "pk"
keygenSecVar = "sk"
ciphertextVar = "ct"
