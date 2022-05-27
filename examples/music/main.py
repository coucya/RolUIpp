import sys, os

import mutagen
from mutagen.easyid3 import EasyID3

import pygame
from pygame import mixer

import PyRolUI
from PyRolUI import *
from PyRolUI import widgets
from PyRolUI.xml_tool import *
from PyRolUI.functional_component import *
from PyRolUI.vscroll_panel import *

from PyRolUI_glfw import *

def load_file(path: str) -> str:
    with open(path, "r", encoding="utf8") as f:
        return f.read()


RESOURCES_DIR = os.path.abspath(os.path.dirname( __file__) + "/../resources")
MUSICS_DIR = r"D:\CloudMusic"

def load_image_resource(name:str) -> Image:
    path = os.path.join(RESOURCES_DIR, name)
    return load_image(path)

def load_music_image(path:str):
    try:
        info = mutagen.File(path)
        image_data = info.tags["APIC:"].data
        image = load_image_mem_file(image_data)
        return image
    except Exception as e:
        return Image()
    

def load_music_infos(path:str) -> list[dict]:
    files = map(lambda f: os.path.join(MUSICS_DIR,f), os.listdir(path))
    music_infos = []
    for i, music_path in enumerate(files):
        id3 = EasyID3(music_path)
        music_info = {}
        music_info["font"] ="C:\\WINDOWS\\FONTS\\MSYHL.TTC"
        music_info["music_path"] = music_path
        music_info["music_index"] = i
        try:
            music_info["music_title"]=id3["title"][0] 
        except:
            music_info["music_title"] = music_path
        try:
            music_info["music_artist"] = id3["artist"][0]
        except:
            music_info["music_artist"] =  ""
        music_info["music_image"] = load_music_image(music_path)
        music_info["on_select"] = make_on_select(music_info)
        music_infos.append(music_info)
    return music_infos

    
CTRL_BUTTON_IMAGES = {
    "prev": Image(), "next": Image(),
    "play": Image(), "pause": Image()
}

def play_music(music_info: dict):
    music = pygame.mixer.music.load(music_info["music_path"])
    mixer.music.play()  
    context["play_button_image"].set(CTRL_BUTTON_IMAGES["pause"])
    context["playing_music_image"].set(music_info["music_image"])
    context["playing_music_title"].set(music_info["music_title"])
    context["playing_music_artist"].set(music_info["music_artist"])
    context["playing_music_path"].set(music_info["music_path"])
    context["current_music_info"] = music_info
    

def on_prev():
    if len(context["music_list"]) == 0:
        return
    if get_current_music_info() is None:
        play_music(context["music_list"][-1])
    else:
        mi = get_current_music_info().get("music_index", 0) - 1
        play_music(context["music_list"][mi])

def on_next():
    if len(context["music_list"]) == 0:
        return
    if get_current_music_info() is None:
        play_music(context["music_list"][0])
    else:
        mi = get_current_music_info().get("music_index", -1) + 1
        play_music(context["music_list"][mi])


def on_play():
    if get_current_music_info() is not None:
        if mixer.music.get_busy():
            mixer.music.pause()
            context["play_button_image"].set(CTRL_BUTTON_IMAGES["play"])
        else:
            mixer.music.unpause()
            context["play_button_image"].set(CTRL_BUTTON_IMAGES["pause"])

def make_on_select(music_info:dict):
    def _on_select():
        play_music(music_info)
    return _on_select


def init():
    load_font("C:\\WINDOWS\\FONTS\\MSYHL.TTC")
    CTRL_BUTTON_IMAGES["prev"] = load_image_resource("prev-wrap.png")
    CTRL_BUTTON_IMAGES["next"] = load_image_resource("next-wrap.png")
    CTRL_BUTTON_IMAGES["play"] = load_image_resource("play-wrap.png")
    CTRL_BUTTON_IMAGES["pause"] = load_image_resource("pause-wrap.png")
     
    
context = {}
def get_current_music_info() -> dict:
    return context.get("current_music_info", None)

def main():
    pygame.mixer.init()

    win = GLFWWindow(800, 600, "music")
    win.on_exit = lambda: Application.exit()
    # win.on_size = lambda *args: Application.flush_frame()

    Application.init(win)
    
    init()

    music_list = load_music_infos(MUSICS_DIR)
    context["music_list"] = music_list
    context["music_progress"] = State(0)

    context["font"] = "C:\\WINDOWS\\FONTS\\MSYHL.TTC"
    context["image_fit_fill"] = ImageWidget.fill
    context["image_fit_contain"] = ImageWidget.contain
    context["image_fit_cover"] = ImageWidget.cover

    context["playing_music_image"] = State(Image())
    context["playing_music_title"] = State("")
    context["playing_music_artist"] = State("")
    context["playing_music_path"] = State("")

    context["play_button_image"] = State(CTRL_BUTTON_IMAGES["play"])
    context["prev_button_image"] = State(CTRL_BUTTON_IMAGES["prev"])
    context["next_button_image"] = State(CTRL_BUTTON_IMAGES["next"])
    context["on_prev"] = on_prev
    context["on_next"] = on_next
    context["on_play"] = on_play

    xml_str = load_file(os.path.join(os.path.dirname(__file__), "ui.xml"))
    w = build_widget_from_xml(xml_str, context)

    Application.run(w)

if __name__ == "__main__":
    main()

