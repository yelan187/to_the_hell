import pygame
from game import *
from random import choice, randint

# ---------------------------------def consts________________________________________
SCORE = 0
SOLID = 1
FRAGILE = 2
DEADLY = 3
BELT_LEFT = 4
BELT_RIGHT = 5
BODY = 6
SHADOW = 7

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


# ----------------------------------------技能类------------------------------------------
class Skills(object):
    def __init__(self):
        self.skill = {
            pygame.K_q: self.dash,
            pygame.K_w: self.wall,
            pygame.K_e: self.hilaijinnojyutsu,
        }
        # dash
        self.dire = 0
        # hilaijinnojyutsu
        self.hilaijin = Hilaijin()

    def dash(self, none, distance=5):
        self.dire = hell.dire
        if self.dire == pygame.K_LEFT:
            hell.body.x -= SIDE * distance if hell.body.x - SIDE * distance > 2 else 2
        elif self.dire == pygame.K_RIGHT:
            hell.body.x += (
                SIDE * distance
                if hell.body.x + SIDE * distance < SCREEN_WIDTH - SIDE - 2
                else SCREEN_WIDTH - SIDE - 2
            )
        return

    def wall(self, none):
        hell.create_barrier(x=hell.body.x - 10, y=hell.body.y + SIDE + 25)

    def hilaijinnojyutsu(self, none):
        # print(self.hilaijin.ishilaijin)
        if self.hilaijin.ishilaijin == 0:
            self.hilaijin.ishilaijin = 1
            self.hilaijin.make_shadow()
        else:
            self.hilaijin.ishilaijin = 0
            hell.body = self.hilaijin.shadow
            self.hilaijin.shadow = None


class Hilaijin(object):
    def __init__(self):
        self.ishilaijin = 0

    def make_shadow(self):
        self.shadow = hell.body.copy()

    def rise(self, vel=ROLLING_SPEED):
        if not self.ishilaijin:
            return
        self.shadow.y -= vel
        if self.shadow.y - SIDE < 0:
            self.ishilaijin = 0
            self.shadow = None


# ----------------------------障碍类-----------------------------
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


