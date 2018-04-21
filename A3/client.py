import socket, sys
import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.PublicKey.DSA as DSA
import Crypto.PublicKey.ElGamal as ElGamal
import Crypto.Util.number as CUN
from Crypto.Cipher import DES
import ast


gmt_server_ip = ""
gmt_server_port = 65140
client_port = 65135
des = DES.new('01234567', DES.MODE_ECB)

'''
flag = 0 if we want to connect to GMT server
flag = 1 if we want to send doc to amother client 
'''
def tcpclienthandler(serverip , serverport , hashed_doc, flag):
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((serverip, serverport))
    
    if flag == 0:
        gmt_public_key = s.recv(1024)
        s.send(hashed_doc) 				#sending the amount to receiver/witness
        gmt_recvd_msg = None
        
        while (gmt_recvd_msg == None):
            gmt_recvd_msg = s.recv(1024)
        
        gmt_recvd_msg = ast.literal_eval(gmt_recvd_msg)
        signed_doc = gmt_recvd_msg[0]
        gmt_time = gmt_recvd_msg[1]
        print "Timestamp from the server: ", gmt_time
        
        return (signed_doc, gmt_time, gmt_public_key)
    elif flag == 1:
        s.send(str(hashed_doc))
        print s.recv(64)
        #s.send(hashed_doc[1])
    
    s.close()

filename = sys.argv[1]
text_doc = open(filename).read()

hd=MD5.new(text_doc).digest()
(signed_doc, gmt_time, gmt_public_key) = tcpclienthandler(gmt_server_ip, gmt_server_port, hd, 0)

text = str ([text_doc, gmt_time, signed_doc]) #document, time, sign
if ( len(text)%8 != 0 ):	#padding
    while( len(text) % 8 != 0):
        text += '0'

cipher_text = des.encrypt(text)
tcpclienthandler(gmt_server_ip, client_port, cipher_text, 1)