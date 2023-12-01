import pygame
from game import *
from random import choice, randint
from utills import *
from UI import *
import time

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
    BODY: 0x00FF00,
    SHADOW: 0xFFFF00,
}
CHOICE = [SOLID, SOLID, SOLID, FRAGILE, FRAGILE, BELT_LEFT, BELT_RIGHT, DEADLY, BOUNCE]

ROLLING_SPEED = 2
FALLING_SPEED = 3
MOVING_SPEED = 2
BELT_SPEED = 1
ACCELERATION = 0.2
MAX_SPEED = 10
MAX_JUMP_CHANCE = 2


SOLO = 0
MUTIPLE = 1
INMENU = 2
INGAME = 3
ENDPAGE = 4


#
# ------------------------------------main game----------------------------------------
class Hell(Game):
    def __init__(self, title, size, fps=60):
        super(Hell, self).__init__(title, size, fps)
        """
        **Params**
        player:List[Player]
        gameMode:int
        gameSelect:int
        score_font:font
        text_font:font
        tite_font:font
        last:int
        barrier:List[Barrier]
        
        """
        self.gameMode = INMENU
        self.gameSelect = SOLO
        # --------------------------------pages------------------------------
        self.menu = form(self.screen)
        label = Label(
            pygame.font.SysFont("arial", 80),
            "title",
            "To The Hell",
            0xB22222,
            (270, 200),
        )
        button1 = Button(
            pygame.font.SysFont("arial", 50), "Solo", "Solo", 0x696969, (390, 350)
        )
        button2 = Button(
            pygame.font.SysFont("arial", 50),
            "Multiple",
            "Multiple",
            0x696969,
            (360, 450),
        )
        button3 = Button(
            pygame.font.SysFont("arial", 50),
            "Submit",
            "Submit",
            0x696969,
            (365, 550),
        )
        button1.select()
        button3.selective = False
        self.menu.add([label, button1, button2, button3])

        self.end_page = form(self.screen)
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
        button4 = Button(
            pygame.font.SysFont("arial", 50),
            "restart",
            "click to restart",
            0x696969,
            (self.screen.get_rect().center[0] - 185, 450),
        )
        button5 = Button(
            pygame.font.SysFont("arial", 50),
            "tomenu",
            "back to menu",
            0x696969,
            (self.screen.get_rect().center[0] - 185, 550),
        )
        button4.selective = False
        button5.selective = False
        self.end_page.add(([label1, label2, button4,button5]))
        #-----------------------------------ohters---------------------------
        self.bind_click(1, self.click_left_handler)
        # self.bindOthers()
        self.score_font = pygame.font.SysFont("arial", 130)
        self.last = 6 * SIDE
        self.barrier = []
        self.players = []

    def SOLO_init(self):
        self.barrier = [Barrier(self.screen, SOLID)]
        self.score = 0
        self.end = False
        self.last = 6 * SIDE
        l = len(self.players)
        if l == 0:
            self.players.append(
                Player(
                    self,
                    [dash(pygame.K_q), wall(pygame.K_w), hilaijinnojyutsu(pygame.K_e)],
                )
            )
            self.bindSelf(self.players[-1])
        elif l > 1:
            self.players = self.players[:1]
        else:
            self.players[-1].reset()

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

    def bindOthers(self):
        self.bind_key([pygame.K_SPACE], self.pause)
        # self.bind_key([pygame.K_RETURN], self.startGame)

    def startGame(self):
        if self.gameSelect == SOLO:
            self.SOLO_init()
        self.gameMode = INGAME

    def click_left_handler(self, pos):
        if self.gameMode == INMENU:
            for element in self.menu.elements:
                if element.type == "button" and element.click_event(pos):
                    if element.name == "Solo":
                        self.gameSelect = SOLO
                        self.menu.getElementByName("Multiple").select()
                    elif element.name == "Multiple":
                        self.gameSelect = MUTIPLE
                        self.menu.getElementByName("Solo").select()
                    elif element.name == "Submit":
                        self.startGame()
        elif self.gameMode == ENDPAGE:
            for element in self.end_page.elements:
                if element.type == "button" and element.click_event(pos):
                    if element.name == "restart":
                        self.startGame()
                    elif element.name == "tomenu":
                        self.gameMode = INMENU
                        self.end = False

    # ---------------------------------game logic--------------------------------

    # 绘制开始页面
    def draw_menu(self):
        if self.gameMode != INMENU:
            return
        self.menu.draw()
        pygame.display.update()

    # 绘制结束页面
    def draw_end(self):
        self.end_page.getElementByName("score").setText("score:" + str(self.score))
        self.end_page.draw()
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
        for player in self.players:
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
        if self.gameMode != INMENU:
            self.end = True
            for player in self.players:
                if player.alive:
                    self.end = False
            if self.end:
                self.gameMode = ENDPAGE
            if self.end or self.is_pause:
                return
            self.to_hell()

    def draw(self, current_time=None):
        # 绘制下一帧
        if self.gameMode != INMENU:
            if self.is_pause:
                return
            if self.end:
                self.draw_end()
                return
            self.screen.fill(0x000000)
            self.draw_score((0x3C, 0x3C, 0x3C))
            for ba in self.barrier:
                ba.draw()
            for player in self.players:
                player.draw()
            pygame.display.update()
        else:
            self.draw_menu()


if __name__ == "__main__":
    hell = Hell("to the hell", (SCREEN_WIDTH, SCREEN_HEIGHT))
    # 先声明了类，那我应该去看初始化
    hell.run()
