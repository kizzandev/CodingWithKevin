import os
from pathlib import Path

""" Organizes folder according to the categories in the dictionary """

SUBDIRECTORIES = {
    'Documents': ['.pdf', '.rtf', '.txt', '.md', '.xlsx', '.doc'],
    'Audio': ['.m4a', 'm4b', '.mp3'],
    'Videos': ['.mov', '.avi', '.mp4', '.flv', '.mkv'],
    'Images': ['.jpg', '.jpeg', '.png', '.svg', '.gif'],
    'Python_files': ['.py', '.pyc'],
    'CPP_files': ['.cpp', '.hpp'],
    'Executables': ['.bat', '.exe']
} # Group type: extensions


def pickDirectory(value):
    for category, suffixes in SUBDIRECTORIES.items():
        for suffix in suffixes:
            if suffix == value:
                return category
    return 'MISC' # if it's not found, returns MISC


def organizeDirectory():
    for item in os.scandir():
        if item.is_dir() or item.name == 'organize.py':  # excludes directories and this file
            continue
        filePath = Path(item)
        filetype = filePath.suffix.lower()
        directory = pickDirectory(filetype)
        directoryPath = Path(directory)
        if directoryPath.is_dir() != True:
            directoryPath.mkdir()
        filePath.rename(directoryPath.joinpath(filePath))


# run
organizeDirectory()
