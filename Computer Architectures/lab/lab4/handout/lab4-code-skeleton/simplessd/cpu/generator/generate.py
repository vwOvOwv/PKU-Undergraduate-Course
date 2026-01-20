# Copyright (C) 2017 CAMELab
#
# This file is part of SimpleSSD.
#
# SimpleSSD is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SimpleSSD is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SimpleSSD.  If not, see <http://www.gnu.org/licenses/>.

import os
import shutil
from functions import function as funcList
from compile import compile, disassemble
from analyse import analyse

# Begin of main
print('SimpleSSD Instruction Counter\n')

# Find root directory of SimpleSSD
pwd = os.getcwd()
rootdir = ''

while True:
    if pwd == '/':
        print('Failed to find SimpleSSD root directory')
        exit(-1)

    if os.path.isdir(os.path.join(pwd, 'cpu')) and os.path.isdir(os.path.join(pwd, 'hil')):
        rootdir = os.path.abspath(pwd)
        break

    pwd = os.path.join(pwd, '..')

print('SimpleSSD found on ' + rootdir)

# Create temp directory
tempdir = os.path.join(rootdir, 'temp_build')

if os.path.exists(tempdir):
    shutil.rmtree(tempdir)

os.mkdir(tempdir)

if not os.path.isdir(tempdir):
    print('Failed to create temp directory')
    exit(-1)

print('Temporary directory ' + tempdir + ' created')

# Load files to compile & disassemble
result = []
files = set()

for func in funcList:
    files.add(func[0])

files = list(files)

# Compile all
print('Begin compile')

for file in files:
    path = os.path.join(rootdir, file)
    filename = os.path.splitext(file.replace('/', '_'))[0] + '.o'
    outfile = os.path.join(tempdir, filename)

    if os.path.exists(path):
        ret = compile('aarch64-linux-gnu-g++', path, outfile, rootdir)

        if ret:
            print(' file ' + file + ' failed to compile')
    else:
        print(' file ' + file + ' not exists')

# Disassemble all
print('Begin disassemble')

for file in files:
    path = os.path.join(rootdir, file)
    filename = os.path.splitext(file.replace('/', '_'))[0]
    name = os.path.join(tempdir, filename)
    objfile = name + '.o'
    asmfile = name + '.asm'

    if os.path.exists(objfile):
        ret = disassemble('aarch64-linux-gnu-objdump', objfile, asmfile)

        if ret:
            print(' file ' + file + ' failed to disassemble')

# Analyse all
print('Begin analysis')

for func in funcList:
    path = os.path.join(rootdir, func[0])
    filename = os.path.splitext(func[0].replace('/', '_'))[0] + '.asm'
    asmfile = os.path.join(tempdir, filename)

    if os.path.exists(asmfile):
        insts = analyse(asmfile, func[1])

        result.append([func[1], func[2], func[3], insts])
    else:
        print('Assembly file for function ' + func[1] + ' not exists')

# Everything is done
# Print result
template = """cpi.find({1})->second.insert(
    {{{2}, InstStat({4}, {5}, {6}, {7}, {8}, {3}, clockPeriod)}});"""

for data in result:
    print(template.format(data[0], data[1], data[2],
                          data[3][0], data[3][1], data[3][2],
                          data[3][3], data[3][4], data[3][5]))

# Remove temp directory
shutil.rmtree(tempdir)
