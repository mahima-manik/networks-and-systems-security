import socket, sys
import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.PublicKey.DSA as DSA
import Crypto.PublicKey.ElGamal as ElGamal
import Crypto.Util.number as CUN
from Crypto.Cipher import DES
import ast, time


pkda_server_ip = "127.0.0.1"
pkda_server_port = 65000

client2_server_ip=""
client2_server_port = 65135

key = RSA.generate(1024, os.urandom)    
public_key = key.publickey()

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
	print(msg, type(msg))
	if flag == 0:
		s.send(message)
		return s.recv(64)
	if flag==1:
		s.send(message)
		return s.recv(1024)
	if flag==2:
		s.send(message)
		return s.recv(1024)
	if flag==3:
		s.send(message)
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
	txt = masterkey.decrypt(msg1[0])#decrypting with master key txt=[session key, ID PKDA]
	session_key=txt[0]
	print("received session_key between PKDA and client 1")
msg2=str(["client1", "public key", session_key.encrypt(public_key)])
print("sending public key to PKDA")
pkda_public_key=tcpclienthandler(pkda_server_ip, pkda_server_port, msg2, 1)
pkda_public_key=session_key.decrypt(pkda_public_key)
print("Received PKDA's public key")

t3=time.time()
msg3=str(["client1", "request", "client2", t3])#client 1 is requesting PKDA for client2's PubKey
print("requesting PKDA for client2 pubkwy")
msg4 = tcpclienthandler(pkda_server_ip, pkda_server_port, msg3, 2)#msg3 to PKDA
msg4 = pkda_public_key.decrypt(msg4)
msg4 = ast.literal_eval(msg4)#[client2 public key, "client 1", "request", "client2", t3]
if msg4[4]==t3:
	client2_public_key=msg4[0]
	print("received public key of client2")
t4=time.time()
msg5=str(["client1", t4])
msg6 = tcpclienthandler(client2_server_ip, client2_server_port, client2_public_key.encrypt(msg5), 3)#msg6 to client2
msg6 = private_key.decrypt(msg6)
msg6 = ast.literal_eval(msg6)#msg6=[t4, t6]
if msg6[0]==t4:
	print("sending ack to client2")
	msg7 = tcpclienthandler(client2_server_ip, client2_server_port, client2_public_key.encrypt(str(msg6[1])), 4)#msg7 to client2
	print("Recieved from client 2: ", private_key.decrypt(msg7))
