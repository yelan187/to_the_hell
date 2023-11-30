import pygame
from random import choice, randint
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


# ---------------------------------------平台类---------------------------------------
class Barrier(object):
    def __init__(self, screen, opt=None, x=None, y=None):
        self.screen = screen
        if opt is None:
            self.type = choice(CHOICE)
        else:
            self.type = opt
        self.frag_touch = False
        self.frag_time = 12
        self.score = False
        self.belt_dire = 0
        self.belt_dire = pygame.K_LEFT if self.type == BELT_LEFT else pygame.K_RIGHT
        left = randint(0, SCREEN_WIDTH - 7 * SIDE - 1) if x == None else x
        top = SCREEN_HEIGHT - SIDE - 1 if y == None else y
        self.rect = pygame.Rect(left, top, 7 * SIDE, SIDE)

    def rise(self, vel=ROLLING_SPEED):
        if self.frag_touch:
            # 如果我碰到了frag
            self.frag_time -= 1
        if self.frag_time == 0:
            return False
        self.rect.top -= vel  # 往上走
        return self.rect.top >= 0  # 在屏幕内返回True
        # 回到update

    def draw_side(self, x, y):
        if self.type == SOLID:
            rect = pygame.Rect(x, y, SIDE, SIDE)
            self.screen.fill(COLOR[SOLID], rect)
        elif self.type == FRAGILE:
            rect = pygame.Rect(x + 2, y, SIDE - 4, SIDE)
            self.screen.fill(COLOR[FRAGILE], rect)
        elif self.type == BELT_LEFT or self.type == BELT_RIGHT:
            rect = pygame.Rect(x, y, SIDE, SIDE)
            pygame.draw.circle(
                self.screen, COLOR[self.type], rect.center, SIDE // 2 + 1
            )
        elif self.type == DEADLY:
            p1 = (x + SIDE // 2 + 1, y)
            p2 = (x, y + SIDE)
            p3 = (x + SIDE, y + SIDE)
            points = [p1, p2, p3]
            pygame.draw.polygon(self.screen, COLOR[DEADLY], points)

    # 绘制平台
    def draw(self):
        for i in range(7):
            self.draw_side(i * SIDE + self.rect.left, self.rect.top)


# ------------------------------------------玩家类------------------------------------
class Player:
    def __init__(self, hell, skill=None, id=0, name="player", SIDE=13):
        self.skills = skill  # List[functions]
        for skill in self.skills:
            skill.bind(self)
        self.hell = hell
        self.id = id
        self.name = name
        self.body = pygame.Rect(self.hell.barrier[0].rect.center[0], 200, SIDE, SIDE)
        self.dire = 0
        self.fallingSpeed = 0
        self.acceleration = 0.2

    def jump(self, key, vel=-8):
        if (not self.hell.is_pause) and (self.hell.gameMode != UNSTART):
            # self.body.top -= 70
            self.fallingSpeed = vel
        elif self.hell.gameMode == UNSTART:
            self.hell.gameSelect = 1 - self.hell.gameSelect

    def fall(self, key):
        if (not self.hell.is_pause) and (self.hell.gameMode != UNSTART):
            self.fallingSpeed = 14
        elif self.hell.gameMode == UNSTART:
            self.hell.gameSelect = 1 - self.hell.gameSelect

    def move(self, key):
        if self.hell.gameMode != UNSTART:
            self.dire = key


    def unmove(self, key):
        self.dire = 0
    
    def reset(self,x = 0,y = 0):
        self.body.center = (x,y)
        self.fallingSpeed = 0
        self.dire = 0
#--------------------------------------技能类----------------------------------------
class Skill:
    def __init__(self,key):
        self.key = key#key:pygame.Key
        
    def bind(self, player):
        self.player = player

    def use():
        pass

