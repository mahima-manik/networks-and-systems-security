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
des = DES.new('01234567', DES.MODE_ECB)

def tcpservercode():
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((gmt_server_ip, get_server_port))
        s.listen(5)
        print 'Client\'s Server started for'
        clientsock = None
        while True:
            clientsock, addr = s.accept()
            cipher = clientsock.recv(1024)
            clientsock.send("OK")
            pubkey = clientsock.recv(1024)
            #print "Received from server", cipher
            pubkey = RSA.importKey(pubkey)
            
            ''' When received a doc to time stamp '''
            if clientsock:
                txt = des.decrypt(cipher)
                while (txt[len(txt)-1] == '0'): #remove padding
                    txt = txt[:len(txt)-1]
                #print(txt, "client2")
                txt= ast.literal_eval(txt)

                dh = MD5.new(txt[0]).digest() #hash of document
                block_size = 4096
                print txt[1]
                ch = dh + txt[1] 
                hash = MD5.new(ch).digest() #hash of hashed document and time
                #print(txt)
                sign = ast.literal_eval(txt[2])
                a = pubkey.verify(hash, sign)	#verify if same
                print a
                if a == True:
                    print ("Document Verified")
                    clientsock.send("Document Verified")
                clientsock.close()
                clientsock = None
                
        s.close()

tcpservercode()