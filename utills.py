import pygame
from random import choice, randint
import time

# ---------------------------------def consts-----------------------------------
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
CHOICE = [SOLID, SOLID, SOLID, FRAGILE, FRAGILE, BELT_LEFT, BELT_RIGHT, DEADLY,BOUNCE]

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


# ---------------------------------------平台类---------------------------------------
class Barrier:
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
        elif self.type == BOUNCE:
            rect = pygame.Rect(x, y, SIDE, SIDE)
            self.screen.fill(COLOR[BOUNCE], rect)
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
    """
    玩家类
    可以通过self.hell访问主进程
    self.skills为列表,其中元素为对应的技能类(继承自Skill)
    hell进程中将各个按键绑定
    reset将玩家状态重置,位置改为hell中第一个barrier位置
    draw绘制自身及技能衍生物
    """

    def __init__(self, hell, skill=[], id=0, name="player", SIDE=13):
        self.screen = hell.screen
        self.skills = skill  # List[functions]
        for skill in self.skills:
            skill.bind(self)
        # 特殊属性
        self.hell = hell
        self.id = id
        self.name = name
        self.alive = True
        self.dead_clear = False
        self.body = pygame.Rect(self.hell.barrier[0].rect.center[0], 200, SIDE, SIDE)
        self.dire = 0
        self.fallingSpeed = 0
        self.acceleration = ACCELERATION
        self.jumpChance = MAX_JUMP_CHANCE

    def jump(self, key = 1, vel=-8):
        if (not self.hell.is_pause) and (self.hell.gameMode != UNSTART):
            if self.jumpChance > 0:
                self.fallingSpeed = vel
                self.jumpChance -= 1

    def fall(self, key = 1):
        if (not self.hell.is_pause) and (self.hell.gameMode != UNSTART):
            self.fallingSpeed = 14

    def move(self, key = 1):
        if self.hell.gameMode != UNSTART:
            self.dire = key

    def unmove(self, key = 1):
        self.dire = 0

    def reset(self):
        self.body = pygame.Rect(self.hell.barrier[0].rect.center[0], 200, SIDE, SIDE)
        self.fallingSpeed = 0
        self.dire = 0
        self.jumpChance = 2
        self.acceleration = ACCELERATION
        self.alive = True
        for skill in self.skills:
            skill.reset()

    def get_score(self, ba):
        # if 人物在平台上方 then 获得分数
        if self.body.top > ba.rect.top and not ba.score:
            self.hell.score += 1
            ba.score = True
            # 因为每次循环都会调用这个函数，这里表示这个ba的分数已经拿到过了

    def exec_barriers(self):
        for ba in self.hell.barrier:
            if not self.body.colliderect(ba.rect):
                self.get_score(ba)
                continue
            # 以下都是建立在角色踩在障碍物的条件之上的
            if ba.type == DEADLY:
                self.alive = False
                return
            if ba.type == BOUNCE:
                self.fallingSpeed = -8
                return
            # 角色跟随障碍物
            self.fallingSpeed = 0
            self.body.top -= ROLLING_SPEED
            # 重置跳跃次数
            self.jumpChance = MAX_JUMP_CHANCE
            if ba.type == FRAGILE:
                ba.frag_touch = True
            elif ba.type == BELT_LEFT or ba.type == BELT_RIGHT:
                self.move_man(ba.belt_dire, BELT_SPEED)
            break

        top = self.body.top
        if top < 0 or top + SIDE >= SCREEN_HEIGHT:
            self.alive = False

    def fall_man(self):
        self.body.top += self.fallingSpeed
        self.fallingSpeed = (
            self.fallingSpeed + self.acceleration
            if self.fallingSpeed < MAX_SPEED
            else MAX_SPEED
        )

    def move_man(self, dire=None, vel=MOVING_SPEED):
        # 此时dire应该已经被game里的handle_input赋值为了某个常数
        # 或者是被传送带赋值
        if dire is None:
            dire = self.dire
        if dire == 0:
            return
        rect = self.body.copy()
        if dire == pygame.K_LEFT:
            rect.left -= vel
        else:
            rect.left += vel
        if rect.left < 0 or rect.left + SIDE >= SCREEN_WIDTH:
            return 
        self.body = rect

    def draw(self):
        if self.alive:
            self.screen.fill(COLOR[BODY], self.body)
            for skill in self.skills:
                if skill.product != None:
                    self.screen.fill(COLOR[skill.product_color], skill.product)
        elif not self.dead_clear:
            self.screen.fill(COLOR[DEADLY], self.body)
            self.dead_clear = True


# --------------------------------------技能类----------------------------------------
class Skill:
    """
    技能根类
    bind会在声明player时自动调用
    使用self.player访问对应玩家
    use在每次按下绑定按键时调用
    update在主进程每次update(即生成下一帧)时调用
    自定义子类可实现标准化操作
    """

    def __init__(self, key = 1):
        self.key = key  # key:pygame.Key
        self.product = None
        self.product_color = None

    def bind(self, player):
        self.player = player

    def use(self):
        pass

    def update(self):
        pass
    def reset(self):
        pass


class dash(Skill):
    def __init__(self, key = 1):
        super(dash, self).__init__(key)
        self.cd = 0

    def use(self, key, distance=5):
        if self.cd != 0:
            return
        dire = self.player.dire
        if dire == pygame.K_LEFT:
            self.player.body.x -= (
                SIDE * distance if self.player.body.x - SIDE * distance > 2 else 2
            )
        elif dire == pygame.K_RIGHT:
            self.player.body.x += (
                SIDE * distance
                if self.player.body.x + SIDE * distance < SCREEN_WIDTH - SIDE - 2
                else SCREEN_WIDTH - SIDE - 2
            )
        self.cd = 60

    def update(self):
        if self.cd > 0:
            self.cd -= 1

    def reset(self):
        self.cd = 0


class wall(Skill):
    def __init__(self, key = 1):
        super(wall, self).__init__(key)
        self.cd = 0

    def use(self, key):
        if self.cd != 0:
            return
        self.player.hell.create_barrier(
            x=self.player.body.x - 10, y=self.player.body.y + SIDE + 25
        )
        self.cd = 90

    def update(self):
        if self.cd > 0:
            self.cd -= 1

    def reset(self):
        self.cd = 0


class hilaijinnojyutsu(Skill):
    def __init__(self, key = 1):
        super(hilaijinnojyutsu, self).__init__(key)
        self.ishilaijin = 0
        self.product_color = SHADOW
        self.cd = 0

    def use(self, key):
        if self.ishilaijin == 1:
            self.player.body = self.product
            self.player.fallingSpeed = 0
            self.player.jumpChance = MAX_JUMP_CHANCE - 1
            self.ishilaijin = 0
            self.product = None
            return 
        if self.cd != 0:
            return  
        self.product = self.player.body.copy()
        self.ishilaijin = 1
        self.cd = 60


    def update(self, vel=ROLLING_SPEED):
        if self.cd > 0:
            self.cd -= 1
        if self.ishilaijin == 1:
            self.product.y -= vel
            if self.product.y - SIDE < 0:
                self.ishilaijin = 0
                self.product = None
                self.cd = 0

    def reset(self):
        self.ishilaijin = 0
        self.cd = 0
        self.product = None