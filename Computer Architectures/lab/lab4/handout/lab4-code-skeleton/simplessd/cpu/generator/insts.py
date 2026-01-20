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

# [ Regex of instruction, CPI, TYPE ]
# TYPE:
#  0: Others
#  1: Branch
#  2: Load
#  3: Store
#  4: Arithmetic
#  5: FP
aarch64 = [
    ["NOP", 1, 0],
    ["CAS[B|H|P|]", 1, 0],
    ["SWP[B|H|]", 1, 0],
    # Branch
    ["B\.(EQ|NE|CS|HS|CC|LO|MI|PL|VS|VC|HI|LS|GE|LT|GT|LE|AL|NV)", 1, 1],
    ["CBN?Z", 1, 1],
    ["TBN?Z", 1, 1],
    ["B", 1, 1],
    ["BL", 1, 1],
    ["BLR", 2, 1],
    ["BR", 1, 1],
    ["RET", 1, 1],
    # Load
    ["LDR(B|SB|H|SH|SW|)", 4, 2],
    ["LDUR(B|SB|H|SH|SW|)", 4, 2],
    ["LDP(SW|)", 4, 2],
    ["LDNP", 4, 2],
    ["LDTR(B|SB|H|SH|SW|)", 4, 2],
    ["LDXR(B|H|)", 4, 2],
    ["LDXP", 4, 2],
    ["LDAPR(B|H|)", 4, 2],
    ["LDAR(B|H|)", 4, 2],
    ["LDAXR(B|H|)", 4, 2],
    ["LDAXP", 4, 2],
    ["LDLAR(B|H|)", 4, 2],
    # Store
    ["STR(B|H|)", 1, 3],
    ["STUR(B|H|)", 1, 3],
    ["STP", 2, 3],
    ["STNP", 2, 3],
    ["STTR(B|H|)", 1, 3],
    ["STXR(B|H|)", 1, 3],
    ["STXP", 1, 3],
    ["STLR(B|H|)", 1, 3],
    ["STLXR(B|H|)", 1, 3],
    ["STLXP", 1, 3],
    ["STLLR(B|H|)", 1, 3],
    # Arithmetic
    ["ADD(S|)", 1, 4],
    ["SUB(S|)", 1, 4],
    ["CMP", 1, 4],
    ["CMN", 1, 4],
    ["AND(S|)", 1, 4],
    ["EOR", 1, 4],
    ["ORR", 1, 4],
    ["TST", 1, 4],
    ["MOV(Z|N|K|)", 1, 4],
    ["ADR(P|)", 1, 4],
    ["BFM", 2, 4],
    ["SBFM", 2, 4],
    ["UBFM", 2, 4],
    ["BFC", 2, 4],
    ["BFI", 2, 4],
    ["BFXIL", 1, 4],
    ["SBFIZ", 2, 4],
    ["SBFX", 1, 4],
    ["UBFIZ", 2, 4],
    ["UBFX", 1, 4],
    ["EXTR", 1, 4],
    ["ASR(V|)", 1, 4],
    ["LSL(V|)", 1, 4],
    ["LSR(V|)", 1, 4],
    ["ROR(V|)", 1, 4],
    ["SXT(B|H|W|)", 2, 4],
    ["UXT(B|H|)", 2, 4],
    ["NEG(S|)", 1, 4],
    ["ADC(S|)", 1, 4],
    ["SBC(S|)", 1, 4],
    ["NGC(S|)", 1, 4],
    ["BIC(S|)", 1, 4],
    ["EON", 1, 4],
    ["MNV", 1, 4],
    ["ORN", 1, 4],
    # Multiply and Divide
    ["MADD", 3, 4],
    ["MSUB", 3, 4],
    ["MNEG", 3, 4],
    ["MUL", 3, 4],
    ["SMADDL", 3, 4],
    ["SMSUBL", 3, 4],
    ["SMNEGL", 3, 4],
    ["SMULL", 3, 4],
    ["SMULH", 6, 4],
    ["UMADDL", 3, 4],
    ["UMSUBL", 3, 4],
    ["UMNEGL", 3, 4],
    ["UMULL", 3, 4],
    ["UMULH", 6, 4],
    ["SDIV", 20, 4],
    ["UDIV", 20, 4],
    # Bit operation
    ["CLS", 1, 4],
    ["CLZ", 1, 4],
    ["RBIT", 1, 4],
    ["REV(16|32|64|)", 1, 4],
    # Conditional select
    ["CSEL", 1, 4],
    ["CSINC", 1, 4],
    ["CSINV", 1, 4],
    ["CSNEG", 1, 4],
    ["CSET(M|)", 1, 4],
    ["CINC", 1, 4],
    ["CINV", 1, 4],
    ["CNEG", 1, 4],
    # Conditional comparison
    ["CCMN", 1, 4],
    ["CCMP", 1, 4],
    # Floating Point
    ["FMOV", 5, 5],
    ["FCVT(XN|)", 5, 5],
    ["FCVT(AS|AU|MS|MU|NS|NU|PS|PU|ZS|ZU)", 10, 5],
    ["FJCVTZS", 1, 5],
    ["SCVTF", 10, 5],
    ["UCVTF", 10, 5],
    ["FRINT(A|I|M|N|P|X|Z|)", 5, 5],
    ["FMADD", 9, 5],
    ["FMSUB", 9, 5],
    ["FNMADD", 9, 5],
    ["FNMSUB", 9, 5],
    ["FABS", 3, 5],
    ["FNEG", 3, 5],
    ["FSQRT", 20, 5],
    ["FADD", 5, 5],
    ["FDIV", 20, 5],
    ["FMUL", 6, 5],
    ["FNMUL", 6, 5],
    ["FSUB", 5, 5],
    ["FMAX", 5, 5],
    ["FMAXNM", 5, 5],
    ["FMIN", 5, 5],
    ["FMINNM", 5, 5],
    ["FCMP(E|P|PE|)", 3, 5],
    ["FCSEL", 3, 5],
]
