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

def time_stamp_doc(hashed_doc):
    global key
    st = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S')

    hash_ds = MD5.new(hashed_doc+st).digest()

	# sign the hash
    K = ''
    signature = key.sign(hash_ds, K)
    print "Timestamp:", st

    return str(signature), st

def tcpservercode():
        global str_pubkey
    	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((gmt_server_ip, get_server_port))
        s.listen(5)
        print '******--GMT Date/Time Server started--******'
        clientsock = None
        
        while True:
            clientsock, addr = s.accept()
            clientsock.send(str_pubkey)
            hashed_doc = clientsock.recv(1024)
            
            if clientsock != None and hashed_doc != "Pub key rcvd":
                info_list = []
                print "Cient request received, ", 
                signed_doc, gmttime = time_stamp_doc(hashed_doc)
                info_list.append(signed_doc)
                info_list.append(gmttime)
                clientsock.send(str(info_list))
                clientsock.close()
                clientsock = None
                info_list = []
        s.close()

tcpservercode()