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

import re
from insts import aarch64

asmregex = re.compile('[0-9a-f ]+:\t[0-9a-f ]+\t([\w\.]+)')
endregex = re.compile('[0-9a-f]{16} <')


def decode(line):
    match = asmregex.match(line)

    if match:
        return match.group(1)

    return ''


def categorize(opstring):
    for list in aarch64:
        regex = re.compile(list[0], re.IGNORECASE)
        match = regex.match(opstring)

        if match:
            if match.group(0) == opstring:
                return [True, list[1], list[2]]

    return [False, 1, 0]


def matchFunction(line, funcstr):
    match = endregex.match(line)

    if match:
        if line.find(funcstr) >= 0:
            return 1
        else:
            return 2
    else:
        return 0


def analyse(asmfile, funcname):
    category = [0, 0, 0, 0, 0, 0]
    funcstr = str(len(funcname)) + funcname

    with open(asmfile, 'r') as f:
        collect = False

        for line in f:
            if collect:
                match = matchFunction(line, funcstr)

                if match > 0:
                    if match == 2:
                        collect = False
                else:
                    op = decode(line)

                    if len(op) > 0:
                        ret = categorize(op)

                        if not ret[0]:
                            print('Unknown assembly ' + op + ' found')

                        category[ret[2]] += ret[1]
            else:
                if matchFunction(line, funcstr) == 1:
                    collect = True

    return category
