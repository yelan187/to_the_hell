import pygame
from pygame.locals import *
from sys import exit

FOUR_NEIGH = {"left": (0, -1), "right": (0, 1), "up": (-1, 0), "down": (1, 0)}
EIGHT_NEIGH = list(FOUR_NEIGH.values()) + [(1, 1), (1, -1), (-1, 1), (-1, -1)]
DIRECTION = {pygame.K_UP: "up", pygame.K_LEFT: "left", pygame.K_RIGHT: "right", pygame.K_DOWN: "down"}


def hex2rgb(color):
    b = color % 256
    color = color >> 8
    g = color % 256
    color = color >> 8
    r = color % 256
    return (r, g, b)


class Game(object):
    def __init__(self, title, size, fps=30):
        self.size = size
        pygame.init()
        self.screen = pygame.display.set_mode(size, 0, 32)
        pygame.display.set_caption(title)
        self.keys = {}
        self.keys_up = {}
        self.clicks = {}
        self.timer = pygame.time.Clock()
        self.fps = fps
        self.score = 0
        self.end = False
        self.fullscreen = False
        self.last_time = pygame.time.get_ticks()
        self.is_pause = False
        self.is_draw = True
        self.score_font = pygame.font.SysFont("Calibri", 100, True)

    def bind_key(self, key, action=None):
        if isinstance(key, list):
            for k in key:
                self.keys[k] = action
            #把action作为value传到keys字典中的k键（list里的每个元素）下面
            #这里action是一个函数（方法），我调用的时候还得另外传参
        elif isinstance(key, int):
            self.keys[key] = action
        elif isinstance(key, dict):
            self.keys.update(key)

    def bind_key_up(self, key, action):
        if isinstance(key, list):
            for k in key:
                self.keys_up[k] = action
        elif isinstance(key, int):
            self.keys_up[key] = action

    def bind_click(self, button, action):
        self.clicks[button] = action

    def pause(self, key):
        self.is_pause = not self.is_pause

    def set_fps(self, fps):
        self.fps = fps

    def handle_input(self, event):
        # 到这里了
        # print(event.type)
        if event.type == pygame.QUIT:
            pygame.quit()
            exit()
            #这会直接导致程序关闭
        if event.type == pygame.KEYDOWN:
            # 哦，这个判断的条件是“按下按键事件”
            # print('已按下按键')
            if event.key in self.keys.keys():
                self.keys[event.key](event.key)
                #把我的event的key去self.keys字典里对应。注意这里左右键都绑定成了move函数，
                #那么我每次都会把我的按键对象event的key属性作为参数传到move里面
            '''
            if event.key == pygame.K_F11:                           # F11全屏
                self.fullscreen = not self.fullscreen
                if self.fullscreen:
                    self.screen = pygame.display.set_mode(self.size, pygame.FULLSCREEN, 32)
                else:
                    self.screen = pygame.display.set_mode(self.size, 0, 32)
            '''
        if event.type == pygame.KEYUP:
            #抬起按键
            if event.key in self.keys_up.keys():
                self.keys_up[event.key](event.key)


    def run(self):
        while True:
            for event in pygame.event.get():
                # 一开游戏就运行这个循环了，监听我的键盘和鼠标，然后把我的按键作为event对象传到handleinput里面
                self.handle_input(event)
                # 处理我的按键（？
            self.timer.tick(self.fps)
            #控制帧率，timer是一个对象，是pygame.time.clock(),tick是其下的一个方法，每次循环都执行这句
            #来控制帧率，fps是类下的属性可以直接访问
            self.update(pygame.time.get_ticks())
            #update在游戏程序里
            self.draw(pygame.time.get_ticks())

    def draw_score(self, color, rect=None):
        score = self.score_font.render(str(self.score), True, color)
        if rect is None:
            r = self.screen.get_rect()
            rect = score.get_rect(center=r.center)
        self.screen.blit(score, rect)

    def is_end(self):
        return self.end

    def update(self, current_time):
        pass

    def draw(self, current_time):
        pass