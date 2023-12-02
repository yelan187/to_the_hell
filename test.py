import socket


def get_all_hosts():
    # 获取本地计算机的IP地址
    host = socket.gethostname()
    # 获取本地计算机的所有IP地址
    hosts = socket.getaddrinfo(host, None)
    # 过滤出IPv4地址
    ipv4_hosts = [host[4][0] for host in hosts if host[0] == socket.AF_INET]
    return ipv4_hosts


# 打印局域网中的所有IP地址
for ip in get_all_hosts():
    print(ip)
