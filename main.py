import pygame
from game import *
from random import choice, randint
from utills import *
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
    BELT_LEFT: 0xFFFF44,
    BELT_RIGHT: 0xFF99FF,
    BODY: 0x00FF00,
    SHADOW: 0xFFFF00,
    TITLE: 0xB22222,
    TEXT: 0x696969,
}
CHOICE = [SOLID, SOLID, SOLID, FRAGILE, FRAGILE, BELT_LEFT, BELT_RIGHT, DEADLY]

ROLLING_SPEED = 2
FALLING_SPEED = 3
MOVING_SPEED = 2
BELT_SPEED = 1
ACCELERATION = 0.2
MAX_SPEED = 10
MAX_JUMP_CHANCE = 2


SOLO = 0
MUTIPLE = 1
UNSTART = 2
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
        self.gameMode = UNSTART
        self.gameSelect = SOLO
        self.score_font = pygame.font.SysFont("arial", 130)
        self.text_font = pygame.font.SysFont("arial", 50)
        self.title_font = pygame.font.SysFont("arial", 80)
        self.last = 6 * SIDE
        self.barrier = [Barrier(self.screen, SOLID)]
        self.players = [Player(self, [dash(pygame.K_q), hilaijinnojyutsu(pygame.K_e)])]
        # 按下事件
        self.bindSelf(self.players[0])
        self.bindOthers()

    def game_init(self):
        self.last = 6 * SIDE
        self.barrier = [Barrier(self.screen, SOLID)]
        for player in self.players:
            player.reset()

    # --------------------------------key related functions----------------------
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
        if self.gameMode != UNSTART and self.end:
            self.gameMode = UNSTART
            self.end = False
            self.game_init()
        else:
            if self.gameSelect == MUTIPLE:
                if len(self.players) > 1:
                    return
                self.players.append(Player(self))
                self.bindSelf(
                    self.players[-1], [pygame.K_t, pygame.K_g, pygame.K_f, pygame.K_h]
                )

            self.gameMode = INGAME

    # ---------------------------------game logic--------------------------------
    def draw_menu(self):
        if self.gameMode != UNSTART:
            return
        self.screen.fill(0x000000)
        if self.gameSelect == SOLO:
            pygame.draw.rect(self.screen, hex2rgb(0xB0C4DE), (320, 350, 235, 60))
            pygame.draw.rect(self.screen, hex2rgb(0xFFFFFF), (320, 450, 235, 60))
        else:
            pygame.draw.rect(self.screen, hex2rgb(0xB0C4DE), (320, 450, 235, 60))
            pygame.draw.rect(self.screen, hex2rgb(0xFFFFFF), (320, 350, 235, 60))
        game_title = self.title_font.render("To The Hell", True, hex2rgb(COLOR[TITLE]))
        solo_text = self.text_font.render("Solo", True, hex2rgb(COLOR[TEXT]))
        muti_text = self.text_font.render("Mutiplayer", True, hex2rgb(COLOR[TEXT]))
        x = self.screen.get_rect().center[0]
        self.screen.blit(game_title, (270, 200))
        self.screen.blit(solo_text, (390, 350))
        self.screen.blit(muti_text, (340, 450))
        pygame.display.update()

    def draw_end(self):
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

    def to_hell(self):
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
        if self.gameMode != UNSTART:
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
        if self.gameMode != UNSTART:
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
