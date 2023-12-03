import socket
import pygame
pygame.init()
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.settimeout(5)
client.connect(("192.168.137.1", 7890))
print(1)
while True:
    for event in pygame.event.get():
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                client.send(b"left")
            elif event.key == pygame.K_RIGHT:
                client.send(b"right")
            elif event.key == pygame.K_UP:
                client.send(b"up")
            elif event.key == pygame.K_DOWN:
                client.send(b"down")
            print(2)
        elif event.type == pygame.KEYUP:
            if event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT:
                client.send(b"unmove")
                print(3)
