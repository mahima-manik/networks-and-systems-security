import socket, sys
import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.PublicKey.DSA as DSA
import Crypto.PublicKey.ElGamal as ElGamal
import Crypto.Util.number as CUN
from Crypto.Cipher import DES
import ast, time, os


pkda_server_ip = "127.0.0.1"
pkda_server_port = 65002

client2_server_ip=""
client2_server_port = 65135

key = RSA.generate(1024, os.urandom)    
public_key = key.publickey()
master_key = DES.new('mclient1', DES.MODE_ECB)
h=MD5.new('09876') 
'''
flag = 0 if we want to request PKDA for session key
flag = 1 if we want to send public key to PKDA
flag = 2 if we want public key of client 2 from PKDA
flag = 3 if we send initial msg to client 2
flag = 4 if we send ack to client 2
'''
def tcpclienthandler(serverip , serverport , message, flag):
    
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((serverip, serverport))
	if flag == 0:
		s.send(message)
		return s.recv(1024)
	if flag==1:
		s.send(message)
		return s.recv(1024)
	if flag==2:
		s.send(message)
		return s.recv(1024)
	if flag==3:
		s.send(message)
		print("message sent")
		return s.recv(1024)
	if flag==4:
		s.send(message)
		return s.recv(1024)
	s.close()

t1=time.time()
msg=str(["client1","session_key",t1])
print("sending request for session_key to PKDA")
msg1 = tcpclienthandler(pkda_server_ip, pkda_server_port, msg, 0)#msg1 to PKDA
msg1=ast.literal_eval(msg1)#msg1=[E(session key, ID PKDA), T1]
if msg1[1]==t1:
	txt = master_key.decrypt(msg1[0])#decrypting with master key txt=[session key, ID PKDA]
	while(txt[-1]=='0'):
	    txt=txt[:-1]
	txt=ast.literal_eval(txt)
	session_key=DES.new(txt[0], DES.MODE_ECB)
	print("received session_key between PKDA and client 1")
m=public_key.exportKey('DER')
#m=str([m])
print(m)
i=0
while(len(m)%8!=0):
    i+=1
    m=m+'0'
msg2=str(["client1", "public_key", session_key.encrypt(m), i])
print("sending public key to PKDA")
pkda_public_key=tcpclienthandler(pkda_server_ip, pkda_server_port, msg2, 1)# [sk_dict[client_id[0]].encrypt(tm), i] 
pkda_public_key = ast.literal_eval(pkda_public_key)
pk_pub = session_key.decrypt(pkda_public_key[0])
pk_pub = pk_pub[:-int(pkda_public_key[1])]
pkda_public_key=RSA.importKey(pk_pub)
print("Received PKDA's public key")

raw_input("Press any key to continue...")


t3=time.time()
msg3=str(["client1", "request", "client2", t3])#client 1 is requesting PKDA for client2's PubKey
print("requesting PKDA for client2 pubkey")
msg4 = tcpclienthandler(pkda_server_ip, pkda_server_port, msg3, 2)#msg3 to PKDA
msg4 = ast.literal_eval(msg4)#[sign, original]
print(type(msg4[0]), type(msg4[1]), type(pkda_public_key))
msg4[1] = ast.literal_eval(msg4[1])#[client2 public key, "client 1", "request", "client2", t3]
print(msg4[0], type(msg4[0]))
msg4[0] = ast.literal_eval(msg4[0])
print(msg4[0], type(msg4[0]))
#msg4[0] = ast.literal_eval(msg4[0])
print(msg4[0][0], type(msg4[0][0]))

h.update(msg4[1][0])
#print("hash: ", h.hexdigest())
b = pkda_public_key.verify(h.hexdigest(), msg4[0][0])
print(b)

if( b ):
    if msg4[1][4]==t3:
	    client2_public_key=RSA.importKey(msg4[1][0])
	    print("received public key of client2")
t4=time.time()
msg5=str(["client1", t4])
print("sending initial msg to client2", t4)
msg6 = tcpclienthandler(client2_server_ip, client2_server_port, str(client2_public_key.encrypt(msg5, '')), 3)#msg6 to client2
msg6=ast.literal_eval(msg6)
msg6 = key.decrypt(msg6)#msg6=[t4, t6]
msg6 = ast.literal_eval(msg6)
print(msg6[0], t4, type(msg6[0]), type(t4), msg6[1])
if msg6[0]==t4:
	print("sending ack to client2")
	msg7 = tcpclienthandler(client2_server_ip, client2_server_port, str(client2_public_key.encrypt(str(msg6[1]), '')), 4)#msg7 to client2
	print("Recieved from client 2: ", key.decrypt(ast.literal_eval(msg7)))
