import socket

hostname = socket.gethostname()
ip = socket.gethostbyname(hostname)

print("Hostname: ", hostname)
print("IP: ", ip)


addrs = socket.getaddrinfo(socket.gethostname(), None)
for item in addrs:
    if ":" not in item[4][0]:
        print("当前主机IPV4地址为:" + item[4][0])
