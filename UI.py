import pygame
from random import randint

def hex2rgb(color):
    b = color % 256
    color = color >> 8
    g = color % 256
    color = color >> 8
    r = color % 256
    return (r, g, b)

def randomColor():
    r = randint(50, 255)
    g = randint(50, 255)
    b = randint(50, 255)
    return (r*256*256+g*256+b)


# --------------------------------------控件类--------------------------------
class Label:
    def __init__(self, font, name, text="", color=0xFFFFFF, pos=(0, 0)):
        self.text = text
        self.name = name
        self.font = font
        self.color = hex2rgb(color)
        self.x = pos[0]
        self.y = pos[1]
        self.active = True
        self.hide = False
        self.type = "label"
        if text and font:
            self.label = self.font.render(self.text, 1, self.color,0)
        else:
            self.label = None

    def draw(self, screen):
        if not self.hide:
            screen.blit(self.label, (self.x, self.y))

    def setPos(self, x, y):
        self.x = x
        self.y = y

    def setText(self, text):
        self.text = text
        self.label = self.font.render(self.text, 1, self.color,0)

    def setColor(self, color):
        self.color = color
        self.label = self.font.render(self.text, 1, self.color,0)


class Button(Label):
    def __init__(
        self,
        font,
        name,
        text="",
        color=0xFFFFFF,
        pos=(0, 0),
        color_select=0xB0C4DE,
        color_unselect=0xFFFFFF,
    ):
        super(Button, self).__init__(font, name, text, color, pos)
        self.type = "button"
        self.selected = False
        self.selective = True
        self.color_select = hex2rgb(color_select)
        self.color_unselect = hex2rgb(color_unselect)
        self.label = self.font.render(self.text, 1, self.color, self.color_unselect)

    def select(self):
        if self.selective:
            if self.selected:
                self.label = self.font.render(
                    self.text, 1, self.color, self.color_unselect
                )
            else:
                self.label = self.font.render(
                    self.text, 1, self.color, self.color_select
                )
            self.selected = not self.selected

    def setStatus(self, status):
        if self.selected != status:
            self.select()

    def click_event(self, pos):
        if (
            pos[0] > self.x
            and pos[0] < self.x + self.label.get_width()
            and pos[1] > self.y
            and pos[1] < self.y + self.label.get_height()
        ):
            self.select()
            return True
        return False


class InputBox(Button):
    def __init__(
        self,
        font,
        name,
        text="",
        color=0xFFFFFF,
        pos=(0, 0),
        color_select=0xB0C4DE,
        color_unselect=0xFFFFFF,
        judge_function=lambda x: True,
    ):
        super(InputBox, self).__init__(
            font, name, text, color, pos, color_select, color_unselect
        )
        self.type = "inputbox"
        self.judge_function = judge_function
        self.active = False

    def addchar(self, event):
        if self.active:
            if event.key == pygame.K_BACKSPACE:
                self.text = self.text[:-1]
            elif event.key == pygame.K_RETURN:
                if self.judge_function(self.text):
                    self.active = False
                else:
                    self.label = self.font.render(
                        "illegal", 1, self.color, self.color_unselect
                    )
                    return
            else:
                self.text += event.unicode
            self.label = self.font.render(self.text, 1, self.color, self.color_select)

    def click_event(self, pos):
        if (
            pos[0] > self.x
            and pos[0] < self.x + self.label.get_width()
            and pos[1] > self.y
            and pos[1] < self.y + self.label.get_height()
        ):
            self.select()
            self.active = True
            return True
        return False


# ------------------------------页面类-------------------------------------
class form:
    def __init__(self, screen):
        self.screen = screen
        self.elements = []

    def draw(self):
        self.screen.fill(0x000000)
        for element in self.elements:
            element.draw(self.screen)

    def add(self, elements):
        for element in elements:
            self.elements.append(element)

    def getElementByName(self, name):
        for element in self.elements:
            if element.name == name:
                return element
        return None

    def getElementByType(self, type):
        elements = []
        for element in self.elements:
            if element.type == type:
                elements.append(element)
        return elements