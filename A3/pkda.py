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
kdc_server_port = 65165
pkda_server_ip = "127.0.0.1"
pkda_server_port = 65000

key = RSA.generate(1024, os.urandom)    
pubkey = key.publickey()
str_pubkey = pubkey.exportKey("DER")
master_key = DES.new('masterpk', DES.MODE_ECB)
def tcpclienthandler(serverip , serverport , messgae, flag):
    
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((serverip, serverport))

	if flag == 0:
		s.send(str(messgae))
		return s.recv(64)
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
		clientsock.send(str_pubkey)
		client_id = clientsock.recv(1024)

		if clientsock != None:
			#info_list = []
			print("Cient request received, "),
			client_id=ast.literal_eval(client_id)
			if client_id[1]=="session_key":#client_id=[ID client, session_key, t1]
				t2=time.time()
				msg=str(["PKDA",client_id[0],t2])
				msg1=tcpclienthandler(kdc_server_ip, kdc_server_port, msg, 0)
				msg1=master_key.decrypt(msg1)
				msg1=ast.literal_eval(msg1)#msg1=[session_key, ID PKDA, ID client, t2, to be sent to client]
				sk_dict[msg1[2]]=msg1[0]
				if(msg1[3]==t2):
					clientsock.send(str([msg1[4], client_id[2]]))
					clientsock.close()
					clientsock = None
			if client_id[1]=="public_key":#client_id=[ID CLIENT, public_key, E(Ks, Pu)]
				pk_dict[client_id[0]]=sk_dict[client_id[0]].encrypt(client_id[2])
				
			#info_list.append(signed_doc)
			#info_list.append(kdctime)
			
			info_list = []
	s.close()

tcpservercode()