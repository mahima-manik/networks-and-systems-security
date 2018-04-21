# networks-and-systems-security

FOR A3:
Run KDC.py
Then PKDA.py
Then client.py

This should establish session keys between PKDA and client
Run using python 2

Flow:
Client1 to PKDA: ["Client1", "Session_Key", T1]
PKDA to KDC: ["PKDA", "C1", T2]
KDC to PKDA: E(Mp, [Ks, "PKDA", "Client1", T2, E(Mc, [Ks, "PKDA"])])
PKDA to Client1: [E(Mc, [Ks, "PKDA"])], T1]
Client1 to PKDA: ["Client1", "Public Key", E(Ks, PUc)]

Mp: Master key of PKDA
T1, T2: nonces
Mc: Master key of client
Ks: Session key
PUc: Public key of client


Same thing will be done for establishing session key of client 2
