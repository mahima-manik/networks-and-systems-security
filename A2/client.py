import socket
import Crypto.Hash.MD5 as MD5
import Crypto.PublicKey.RSA as RSA
import Crypto.PublicKey.DSA as DSA
import Crypto.PublicKey.ElGamal as ElGamal
import Crypto.Util.number as CUN
from Crypto.Cipher import DES
import ast


gmt_server_ip = ""
gmt_server_port = 65141
client_port = 65140
des = DES.new('01234567', DES.MODE_ECB)

'''
flag = 0 if we want to connect to GMT server
flag = 1 if we want to send doc to amother client 
'''
def tcpclienthandler(serverip , serverport , hashed_doc, flag):
    
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((serverip, serverport))
    
    if flag == 0:
        s.send(hashed_doc) 				#sending the amount to receiver/witness
        gmt_recvd_msg = None
        while (gmt_recvd_msg == None):
            gmt_recvd_msg = s.recv(1024)
        print "Message received from the server: ", gmt_recvd_msg
        gmt_recvd_msg = ast.literal_eval(gmt_recvd_msg)
        
        signed_doc = gmt_recvd_msg[0]
        gmt_time = gmt_recvd_msg[1]
        gmt_public_key = gmt_recvd_msg[2]
        return (signed_doc, gmt_time, gmt_public_key)
    elif flag == 1:
        s.send(str(info_list[0]))
        s.recv(10)
        s.send(info_list[1])
        print "Verified"
    
    s.close()


text_doc = "I am the Document"
hd=MD5.new(text_doc).digest()
(signed_doc, gmt_time, gmt_public_key) = tcpclienthandler(gmt_server_ip, gmt_server_port, hd, 0)

text = str ([text_doc, gmt_time, signed_doc]) #document, time, sign
if ( len(text)%8 != 0 ):	#padding
    while( len(text) % 8 != 0):
        text += '0'

cipher_text = des.encrypt(text)
info_list = [cipher_text, gmt_public_key]
tcpclienthandler(gmt_server_ip, client_port, info_list, 1)