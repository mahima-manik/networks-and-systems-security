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

gmt_server_ip = ""
get_server_port = 65140

key = RSA.generate(1024, os.urandom)    
pubkey = key.publickey()
str_pubkey = pubkey.exportKey("DER")
master_key_pkda = "masterpk"
master_key_c1 = "masterc1"
master_key_c2 = "masterc2"

def tcpservercode():
        global str_pubkey
    	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((gmt_server_ip, get_server_port))
        s.listen(5)
        print '******--KDC Service started--******'
        clientsock = None
        
        while True:
            clientsock, addr = s.accept()
            clientsock.send(str_pubkey)
            client_id = clientsock.recv(1024)
            
            if clientsock != None:
                #info_list = []
                print "Cient request received, ",
                des = DES.new('masterpk', DES.MODE_ECB)
                #info_list.append(signed_doc)
                #info_list.append(gmttime)
                clientsock.send(str(des))
                clientsock.close()
                clientsock = None
                info_list = []
        s.close()

tcpservercode()