class Hell(Game):
    def __init__(self, title, size, fps=60):
        super(Hell, self).__init__(title, size, fps)
        self.gameMode = UNSTART
        self.gameSelect = SOLO
        self.score_font = pygame.font.SysFont("arial", 130)
        self.text_font = pygame.font.SysFont("arial", 50)
        self.title_font = pygame.font.SysFont("arial", 80)
        # 去把game里的初始化也看了
        # 那边没什么太值得看的
        self.last = 6 * SIDE
        #  这个变量是什么意思？在update里
        self.dire = 0
        self.fallingSpeed = 0
        self.barrier = [Barrier(self.screen, SOLID)]
        # 第一个肯定是solid防止开局就死
        self.body = pygame.Rect(self.barrier[0].rect.center[0], 200, SIDE, SIDE)
        # 按下事件
        self.bind_key([pygame.K_LEFT, pygame.K_RIGHT], self.move)
        self.bind_key([pygame.K_DOWN], self.fall)
        self.bind_key([pygame.K_UP], self.jump)
        self.bind_key(skills.skill)
        self.bind_key(pygame.K_SPACE, self.pause)
        self.bind_key(pygame.K_RETURN, self.startGame)
        self.bind_key(pygame.K_r, self.restart)
        # 松开事件
        self.bind_key_up([pygame.K_LEFT, pygame.K_RIGHT], self.unmove)

    # --------------------------------key related functions----------------------
    def jump(self, key, vel=-8):
        if (not self.is_pause) and (self.gameMode != UNSTART):
            # self.body.top -= 70
            self.fallingSpeed = vel
        elif self.gameMode == UNSTART:
            self.gameSelect = 1 - self.gameSelect

    def fall(self, key):
        if (not self.is_pause) and (self.gameMode != UNSTART):
            self.fallingSpeed = 14
        elif self.gameMode == UNSTART:
            self.gameSelect = 1 - self.gameSelect

    def move(self, key):
        # 从game.py来的，key是我按键的key属性。
        # 接下来看看dire有什么用，到move_man里去
        if self.gameMode != UNSTART:
            self.dire = key
        # 会把我按下的按键的key属性作为参数传给dire，但我传的是一个key属性，这是一个常数值，映射为一个按键

    def unmove(self, key):
        self.dire = 0

    def startGame(self,key):
        self.gameMode = self.gameSelect

    def restart(self, key):
        if self.gameMode != UNSTART and self.end:
            self.gameMode = UNSTART
            self.gameSelect = SOLO
            self.end = False
            self.last = 6 * SIDE
            self.dire = 0
            self.fallingSpeed = 0
            self.barrier = [Barrier(self.screen, SOLID)]
            self.body = pygame.Rect(self.barrier[0].rect.center[0], 200, SIDE, SIDE)
            self.draw_menu()
    # ---------------------------------game logic--------------------------------
    def draw_menu(self):
        self.screen.fill(0x000000)
        game_title = self.title_font.render("Starting Screen", True, hex2rgb(0xFFFFFF))
        solo_text = self.text_font.render("Solo", True, hex2rgb(0xFFFFFF))
        muti_text = self.text_font.render("Mutiplayer", True, hex2rgb(0xFFFFFF))
        self.screen.blit(game_title, (180, 200))
        self.screen.blit(solo_text, (390, 350))
        self.screen.blit(muti_text, (320, 450))
        if self.gameSelect == SOLO:
            pygame.draw.rect(self.screen, hex2rgb(0xB0C4DE), (320, 350, 235, 60))
            pygame.draw.rect(self.screen, hex2rgb(0xFFFFFF), (320, 450, 235, 60))
        else:
            pygame.draw.rect(self.screen, hex2rgb(0xB0C4DE), (320, 450, 235, 60))
            pygame.draw.rect(self.screen, hex2rgb(0xFFFFFF), (320, 350, 235, 60))

    def show_end(self):
        self.draw(0, end=True)
        self.end = True

    def fall_man(self):
        self.body.top += self.fallingSpeed
        self.fallingSpeed = (
            self.fallingSpeed + ACCELERATION
            if self.fallingSpeed < MAX_SPEED
            else MAX_SPEED
        )

    def move_man(self, dire, vel=MOVING_SPEED):
        # 此时dire应该已经被game里的handle_input赋值为了某个常数
        # 或者是被传送带赋值
        if dire == 0:
            return True
        rect = self.body.copy()
        if dire == pygame.K_LEFT:
            rect.left -= vel
        else:
            rect.left += vel
        if rect.left < 0 or rect.left + SIDE >= SCREEN_WIDTH:
            return False
        # for ba in self.barrier:
        #     if rect.colliderect(ba.rect):
        #         # return False
        #         pass
        self.body = rect
        return True
        # 这些返回值在哪里有用？

    def get_score(self, ba):
        # 从to_hell来的
        # if 人物在平台上方 then 获得分数
        if self.body.top > ba.rect.top and not ba.score:
            self.score += 1
            ba.score = True
            # 因为每次循环都会调用这个函数，这里表示这个ba的分数已经拿到过了

    def exec_barriers(self):
        for ba in self.barrier:
            if not self.body.colliderect(ba.rect):
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
            self.body.top -= 2
            if ba.type == FRAGILE:
                ba.frag_touch = True
            elif ba.type == BELT_LEFT or ba.type == BELT_RIGHT:
                self.move_man(ba.belt_dire, BELT_SPEED)
            break

        top = self.body.top
        if top < 0 or top + SIDE >= SCREEN_HEIGHT:
            self.show_end()

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
        skills.hilaijin.rise()
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
            if skills.hilaijin.ishilaijin:
                self.screen.fill(COLOR[SHADOW], skills.hilaijin.shadow)
            if not end:
                self.screen.fill(COLOR[BODY], self.body)
            else:
                self.screen.fill(COLOR[DEADLY], self.body)
        else:
            self.draw_menu()
        pygame.display.update()


if __name__ == "__main__":
    skills = Skills()
    hell = Hell("to the hell", (SCREEN_WIDTH, SCREEN_HEIGHT))
    # 先声明了类，那我应该去看初始化
    hell.run()
    # game里找run
