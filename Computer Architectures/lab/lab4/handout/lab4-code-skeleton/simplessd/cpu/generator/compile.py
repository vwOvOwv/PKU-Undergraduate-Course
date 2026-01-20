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

import subprocess
import os


def compile(cxx, src, dst, rootpath):
    command = [cxx,
               '-std=c++11',
               '-O2',
               '-g',
               '-I' + rootpath,
               '-I' + os.path.join(rootpath, '../../../../ext/drampower/src'),
               '-I' + os.path.join(rootpath, '../lib/drampower/src'),
               '-c',
               src,
               '-o',
               dst]

    proc = subprocess.Popen(
        command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    output = proc.communicate()[0]

    if proc.returncode:
        print output

    return proc.returncode


def disassemble(objdump, src, dst):
    command = [objdump,
               '-S',
               '-d',
               src]

    proc = subprocess.Popen(
        command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    output = proc.communicate()[0]

    if proc.returncode == 0:
        with open(dst, 'w')  as f:
            f.write(output)

    return proc.returncode
