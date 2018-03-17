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
get_server_port = 65141

key = RSA.generate(1024, os.urandom)    
pubkey = key.publickey()

def time_stamp_doc(hashed_doc):
    global key
    st = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S')

    hash_ds = MD5.new(hashed_doc+st).digest()

	# sign the hash
    K = ''
    signature = key.sign(hash_ds, K)
    print "Sending to client ", st

    return str(signature), st

def tcpservercode():
        global pubkey
    	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((gmt_server_ip, get_server_port))
        s.listen(5)
        print 'GMT Date/Time Server started for'
        clientsock = None
        while True:
            clientsock, addr = s.accept()
            hashed_doc = clientsock.recv(1024)
            
            ''' When received a doc to time stamp '''
            if clientsock:
                info_list = []
                print "Cient request received", 
                signed_doc, gmttime = time_stamp_doc(hashed_doc)
                info_list.append(signed_doc)
                info_list.append(gmttime)
                print info_list
                str_pubkey = pubkey.exportKey("DER")
                info_list.append(str_pubkey)
                clientsock.send(str(info_list))
                clientsock.close()
                clientsock = None
                info_list = []
        s.close()

tcpservercode()