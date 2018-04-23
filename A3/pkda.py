#PKDA

import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.PublicKey.DSA as DSA
import Crypto.PublicKey.ElGamal as ElGamal
import Crypto.Util.number as CUN
from Crypto.Cipher import DES
import ast
import socket, sys, os
import datetime, time
pk_dict={}
sk_dict={}
kdc_server_ip = ""
kdc_server_port = 65159
pkda_server_ip = "127.0.0.1"
pkda_server_port = 65002

key = RSA.generate(1024, os.urandom)    
pubkey = key.publickey()
str_pubkey = pubkey.exportKey("DER")
master_key = DES.new('masterpk', DES.MODE_ECB)
print(master_key, DES.new('masterpk', DES.MODE_ECB))
def tcpclienthandler(serverip , serverport , messgae, flag):
    
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((serverip, serverport))

	if flag == 0:
		s.send(str(messgae))
		return s.recv(1024)
	if flag==1:
		s.send(messgae) 
	s.close()
def tcpservercode():
	global str_pubkey
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((pkda_server_ip, pkda_server_port))
	s.listen(5)
	print('******--PKDA Service started--******')
	clientsock = None

	while True:
		clientsock, addr = s.accept()
		client_id = clientsock.recv(2048)
		print("Server received", client_id)
		if clientsock != None:
			#info_list = []
			print("Cient request received, "),
			client_id=ast.literal_eval(client_id)
			if client_id[1]=="session_key":#client_id=[ID client, session_key, t1]
				print(client_id[0], "requesting for session key")
				t2=time.time()
				msg=str(["PKDA",client_id[0],t2])
				print("sending request to KDC for session key")
				msg1=tcpclienthandler(kdc_server_ip, kdc_server_port, msg, 0)
				msg1=master_key.decrypt(msg1)
				print(msg1)
				while(msg1[-1]=='0'):
				    msg1=msg1[:-1]
				print(msg1)
				msg1=ast.literal_eval(msg1)#msg1=[session_key, ID PKDA, ID client, t2, to be sent to client]
				sk_dict[msg1[2]]=session_key=DES.new(msg1[0], DES.MODE_ECB)
				print("Received session key from KDC between PKDA and ", client_id[0])
				if(msg1[3]==t2):
					print("Sending session_key to", client_id[0])
					clientsock.send(str([msg1[4], client_id[2]]))
					clientsock.close()
					clientsock = None
			if client_id[1]=="public_key":#client_id=[ID CLIENT, public_key, E(Ks, Pu), no. of 0s]
				print(client_id[0], "sent its public key")
				m=client_id[2]
				m=sk_dict[client_id[0]].decrypt(m)
				#m=ast.literal_eval(m)
				print(m[-1], m[-1]=='0')
				m=m[:-int(client_id[3])]
				#print(ast.literal_eval(m))
				pk_dict[client_id[0]]=RSA.importKey(m)
				print("sending PKDA public key to", client_id[0])
				tm=str(str_pubkey)
				i = 0
				while (len(tm) % 8 != 0):
				    i += 1
				    tm = tm + '0'
				m = str([sk_dict[client_id[0]].encrypt(tm), i] )
				clientsock.send(m)#sending PKDA's public key
				clientsock.close()
				clientsock = None
			if client_id[1]=="request":#client_id=[ID CLIENT, "request", ID CLIENT another, t3]
				
				k = pk_dict[client_id[2]].exportKey('DER')
				h=MD5.new('09876')
				h.update(k)
				print(type(k), k)
				msg2 = h.hexdigest()
				print("hash: ", msg2)
				print ("sending public key of other client")
				m = key.sign(msg2, ' ')
				print(m, type(m))
				m=str([m])
				clientsock.send(str([m, str([k, client_id[0], client_id[1], client_id[2], client_id[3]])]))
				clientsock.close()
				clientsock = None
			info_list = []
	s.close()

tcpservercode()
