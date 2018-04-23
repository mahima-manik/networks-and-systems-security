#KDC

import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.PublicKey.DSA as DSA
import Crypto.PublicKey.ElGamal as ElGamal
import Crypto.Util.number as CUN
from Crypto.Cipher import DES
import ast
import socket, sys, os
import datetime, time

kdc_server_ip = ""
kdc_server_port = 65159

key = RSA.generate(1024, os.urandom)    
pubkey = key.publickey()
str_pubkey = pubkey.exportKey("DER")

master_key_pkda = DES.new('masterpk', DES.MODE_ECB)
print(master_key_pkda, DES.new('masterpk', DES.MODE_ECB))
master_key_c1 = DES.new('mclient1', DES.MODE_ECB)
master_key_c2 = DES.new('mclient2', DES.MODE_ECB)

def tcpservercode():
	global str_pubkey
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind((kdc_server_ip, kdc_server_port))
	s.listen(5)
	print('******--KDC Service started--******')
	clientsock = None

	while True:
		clientsock, addr = s.accept()
		client_id = clientsock.recv(1024)
		print("Server received", client_id)
		if clientsock != None:
			#info_list = []
			print("Cient request received")
			session_key=DES.new(str(os.urandom)[:8], DES.MODE_ECB)
			client_id=ast.literal_eval(client_id)#client_id=[ID PKDA|| ID Client|| t2]
			print("PKDA request received for session key between PKDA and", client_id[1])
			str_session_key = str(str(os.urandom)[:8])
			m=str([str_session_key,client_id[0]])
			while(len(m)%8!=0):
			    m=m+'0'
			print(m)
			if client_id[1]=="client1":
				temp_msg=master_key_c1.encrypt(m)
			elif client_id[1]=="client2":
				temp_msg=master_key_c2.encrypt(m)
			m=str([str_session_key, client_id[0], client_id[1], client_id[2], temp_msg])
			
			while(len(m)%8!=0):
			    m=m+'0'
			print(m)
			
			msg=master_key_pkda.encrypt(m)
			#info_list.append(signed_doc)
			#info_list.append(gmttime)
			print("sending session key to PKDA for", client_id[1])
			clientsock.send(str(msg))
			clientsock.close()
			clientsock = None
			info_list = []
	s.close()

tcpservercode()
