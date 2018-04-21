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
kdc_server_port = 65165

key = RSA.generate(1024, os.urandom)    
pubkey = key.publickey()
str_pubkey = pubkey.exportKey("DER")

master_key_pkda = DES.new('masterpk', DES.MODE_ECB)
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
		clientsock.send(str_pubkey)
		client_id = clientsock.recv(1024)

		if clientsock != None:
			#info_list = []
			print("Cient request received, "),
			session_key=random.random()
			client_id=ast.literal_eval(client_id)#client_id=[ID PKDA|| ID Client|| t2]
			if client_id=="client1":
				temp_msg=master_key_c1.encrypt(str([session_key,client_id[0]]))
			elif client_id=="client2":
				temp_msg=master_key_c2.encrypt(str([session_key,client_id[0]]))
			msg=master_key_pkda.encrypt(str([session_key, client_id[0], client_id[1], client_id[2], temp_msg]))
			#info_list.append(signed_doc)
			#info_list.append(gmttime)
			clientsock.send(str(des))
			clientsock.close()
			clientsock = None
			info_list = []
	s.close()

tcpservercode()