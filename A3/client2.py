import socket, sys
import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.PublicKey.DSA as DSA
import Crypto.PublicKey.ElGamal as ElGamal
import Crypto.Util.number as CUN
from Crypto.Cipher import DES
import ast, os, time


pkda_server_ip = "127.0.0.1"
pkda_server_port = 65002
client_ip=""
client_port = 65135
master_key = DES.new('mclient2', DES.MODE_ECB)

key = RSA.generate(1024, os.urandom)    
public_key = key.publickey()
h=MD5.new('09876')

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
        return s.recv(1024)
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
	
	t6=0
	while True:
		print("client sock:", clientsock)
		clientsock, addr = s.accept()
		print("here", clientsock, addr, clientsock != None)
		if (clientsock != None):
			print("here1")
			client_id = clientsock.recv(1024)
			print("Cient request received, "),
			client_id=ast.literal_eval(client_id)
			t5=time.time()
			client_id=key.decrypt(client_id)
			client_id=ast.literal_eval(client_id)
			print("HERE----------", client_id, type(client_id))
			
			
			if(type(client_id)==list and len(client_id)==2):#client_id=[ID client, t4]
				
				msg3=str(["client2", "request", client_id[0], t5])
				print("requesting PKDA for client 1 pubkKey")
				msg4 = tcpclienthandler(pkda_server_ip, pkda_server_port, msg3, 2)#msg3 to PKDA
				msg4 = ast.literal_eval(msg4)#[sign, original]
				msg4[1] = ast.literal_eval(msg4[1])#[client2 public key, "client 1", "request", "client2", t3]
				msg4[0] = ast.literal_eval(msg4[0])
				h.update(msg4[1][0])
				print(h.hexdigest())
				b = pkda_public_key.verify(h.hexdigest(), msg4[0][0])
				print(b)
				if( b ):
				    if msg4[1][4]==t5:
				        client1_public_key=RSA.importKey(msg4[1][0])
				        print("received public key of client1")
				t6=time.time()
				t6=str(t6)
				print(t6)
				msg5=str([client_id[1], t6])
				msg6=client1_public_key.encrypt(msg5, '')
				print("sending ack to client1")
				clientsock.send(str(msg6))
				clientsock.close()
				clientsock = None
			if(type(client_id)==float):#client_id=[t6]
			    print("inside", client_id, t6)
			    print(t6, str(client_id)==str(t6), str(client_id), str(t6))
			    if(str(client_id)==str(t6)):
			        print("sending hello to client1")
			        clientsock.send(str(client1_public_key.encrypt("hello client1", '')))
			        clientsock.close()
			        clientsock = None
		clientsock = None
		print("next while loop")
        
        '''
		    client_id = clientsock.recv(1024)
			print("Cient request received, "),
			client_id=ast.literal_eval(client_id)
			t6=time.time()
			client_id=key.decrypt(client_id)
			client_id=ast.literal_eval(client_id)
			print("HERE----------", client_id, type(client_id))
			if(len(client_id)==2):#client_id=[ID client, t4]
				t5=time.time()
				msg3=str(["client2", "request", client_id[0], t5])
				print("requesting PKDA for client 1 pubkKey")
				msg4 = tcpclienthandler(pkda_server_ip, pkda_server_port, msg3, 2)#msg3 to PKDA
				msg4 = ast.literal_eval(msg4)#[sign, original]
				msg4[1] = ast.literal_eval(msg4[1])#[client2 public key, "client 1", "request", "client2", t3]
				msg4[0] = ast.literal_eval(msg4[0])
				h.update(msg4[1][0])
				b = pkda_public_key.verify(h.hexdigest(), msg4[0][0])
				if( b ):
				    if msg4[1][4]==t5:
				        client1_public_key=RSA.importKey(msg4[1][0])
				        print("received public key of client1")
				msg5=str([t4, t6])
				msg6=client1_public_key.encrypt(msg5, '')
				print("sending ack to client1")
				clientsock.send(msg6)
				clientsock.close()
				clientsock = None
			if(len(client_id)==1):#client_id=[t6]
			    print("inside", client_id[0], t6)
			    if(client_id[0]==t6):
			        print("sending hello to client1")
			        clientsock.send(client1_public_key.encrypt("hello client1", ''))
			        clientsock.close()
			        clientsock = None'''
        
	s.close()
	
	
t1=time.time()
msg=str(["client2","session_key",t1])
print("sending request for session_key to PKDA")
msg1 = tcpclienthandler(pkda_server_ip, pkda_server_port, msg, 0)#msg1 to PKDA
msg1=ast.literal_eval(msg1)#msg1=[E(session key, ID PKDA), T1]
if msg1[1]==t1:
	txt = master_key.decrypt(msg1[0])#decrypting with master key txt=[session key, ID PKDA]
	while(txt[-1]=='0'):
	    txt=txt[:-1]
	txt=ast.literal_eval(txt)
	session_key=DES.new(txt[0], DES.MODE_ECB)
	print("received session_key between PKDA and client 2")
m=public_key.exportKey('DER')
#m=str([m])
print(m)
i=0
while(len(m)%8!=0):
    i+=1
    m=m+'0'
msg2=str(["client2", "public_key", session_key.encrypt(m), i])
print("sending public key to PKDA")
pkda_public_key=tcpclienthandler(pkda_server_ip, pkda_server_port, msg2, 1)# [sk_dict[client_id[0]].encrypt(tm), i] 
pkda_public_key = ast.literal_eval(pkda_public_key)
pk_pub = session_key.decrypt(pkda_public_key[0])
pk_pub = pk_pub[:-int(pkda_public_key[1])]
pkda_public_key=RSA.importKey(pk_pub)
print("Received PKDA's public key")

tcpservercode()
