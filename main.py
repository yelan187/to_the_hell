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

UNSTART = 2
SOLO = 0
MUTIPLE = 1


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
        self.player = [Player(self)]
        # 按下事件
        self.bindSelf(self.player[0])

    # --------------------------------key related functions----------------------

    def bindSelf(self, player):
        # 按下事件
        self.bind_key([pygame.K_LEFT, pygame.K_RIGHT], player.move)
        self.bind_key([pygame.K_DOWN], player.fall)
        self.bind_key([pygame.K_UP], player.jump)
        if Player.skills:
            for skill in Player.skills:
                self.bind_key([skill.key], skill.use)
        # 松开事件
        self.bind_key_up([pygame.K_LEFT, pygame.K_RIGHT], self.unmove)

    def bindOthers(self):
        self.bind_key(pygame.K_SPACE, self.pause)
        self.bind_key(pygame.K_RETURN, self.startGame)

    def startGame(self, key):
        if self.gameMode != UNSTART and self.end:
            self.gameMode = UNSTART
            self.end = False
            self.draw_menu()
        else:
            self.gameMode = self.gameSelect

    # ---------------------------------game logic--------------------------------
    def draw_menu(self):
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


    def fall_man(self, id=0):
        self.body[id].top += self.fallingSpeed
        self.fallingSpeed = (
            self.fallingSpeed + ACCELERATION
            if self.fallingSpeed < MAX_SPEED
            else MAX_SPEED
        )

    def move_man(self,player, vel=MOVING_SPEED):
        # 此时dire应该已经被game里的handle_input赋值为了某个常数
        # 或者是被传送带赋值
        if player.dire == 0:
            return True
        rect = player.body.copy()
        if player.dire == pygame.K_LEFT:
            rect.left -= vel
        else:
            rect.left += vel
        if rect.left < 0 or rect.left + SIDE >= SCREEN_WIDTH:
            return False
        player.body = rect
        return True
        # 这些返回值在哪里有用？

    def get_score(self, ba, id=0):
        # 从to_hell来的
        # if 人物在平台上方 then 获得分数
        if self.body[id].top > ba.rect.top and not ba.score:
            self.score += 1
            ba.score = True
            # 因为每次循环都会调用这个函数，这里表示这个ba的分数已经拿到过了

    def exec_barriers(self, id=0):
        for ba in self.barrier:
            if not self.body[id].colliderect(ba.rect):
                self.get_score(ba)
                continue
            # 以下都是建立在角色踩在障碍物的条件之上的
            if ba.type == DEADLY:
                self.show_end()
                return
            # 角色跟随障碍物
            # 如果不写这个-2的话，会判断为colliderect，一直continue，一直移动不了

            self.fallingSpeed = 0
            # self.body.top = ba.rect.top - SIDE - 2
            self.body[id].top -= 2
            if ba.type == FRAGILE:
                ba.frag_touch = True
            elif ba.type == BELT_LEFT or ba.type == BELT_RIGHT:
                self.move_man(ba.belt_dire, BELT_SPEED)
            break

        top = self.body[id].top
        if top < 0 or top + SIDE >= SCREEN_HEIGHT:
            self.show_end()
            """TO FIX"""

    def to_hell(self):
        # 人物和障碍交互逻辑
        self.last -= 1  # 生成两个障碍物的间隔（时间？）
        # （初始化为self.last = 6 * SIDE）
        # 生成新障碍
        if self.last == 0:
            self.create_barrier()
            self.last = randint(3, 5) * SIDE
            # 去看barrier的生成逻辑
        # 更新影子状态
        self.skills.hilaijin.rise()
        # 更新障碍状态
        for ba in self.barrier:
            if not ba.rise():
                # 如果这个ba（不在屏幕内）或者（是frag且被踩掉了）
                if ba.type == FRAGILE and ba.rect.top > 0:
                    # 屏幕内被踩掉的frag
                    self.score += 1
                self.barrier.remove(ba)

        # 左右移动
        self.move_man(self.dire)
        # 下降
        self.fall_man()
        # self.body.top += FALLING_SPEED
        # 障碍交互逻辑
        self.exec_barriers()

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

    def update(self, current_time):
        # 更新下一帧状态
        if self.gameMode != UNSTART:
            if self.end or self.is_pause:
                return
            self.to_hell()

    def draw(self, current_time, end=False):
        # 绘制下一帧
        if self.gameMode != UNSTART:
            if self.end or self.is_pause:
                return
            self.screen.fill(0x000000)
            self.draw_score((0x3C, 0x3C, 0x3C))
            for ba in self.barrier:
                ba.draw()
            if self.skills.hilaijin.ishilaijin:
                self.screen.fill(COLOR[SHADOW], self.skills.hilaijin.shadow)
            if not end:
                for body in self.body:
                    self.screen.fill(COLOR[BODY], body)
            else:
                self.screen.fill(COLOR[DEADLY], body)
            """TO FIX"""
        else:
            self.draw_menu()
        pygame.display.update()


if __name__ == "__main__":
    skills = Skills()
    hell = Hell("to the hell", (SCREEN_WIDTH, SCREEN_HEIGHT), skills=skills)
    # 先声明了类，那我应该去看初始化
    hell.run()
