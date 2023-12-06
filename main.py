import pygame
from game import *
from random import choice, randint
from utills import *
from UI import *
import time
from threading import Thread
import socket

# ---------------------------------def consts________________________________________
SCORE = 0
SOLID = 1
FRAGILE = 2
DEADLY = 3
BELT_LEFT = 4
BELT_RIGHT = 5
BODY = 6
SHADOW = 7
BOUNCE = 8

GAME_ROW = 60
GAME_COL = 70
OBS_WIDTH = GAME_COL // 4
SIDE = 13
SCREEN_WIDTH = SIDE * GAME_COL
SCREEN_HEIGHT = SIDE * GAME_ROW
COLOR = {
    SOLID: 0x00FFFF,
    FRAGILE: 0xFF5500,
    DEADLY: 0xFF2222,
    SCORE: 0xCCCCCC,
    BOUNCE: 0xFFA500,
    BELT_LEFT: 0xFFFF44,
    BELT_RIGHT: 0xFF99FF,
}
CHOICE = [SOLID, SOLID, SOLID, FRAGILE, FRAGILE, BELT_LEFT, BELT_RIGHT, DEADLY, BOUNCE]

ROLLING_SPEED = 2
FALLING_SPEED = 3
MOVING_SPEED = 2
BELT_SPEED = 1
ACCELERATION = 0.2
MAX_SPEED = 10
MAX_JUMP_CHANCE = 2

MENU = 4
SERVER_LOBBY = 5
CLIENT_LOBBY = 6
REMOTE = 7
INGAME_SOLO = 8
INGAME_MULTIPLE = 9
ENDPAGE_SOLO = 10
ENDPAGE_MULTIPLE = 11
ERROR = 12


