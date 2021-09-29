# Big giant hack to take the tom-thumb.bdf font and turn it
# into a set of uint32_t's for embedding.
# This script also makes a JSON representation of the core
# parts of the font for the purposes of exploration.
#
# Copyright (c) 2021, Andrew Gwozdziewycz
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
#       copyright notice, this list of conditions and the following
#       disclaimer in the documentation and/or other materials
#       provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.


buf = open("/home/apg/Downloads/tom-thumb.bdf").read()
chars = buf.split("STARTCHAR ")
out = {}
for c in chars[1:]:
    lines = c.split('\n')
    attrs = {}
    bitmap = 0
    for x in lines:
        if bitmap >= 1:
            bitmap -= 1
            attrs["bitmap"].append(int(x, 16))
            continue

        if x.startswith("ENCODING "):
            attrs["ascii"] = int(x[9:])
        elif x.startswith("BBX "):
            box = list(map(int, x[4:].split()))
            attrs["bbox"] = box
        elif x.startswith("BITMAP"):
            attrs["bitmap"] = []
            bitmap = attrs["bbox"][1]

    if attrs["ascii"] > 127:
        break

    out[lines[0]] = attrs


# font has 127 characters. Many of which are empty.
font = [0] * 127

for c in out.values():
    box = c['bbox']
    ch = 0
    for b in c['bitmap']:
        p = (b >> 4) # pattern
        p >>= box[2]
        ch = (ch << 4) | p

    font[c['ascii']] = ch

# JSON:
import json
print(json.dumps(out))

print()
print()
print("static tomThumb uint32_t[127] = {")
print(", ".join([hex(x) for x in font]))
print("};")
