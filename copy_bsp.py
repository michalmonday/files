''' 
Usage: py -3 copy_bsp.py project_name

This script copies BSP files and Utilities to specified project. 

This script should be placed in the workspace folder of STM32CubeIDE. 
In the same directory where project folders are created. 
REPOSITORY_PATH must be manually modified in this script in order to work.

Whenever "Project -> Generate Code" option is used, the Utilities folder 
is deleted from the project, so a script like this one could be used to quickly restore it.

This script was tested on Windows 10 with V1.27.1 sotfware for the STM32F412G-Discovery board.
'''

import argparse
from pathlib import Path
import shutil
import os

REPOSITORY_PATH = Path("C:/Users/michal/STM32Cube/Repository/STM32Cube_FW_F4_V1.27.1")

def get_project_path():
    parser = argparse.ArgumentParser(
        description = (
            'This program copies BSP files from STM32 repository to the specified project\n'
            '(please modify path to STM32 repository before running it)'
        ),
        usage = 'py -3 copy_bsp.py project_name'
    )
    parser.add_argument('project_name')
    args = parser.parse_args()
    return Path(__file__).parent / args.project_name 

PROJECT_PATH = get_project_path()

# list of directories to copy
to_copy = [
    'Drivers/BSP/STM32412G-Discovery',
    'Drivers/BSP/Components',
    'Utilities/Fonts'
]

# these files result in compilation errors for some reason
# more details: https://community.st.com/t5/stm32-mcus-products/how-to-setup-stm32f412g-discovery-bsp-for-the-display-in/m-p/592469#M223508
to_not_copy = [
    'Drivers/BSP/STM32412G-Discovery/stm32412g_discovery_audio.h',
    'Drivers/BSP/STM32412G-Discovery/stm32412g_discovery_audio.c'
]

for path in to_copy:
    src_path = REPOSITORY_PATH / path
    dst_path = PROJECT_PATH / path
    shutil.copytree(src_path, dst_path, dirs_exist_ok=True)

# remove files that should not be copied (from the destination folder)
# the source files are not removed
for path in to_not_copy:
    name = PROJECT_PATH / path
    if os.path.isfile(name):
        os.remove(name)
    elif os.path.isdir(name):
        os.rmdir(name)
    else:
        raise Exception(f'ERROR: "{name}" was not found.')