#
# ------------------------------------main game----------------------------------------
class Hell(Game):
    def __init__(self, title, size, fps=60):
        super(Hell, self).__init__(title, size, fps)
        self.currentPage = MENU
        # --------------------------------pages------------------------------
        self.menu_page = form(self.screen)
        label = Label(
            pygame.font.SysFont("arial", 80),
            "title",
            "To The Hell",
            0xB22222,
            (270, 150),
        )
        button1 = Button(
            pygame.font.SysFont("arial", 50), "Solo", "Solo", 0x696969, (390, 300)
        )
        button2 = Button(
            pygame.font.SysFont("arial", 50),
            "Multiple_server",
            "Play as server",
            0x696969,
            (305, 400),
        )
        button3 = Button(
            pygame.font.SysFont("arial", 50),
            "Multiple_client",
            "Play as client",
            0x696969,
            (315, 500),
        )
        button4 = Button(
            pygame.font.SysFont("arial", 50),
            "Submit",
            "Submit",
            0x696969,
            (365, 600),
        )
        button1.select()
        button4.selective = False
        self.menu_page.add([label, button1, button2, button3, button4])

        self.end_page_solo = form(self.screen)
        label1 = Label(
            pygame.font.SysFont("arial", 80),
            "1",
            "Game Over",
            0xB22222,
            (self.screen.get_rect().center[0] - 185, 250),
        )
        label2 = Label(
            pygame.font.SysFont("arial", 50),
            "score",
            "placeHolder",
            0x696969,
            (self.screen.get_rect().center[0] - 185, 350),
        )
        button1 = Button(
            pygame.font.SysFont("arial", 50),
            "restart",
            "click to restart",
            0x696969,
            (self.screen.get_rect().center[0] - 185, 450),
        )
        button2 = Button(
            pygame.font.SysFont("arial", 50),
            "tomenu",
            "back to menu",
            0x696969,
            (self.screen.get_rect().center[0] - 185, 550),
        )
        button1.selective = False
        button2.selective = False
        self.end_page_solo.add(([label1, label2, button1, button2]))

        self.end_page_multiple = form(self.screen)
        label1 = Label(
            pygame.font.SysFont("arial", 80),
            "1",
            "Game Over",
            0xB22222,
            (self.screen.get_rect().center[0] - 185, 250),
        )
        label2 = Label(
            pygame.font.SysFont("arial", 50),
            "score",
            "placeHolder",
            0x696969,
            (self.screen.get_rect().center[0] - 185, 350),
        )
        button1 = Button(
            pygame.font.SysFont("arial", 50),
            "restart",
            "click to restart",
            0x696969,
            (self.screen.get_rect().center[0] - 185, 450),
        )
        button2 = Button(
            pygame.font.SysFont("arial", 50),
            "tomenu",
            "back to menu",
            0x696969,
            (self.screen.get_rect().center[0] - 185, 550),
        )
        button1.selective = False
        button2.selective = False
        self.end_page_multiple.add(([label1, label2, button1, button2]))

        self.server_lobby_page = form(self.screen)
        label1 = Label(
            pygame.font.SysFont("arial", 50),
            "active_connection",
            "placeHolder",
            0x696969,
            (100, 50),
        )
        label2 = Label(
            pygame.font.SysFont("arial", 50),
            "skill",
            "placeHolder",
            0x696969,
            (155, 345),
        )
        button1 = Button(
            pygame.font.SysFont("arial", 50),
            "prepare",
            "prepare",
            0x696969,
            (100, 150),
        )
        button2 = Button(
            pygame.font.SysFont("arial", 50),
            "tomenu",
            "back to menu",
            0x696969,
            (100, 250),
        )
        button3 = Button(
            pygame.font.SysFont("arial", 40),
            "skill_left",
            "<<",
            0xFFFFFF,
            (100, 350),
            0,
            0,
        )
        button4 = Button(
            pygame.font.SysFont("arial", 40),
            "skill_right",
            ">>",
            0xFFFFFF,
            (400, 350),
            0,
            0,
        )
        button1.hide = True
        button2.selective = False
        button3.selective = False
        button4.selective = False
        self.server_lobby_page.add([label1, label2, button1, button2, button3, button4])

        self.client_lobby_page = form(self.screen)
        label1 = Label(
            pygame.font.SysFont("arial", 50),
            "Tip",
            "Connecting to server...",
            0x696969,
            (100, 50),
        )
        label2 = Label(
            pygame.font.SysFont("arial", 50),
            "skill",
            "placeHolder",
            0x696969,
            (155, 345),
        )
        button1 = Button(
            pygame.font.SysFont("arial", 50),
            "prepare",
            "prepare",
            0x696969,
            (100, 150),
        )
        button2 = Button(
            pygame.font.SysFont("arial", 50),
            "retry",
            "retry",
            0x696969,
            (300, 150),
        )
        button3 = Button(
            pygame.font.SysFont("arial", 50),
            "tomenu",
            "back to menu",
            0x696969,
            (100, 250),
        )
        button4 = Button(
            pygame.font.SysFont("arial", 40),
            "skill_left",
            "<<",
            0xFFFFFF,
            (100, 350),
            0,
            0,
        )
        button5 = Button(
            pygame.font.SysFont("arial", 40),
            "skill_right",
            ">>",
            0xFFFFFF,
            (400, 350),
            0,
            0,
        )
        button1.hide = True
        button2.hide = True
        button2.selective = False
        button3.selective = False
        self.client_lobby_page.add([label1,label2, button1, button2, button3,button4,button5])

        self.error_page = form(self.screen)
        label1 = Label(
            pygame.font.SysFont("arial", 40),
            "l1",
            "",
            0x696969,
            (50, 50),
        )
        label2 = Label(
            pygame.font.SysFont("arial", 40),
            "l2",
            "",
            0x696969,
            (50, 150),
        )
        label3 = Label(
            pygame.font.SysFont("arial", 40),
            "l3",
            "",
            0x696969,
            (50, 250),
        )
        button1 = Button(
            pygame.font.SysFont("arial", 50),
            "confirm",
            "confirm",
            0x696969,
            (50, 350),
        )
        button1.selective = False
        self.error_page.add([label1, label2, label3, button1])

        self.page_draw = {
            MENU: self.draw_menu_page,
            SERVER_LOBBY: self.draw_server_lobby_page,
            CLIENT_LOBBY: self.draw_client_lobby_page,
            INGAME_SOLO: self.draw_game_page,
            INGAME_MULTIPLE: self.draw_game_page,
            ERROR: self.draw_error_page,
            ENDPAGE_SOLO: self.draw_end_page_solo,
            ENDPAGE_MULTIPLE: self.draw_end_page_multiple,
            REMOTE: self.draw_remote_page,
        }
        # -----------------------------------ohters---------------------------
        self.bind_click(1, self.click_left_handler)
        self.score_font = pygame.font.SysFont("arial", 130)
        self.last = 6 * SIDE
        self.barrier = [Barrier(self.screen, SOLID)]
        self.players = {}
        self.error = ""
        self.score_players = {0: 0}
        self.skill_list = [
            ["dash", "dash()"],
            ["wall", "wall()"],
            ["hilaijin", "hilaijinnojyutsu()"],
        ]
        self.currentSkill_index = 0
        # network
        # server
        self.active_connection = 0
        self.prepared_connection = 0
        self.connections = []
        self.ip = "127.0.0.1"
        self.server = None
        self.is_server = False
        self.prepared_list = {0: False}
        # client
        self.ServerTimeOut = False
        self.currentServer = ""
        self.client = None
        self.message_buffer = ""
        self.remote_operation = ""
        self.Server_init()

    def SOLO_init(self):
        if len(self.barrier) > 1:
            self.barrier = [Barrier(self.screen, SOLID)]
        self.score = 0
        self.last = 6 * SIDE
        l = len(self.players.items())
        if l == 0:
            self.players[0] = Player(
                self,
                [dash(pygame.K_q), wall(pygame.K_w), hilaijinnojyutsu(pygame.K_e)],
                id=0,
            )
        elif l > 1:
            self.players = {0: self.players[0]}
            self.players[0].reset()
        else:
            self.players[0].reset()
        self.bindOthers()

    def MULTIPLE_init(self):
        if len(self.barrier) > 1:
            self.barrier = [Barrier(self.screen, SOLID)]
        if len(self.players.items()) == 0:
            self.players[0] = Player(
                self,
                [dash(pygame.K_q)],
                id=0,
            )
        self.score = 0
        self.last = 6 * SIDE
        for player in self.players.values():
            player.reset()

    # --------------------------------key/click related functions----------------------
    def bindSelf(
        self,
        player,
        key_list=[pygame.K_UP, pygame.K_DOWN, pygame.K_LEFT, pygame.K_RIGHT],
    ):
        # 按下事件
        self.bind_key([key_list[0]], player.jump)
        self.bind_key([key_list[1]], player.fall)
        self.bind_key([key_list[2], key_list[3]], player.move)
        if len(player.skills) > 0:
            for skill in player.skills:
                self.bind_key([skill.key], skill.use)
        # 松开事件
        self.bind_key_up([key_list[2], key_list[3]], player.unmove)

    def bindRemote(self):
        self.bind_key(
            [pygame.K_UP, pygame.K_DOWN, pygame.K_LEFT, pygame.K_RIGHT],
            self.remote_operation_down,
        )
        self.bind_key_up([pygame.K_LEFT, pygame.K_RIGHT], self.remote_operation_up)
        if self.skill_list[self.currentSkill_index][0] == "dash":
            self.bind_key([pygame.q], self.remote_skill)
        elif self.skill_list[self.currentSkill_index][0] == "wall":
            self.bind_key([pygame.w], self.remote_skill)
        elif self.skill_list[self.currentSkill_index][0] == "hilajin":
            self.bind_key([pygame.e], self.remote_skill)

    def bindOthers(self):
        self.bind_key([pygame.K_SPACE], self.pause)

    def remote_operation_down(self, key):
        if self.currentPage == REMOTE:
            if key == pygame.K_UP:
                self.remote_operation = "up"
            elif key == pygame.K_DOWN:
                self.remote_operation = "down"
            elif key == pygame.K_LEFT:
                self.remote_operation = "left"
            elif key == pygame.K_RIGHT:
                self.remote_operation = "right"

    def remote_operation_up(self, key):
        if self.currentPage == REMOTE:
            self.remote_operation = "unmove"

    def remote_skill(self):
        if self.client:
            self.client.send(
                self.skill_list[self.currentSkill_index][0].encode("UTF-8")
            )

    def click_left_handler(self, pos):
        if self.currentPage == MENU:
            self.menu_handler(pos)
        elif self.currentPage == ENDPAGE_SOLO:
            self.end_page_solo_handler(pos)
        elif self.currentPage == SERVER_LOBBY:
            self.server_lobby_handler(pos)
        elif self.currentPage == CLIENT_LOBBY:
            self.client_lobby_handler(pos)
        elif self.currentPage == ENDPAGE_MULTIPLE:
            self.end_page_multiple_handler(pos)
        elif self.currentPage == ERROR:
            self.error_page_handler(pos)

    def error_page_handler(self, pos):
        for element in self.error_page.elements:
            if element.type == "button" and element.click_event(pos):
                self.is_server = False
                self.currentPage = MENU

    def client_lobby_handler(self, pos):
        for element in self.client_lobby_page.elements:
            if element.type == "button" and element.click_event(pos):
                if element.name == "prepare":
                    if element.selected:
                        self.client.send(b"prepare")
                    else:
                        self.client.send(b"unprepare")
                elif element.name == "retry":
                    thread = Thread(target=self.AutoFindServer)
                    thread.setDaemon(True)
                    thread.start()
                elif element.name == "tomenu":
                    if self.client:
                        self.client.close()
                        self.client = None
                    if self.is_server:
                        for connection in self.connections:
                            connection.close()
                        self.is_server = False
                    self.currentPage = MENU

    def server_lobby_handler(self, pos):
        for element in self.server_lobby_page.elements:
            if element.type == "button" and element.click_event(pos):
                if element.name == "prepare":
                    if element.selected:
                        self.prepared_connection += 1
                        self.prepared_list[0] = True
                    else:
                        self.prepared_connection -= 1
                        self.prepared_list[0] = False
                elif element.name == "tomenu":
                    if self.client:
                        self.client.close()
                        self.client = None
                    if self.is_server:
                        for connection in self.connections:
                            connection.close()
                        self.is_server = False
                    self.currentPage = MENU
                elif element.name == "skill_left":
                    self.currentSkill_index = (self.currentSkill_index - 1) % len(
                        self.skill_list
                    )
                    del self.players[0]
                    self.players[0] = Player(
                        self,
                        [eval(self.skill_list[self.currentSkill_index][1])],
                        id=0,
                    )
                elif element.name == "skill_right":
                    self.currentSkill_index = (self.currentSkill_index + 1) % len(
                        self.skill_list
                    )
                    del self.players[0]
                    self.players[0] = Player(
                        self,
                        [eval(self.skill_list[self.currentSkill_index][1])],
                        id=0,
                    )

    def menu_handler(self, pos):
        for element in self.menu_page.elements:
            if element.type == "button" and element.click_event(pos):
                if element.name == "Solo":
                    self.menu_page.getElementByName("Multiple_server").setStatus(False)
                    self.menu_page.getElementByName("Multiple_client").setStatus(False)
                elif element.name == "Multiple_server":
                    self.menu_page.getElementByName("Solo").setStatus(False)
                    self.menu_page.getElementByName("Multiple_client").setStatus(False)
                elif element.name == "Multiple_client":
                    self.menu_page.getElementByName("Solo").setStatus(False)
                    self.menu_page.getElementByName("Multiple_server").setStatus(False)
                elif element.name == "Submit":
                    if self.menu_page.getElementByName("Solo").selected:
                        self.SOLO_init()
                        self.debind_key_all()
                        self.bindSelf(self.players[0])
                        self.currentPage = INGAME_SOLO
                    elif self.menu_page.getElementByName("Multiple_server").selected:
                        self.Server_init()
                        if self.server:
                            self.is_server = True
                            self.active_connection = 0
                            self.prepared_connection = 0
                            self.connections = []
                            self.prepared_list = {0: False}
                            self.currentSkill_index = 0
                            self.server_lobby_page.getElementByName(
                                "prepare"
                            ).setStatus(False)
                            self.currentPage = SERVER_LOBBY
                            self.MULTIPLE_init()
                    elif self.menu_page.getElementByName("Multiple_client").selected:
                        thread = Thread(target=self.AutoFindServer)
                        thread.setDaemon(True)
                        thread.start()
                        self.client_lobby_page.getElementByName("prepare").setStatus(
                            False
                        )
                        self.currentPage = CLIENT_LOBBY
                    else:
                        self.error = "you must select a mode"
                        self.currentPage = ERROR

    def end_page_solo_handler(self, pos):
        for element in self.end_page_solo.elements:
            if element.type == "button" and element.click_event(pos):
                if element.name == "restart":
                    self.SOLO_init()
                    self.currentPage = INGAME_SOLO
                elif element.name == "tomenu":
                    self.currentPage = MENU

    def end_page_multiple_handler(self, pos):
        for element in self.end_page_multiple.elements:
            if element.type == "button" and element.click_event(pos):
                if element.name == "restart":
                    if self.menu_page.getElementByName("Multiple_server").selected:
                        self.currentPage = SERVER_LOBBY
                    else:
                        self.currentPage = CLIENT_LOBBY
                elif element.name == "tomenu":
                    if self.client:
                        self.client.close()
                        self.client = None
                    if self.is_server:
                        for connection in self.connections:
                            connection.close()
                        self.is_server = False
                    self.currentPage = MENU

    # -------------------------------network---------------------------
    # client
    def messageLoop(self):
        while True:
            try:
                if self.client:
                    data = self.client.recv(256).decode("UTF-8")
                    print(data)
                    if not data:
                        self.client.close()
                        self.client = None
                        self.currentServer = ""
                        if self.currentPage == CLIENT_LOBBY:
                            self.error = "lost connection with server"
                            self.client_lobby_page.getElementByName(
                                "prepare"
                            ).setStatus(False)
                            self.currentPage = ERROR
                        print(1)
                    elif "ST" in data and self.currentPage == CLIENT_LOBBY:
                        self.client_lobby_page.getElementByName("prepare").setStatus(
                            False
                        )
                        self.debind_key_all()
                        self.bindRemote()
                        self.currentPage = REMOTE
                    elif "END" in data and self.currentPage == REMOTE:
                        self.currentPage = ENDPAGE_MULTIPLE
                        print(3)
                    else:
                        self.message_buffer = data
                        print(4)
            except Exception as e:
                if str(e) != "timed out":
                    self.error = str(e)
                    self.currentPage = ERROR

    def AutoFindServer(self):
        if self.ip == "127.0.0.1":
            self.error = "network error:cannot solve ip"
            self.currentPage = ERROR
            return
        self.ServerTimeOut = False
        if self.client:
            self.client.close()
            self.currentServer = ""
            self.client = None
        ip_prefix = ".".join(self.ip.split(".")[:3]) + "."
        print(ip_prefix)
        for i in range(1, 255):
            thread = Thread(target=self.tryConnect, args=(ip_prefix + str(i),))
            thread.setDaemon(True)
            thread.start()
        time.sleep(5)
        if not self.client:
            self.ServerTimeOut = True

    def tryConnect(self, ip):
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.settimeout(2)
        try:
            client.connect((ip, 7890))
            data = client.recv(128).decode("UTF-8")
            if "OK" in data:
                print(ip)
                self.currentServer = ip
                self.client = client
                thread = Thread(target=self.messageLoop)
                thread.setDaemon(True)
                thread.start()
                return
            else:
                client.close()
        except:
            pass

    # server
    def Server_init(self):
        if self.server:
            return
        try:
            ip_list = self.get_all_hosts()
            for ip in ip_list:
                if ip.startswith("192.168."):
                    self.ip = ip
                    print(ip)
            self.server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.server.bind((self.ip, 7890))
            thread = Thread(target=self.waitForConnect)
            thread.setDaemon(True)
            thread.start()
        except Exception as e:
            self.error = " network error:cannot initialize network"
            self.currentPage = ERROR

    def waitForConnect(self):
        self.server.listen(4)
        while True:
            connection, _ = self.server.accept()
            self.active_connection += 1
            self.connections.append(connection)
            thread = Thread(
                target=self.dealConnection, args=(connection, self.active_connection)
            )
            thread.setDaemon(True)
            thread.start()

    def dealConnection(self, connection, id):
        self.players[id] = Player(self, id=id)
        self.players[id].body_color = randomColor()
        self.score_players[id] = 0
        self.prepared_list[id] = False
        if self.is_server:
            connection.send(b"OKOKOKOKOKOK")
        while True:
            data = connection.recv(128).decode("UTF-8")
            print(data)
            if not data:
                self.active_connection -= 1
                self.connections.remove(connection)
                if self.prepared_list[id]:
                    self.prepared_connection -= 1
                del self.prepared_list[id]
                del self.players[id]
                del self.score_players[id]
                break
            elif data == "prepare":
                self.prepared_connection += 1
                self.prepared_list[id] = True
            elif data == "unprepare":
                self.prepared_connection -= 1
                self.prepared_list[id] = False
            elif data == "up":
                self.players[id].jump()
            elif data == "down":
                self.players[id].fall()
            elif data == "left":
                self.players[id].move(pygame.K_LEFT)
            elif data == "right":
                self.players[id].move(pygame.K_RIGHT)
            elif data == "unmove":
                self.players[id].unmove()

    def get_all_hosts(self):
        host = socket.gethostname()
        hosts = socket.getaddrinfo(host, None)
        # 过滤出IPv4地址
        ipv4_hosts = [host[4][0] for host in hosts if host[0] == socket.AF_INET]
        return ipv4_hosts

    def remote_send(self):
        if self.remote_operation and self.client and self.currentPage == REMOTE:
            self.client.send(self.remote_operation.encode("UTF-8"))
            print(self.remote_operation)
        self.remote_operation = ""

    # ---------------------------------game logic--------------------------------
    def message_format(self):
        message = ""
        for player in self.players.values():
            if player.alive:
                message += (
                    ",".join(
                        list(
                            map(
                                str,
                                [player.body.left, player.body.top, player.body_color],
                            )
                        )
                    )
                    + ";"
                )
        message += ":"
        for barrier in self.barrier:
            message += (
                ",".join(
                    list(map(str, [barrier.rect.left, barrier.rect.top, barrier.type]))
                )
                + ";"
            )
        message += ":"
        message += str(self.score) + ":"
        # 补全到256位
        message += "0" * (256 - len(message))
        return message

    def pause(self, key):
        if self.currentPage == INGAME_SOLO:
            self.is_pause = not self.is_pause

    def message_parse(self, message):
        try:
            print(message)
            players = []
            barriers = []
            data = message.split(":")
            score = int(data[2])
            for playerStatus in data[0].split(";")[:-1]:
                players.append(
                    list(map(int, playerStatus.split(",")))
                )  # l,t,color(l,t,color)
            for barrierStatus in data[1].split(";")[:-1]:
                barriers.append(list(map(int, barrierStatus.split(","))))
            return (players, barriers, score)
        except Exception as err:
            self.error = str(err)
            self.currentPage = ERROR

    # ---------------------------graphical functions----------------------------
    def draw_remote_page(self):
        try:
            if self.message_buffer:
                players, barriers, self.score = self.message_parse(self.message_buffer)
                self.screen.fill(0x000000)
                self.draw_score(hex2rgb(0x3C3C3C))
                for player in players:
                    rect = pygame.Rect(player[0], player[1], SIDE, SIDE)
                    self.screen.fill(hex2rgb(player[2]), rect)
                for barrier in barriers:
                    ba = Barrier(self.screen, barrier[2], barrier[0], barrier[1])
                    ba.draw()
                pygame.display.update()
        except Exception as e:
            self.error = str(e)
            self.currentPage = ERROR

    def draw_game_page(self):
        self.screen.fill(0x000000)
        self.draw_score(hex2rgb(0x3C3C3C))
        for ba in self.barrier:
            ba.draw()
        for player in self.players.values():
            player.draw()
        pygame.display.update()

    # 绘制开始页面
    def draw_menu_page(self):
        self.menu_page.draw()
        pygame.display.update()

    # 绘制结束页面
    def draw_end_page_solo(self):
        self.end_page_solo.getElementByName("score").setText("score:" + str(self.score))
        self.end_page_solo.draw()
        pygame.display.update()

    def draw_end_page_multiple(self):
        self.end_page_solo.getElementByName("score").setText("score:" + str(self.score))
        self.end_page_solo.draw()
        pygame.display.update()

    def draw_server_lobby_page(self):
        self.server_lobby_page.getElementByName("active_connection").setText(
            "players connected:" + str(self.active_connection)
        )
        if self.active_connection != 0:
            self.server_lobby_page.getElementByName("prepare").hide = False
        else:
            self.server_lobby_page.getElementByName("prepare").hide = True
            self.server_lobby_page.getElementByName("prepare").setStatus(False)
        self.server_lobby_page.getElementByName("skill").setText(
            string_align_cnter(self.skill_list[self.currentSkill_index][0], 14)
        )
        self.server_lobby_page.draw()
        pygame.display.update()

    def draw_client_lobby_page(self):
        if self.ServerTimeOut:
            self.client_lobby_page.getElementByName("Tip").setText("Failed to connect")
            self.client_lobby_page.getElementByName("retry").hide = False
            self.client_lobby_page.getElementByName("prepare").hide = True
        elif self.currentServer:
            self.client_lobby_page.getElementByName("Tip").setText(
                "Connected to " + self.currentServer
            )
            self.client_lobby_page.getElementByName("prepare").hide = False
            self.client_lobby_page.getElementByName("retry").hide = True
        else:
            self.client_lobby_page.getElementByName("prepare").hide = True
            self.client_lobby_page.getElementByName("retry").hide = True
            self.client_lobby_page.getElementByName("Tip").setText(
                "Connecting to server..."
            )
        self.client_lobby_page.draw()
        pygame.display.update()

    def draw_error_page(self):
        errorClip = ["", "", ""]
        l = len(self.error)
        errorClip[0] = self.error[:40] if l > 40 else self.error
        errorClip[1] = self.error[40:80] if l > 80 else self.error[40:]
        errorClip[2] = self.error[80:] if l > 80 else ""
        for i in range(3):
            self.error_page.elements[i].setText(errorClip[i])
        self.error_page.draw()
        pygame.display.update()

    def to_hell(self):
        """
        更新所有人物状态
        receive:key
        exec key=>action
        """
        # 主逻辑
        self.last -= 1  # 生成两个障碍物的间隔（时间？）
        # （初始化为self.last = 6 * SIDE）
        # 生成新障碍
        if self.last == 0:
            self.create_barrier()
            self.last = randint(3, 5) * SIDE
            # 去看barrier的生成逻辑
        # 更新障碍状态
        for ba in self.barrier:
            if not ba.rise():
                # 如果这个ba（不在屏幕内）或者（是frag且被踩掉了）
                if ba.type == FRAGILE and ba.rect.top > 0:
                    # 屏幕内被踩掉的frag
                    self.score += 1
                self.barrier.remove(ba)
        # 逐人物处理
        for player in self.players.values():
            if not player.alive:
                continue
            # 更新技能
            for skill in player.skills:
                skill.update()
            # 左右移动
            player.move_man()
            # 下降
            player.fall_man()
            # 障碍交互逻辑
            player.exec_barriers()
        """
        回传各个人物位置，障碍位置，分数
        """

    def create_barrier(self, x=None, y=None):
        if x != None:
            self.barrier.append(Barrier(self.screen, SOLID, x, y))
            return
        solid = list(filter(lambda ba: ba.type == SOLID, self.barrier))
        # 保证至少有一个solid平台
        if len(solid) < 1:
            self.barrier.append(Barrier(self.screen, SOLID))
        else:
            self.barrier.append(Barrier(self.screen))
        # 回到update中

    def update(self, current_time=None):
        # 更新下一帧状态
        if self.currentPage == INGAME_SOLO:
            if not self.players[0].alive:
                self.currentPage = ENDPAGE_SOLO
            if self.is_pause:
                return
            self.to_hell()
        elif self.currentPage == INGAME_MULTIPLE:
            end = True
            for player in self.players.values():
                if player.alive:
                    end = False
            if end:
                self.currentPage = ENDPAGE_MULTIPLE
                for connection in self.connections:
                    connection.send(b"ENDENDENDEND" + b"0" * 244)
            self.to_hell()
            message = self.message_format()
            for connection in self.connections:
                connection.send(message.encode("UTF-8"))
        elif self.currentPage == REMOTE:
            self.remote_send()
        elif self.currentPage == SERVER_LOBBY:
            if self.prepared_connection == self.active_connection + 1:
                self.prepared_connection = 0
                self.server_lobby_page.getElementByName("prepare").select()
                self.MULTIPLE_init()
                self.debind_key_all()
                self.bindSelf(self.players[0])
                self.currentPage = INGAME_MULTIPLE
                for connection in self.connections:
                    connection.send(b"STSTSTSTSTST" + b"0" * 244)

    def draw(self, current_time=None):
        # 绘制下一帧
        self.page_draw[self.currentPage]()

    def quit(self):
        if self.client:
            self.client.close()
        for connection in self.connections:
            connection.close()
        if self.server != None:
            self.server.close()


if __name__ == "__main__":
    hell = Hell("to the hell", (SCREEN_WIDTH, SCREEN_HEIGHT))
    hell.run()
