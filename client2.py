import socket, sys
import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.PublicKey.DSA as DSA
import Crypto.PublicKey.ElGamal as ElGamal
import Crypto.Util.number as CUN
from Crypto.Cipher import DES
import ast


pkda_server_ip = "127.0.0.1"
pkda_server_port = 65140
client_ip=""
client_port = 65135
masterkey = DES.new('mclient2', DES.MODE_ECB)
privatekey, public_key=0, 1

'''
flag = 0 if we want to request PKDA for session key
flag = 1 if we want to send public key to PKDA
flag = 2 if we request PKDA for client 1 public key
'''
def tcpclienthandler(serverip , serverport , message, flag):
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((serverip, serverport))
    
	if flag == 0:
        s.send(str(message))
        return s.recv(64)
	if flag==1:
		s.send(message)
		return s.recv(1024)
	if flag==2:
		s.send(message)
		return s.recv(1024)
    s.close()

def tcpservercode():
	global str_pubkey
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((client_ip, client_port))
	s.listen(5)
	print('******--client2 Service started--******')
	clientsock = None

	while True:
		clientsock, addr = s.accept()
		client_id = clientsock.recv(1024)

		if clientsock != None:
			#info_list = []
			print("Cient request received, "),
			client_id=ast.literal_eval(client_id)
			if(len(client_id)==2):#client_id=[ID client, t4]
				t5=time.time()
				msg3=str(["client2", "request", client_id[0], t5])
				print("requesting PKDA for client 1 pubkKey")
				msg4=tcpclienthandler(pkda_server_ip, pkda_server_port, msg3, 2)
				msg4 = pkda_public_key.decrypt(msg4)
				msg4 = ast.literal_eval(msg4)#[client1 public key, "client2", "request", "client1", t5]
				if msg4[4]==t5:
					client1_public_key=msg4[0]
					print("received public key of client1")
				t6=time.time()
				msg5=str([t4, t6])
				print("sending ack to client1")
				clientsock.send(msg6)
				clientsock.close()
				clientsock = None
			if(len(client_id)==1):#client_id=[t6]
				if(client_id[0]==t6):
					clientsock.send(client1_public_key.encrypt("hello client1"))
					clientsock.close()
					clientsock = None
			
	s.close()
	
	
	
t1=time.time()
msg=str(["client2"||"session_key"||t1])
print("sending request for session_key to PKDA")
msg1 = tcpclienthandler(pkda_server_ip, pkda_server_port, msg, 0)#msg1 to PKDA
msg1=ast.literal_eval(msg1)#msg1=[E(session key, ID PKDA), T1]
if msg1[1]==t1:
	txt = masterkey.decrypt(msg1[0])#decrypting with master key txt=[session key, ID PKDA]
	session_key=txt[0]
	print("received session_key between PKDA and client 2")
print("sending public key to PKDA")
msg2=str(["client2", "public key", session_key.encrypt(public_key)])
pkda_public_key=tcpclienthandler(pkda_server_ip, pkda_server_port, msg2, 1)
pkda_public_key=session_key.decrypt(pkda_public_key)
print("Received PKDA's public key")


tcpservercode()
