import socket, sys
import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.PublicKey.DSA as DSA
import Crypto.PublicKey.ElGamal as ElGamal
import Crypto.Util.number as CUN
from Crypto.Cipher import DES
import ast


pkda_server_ip = ""
pkda_server_port = 65140
client_port = 65135
masterkey = DES.new('mclient2', DES.MODE_ECB)
privatekey, public_key=0, 1

'''
flag = 0 if we want to request PKDA for session key
flag = 1 if we want to send public key to PKDA
'''
def tcpclienthandler(serverip , serverport , message, flag):
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((serverip, serverport))
    
	if flag == 0:
        s.send(str(message))
        return s.recv(64)
	if flag==1:
		s.send(message) 
    s.close()

t1=time.time()
msg=str(["client2"||"session_key"||t1])
msg1 = tcpclienthandler(pkda_server_ip, pkda_server_port, msg, 0)#msg1 to PKDA
msg1=ast.literal_eval(msg1)#msg1=[E(session key, ID PKDA), T1]
if msg1[1]==t1:
	txt = masterkey.decrypt(msg1[0])#decrypting with master key txt=[session key, ID PKDA]
	session_key=txt[0]
msg2=str(["client2", "public key", session_key.encrypt(public_key)])
tcpclienthandler(pkda_server_ip, pkda_server_port, msg2, 1)

