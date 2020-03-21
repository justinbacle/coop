import socket

TCP_PORT = 40000
TCP_IP = '192.168.1.100'
BUFFER_SIZE = 32

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.bind(("", TCP_PORT))
sock.listen()

conn, addr = sock.accept()
print(f"Connection address: {addr}")
while 1:
    data = conn.recv(BUFFER_SIZE)
    if data:
        print(f"received data: {data}")
        conn.sendall(data)  # echo
    else:
        break
conn.close()
