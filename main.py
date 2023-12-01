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
TEXT = 8
TITLE = 9
BOUNCE = 10

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
    TITLE: 0xB22222,
    TEXT: 0x696969,
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
            "1",
            "Score:"+str(self.score),
            0x696969,
            (self.screen.get_rect().center[0] - 185, 350),
        )
        self.end_page.add(([label1, label2]))

        self.bind_click(1, self.click_left_handler)
        # self.bindOthers()
        self.score_font = pygame.font.SysFont("arial", 130)
        self.last = 6 * SIDE
        self.players = []
        self.barrier = [Barrier(self.screen, SOLID)]

    def game_init(self):
        self.score = 0
        self.last = 6 * SIDE
        self.barrier = [Barrier(self.screen, SOLID)]
        for player in self.players:
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

    def bindOthers(self):
        self.bind_key([pygame.K_SPACE], self.pause)
        self.bind_key([pygame.K_RETURN], self.startGame)

    def startGame(self, key):
        if self.gameMode != INMENU and self.end:
            self.gameMode = INMENU
            self.end = False
            self.game_init()
        else:
            if self.players == []:
                self.players = [
                    Player(
                        self,
                        [
                            dash(pygame.K_q),
                            wall(pygame.K_w),
                            hilaijinnojyutsu(pygame.K_e),
                        ],
                    )
                ]
                # 按下事件
                self.bindSelf(self.players[0])
                self.bindOthers()
            if self.gameSelect == MUTIPLE:
                self.players.append(Player(self))
                self.bindSelf(
                    self.players[-1], [pygame.K_t, pygame.K_g, pygame.K_f, pygame.K_h]
                )
            else:
                self.players = self.players[0:1]
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
                        element.select()
                        self.startGame(1)

    # ---------------------------------game logic--------------------------------

    # 绘制开始页面
    def draw_menu(self):
        if self.gameMode != INMENU:
            return
        self.menu.draw()
        pygame.display.update()

    # 绘制结束页面
    def draw_end(self):
        self.end_page.draw()
        pygame.display.update()
        """
        self.screen.fill(0x000000)
        end_text = self.title_font.render("Game Over", True, hex2rgb(COLOR[TITLE]))
        score_text = self.text_font.render(
            "Score: " + str(self.score), True, hex2rgb(COLOR[TEXT])
        )
        tip_text = self.text_font.render(
            "Press Enter To Restart", True, hex2rgb(COLOR[TEXT])
        )
        self.screen.blit(end_text, (self.screen.get_rect().center[0] - 185, 250))
        self.screen.blit(score_text, (self.screen.get_rect().center[0] - 100, 400))
        self.screen.blit(tip_text, (self.screen.get_rect().center[0] - 220, 500))
        pygame.display.update()
        """

    def to_hell(self):
        '''
        更新所有人物状态
        receive:key
        exec key=>action
        '''
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
        '''
        回传各个人物位置，障碍位置，分数
        '''

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
            end = True
            for player in self.players:
                if player.alive:
                    end = False
            if end:
                self.draw()
                self.end = True
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
