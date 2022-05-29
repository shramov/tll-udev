#!/usr/bin/env python3
# vim: sts=4 sw=4 et

import os

import time

from tll.channel import Context
from tll.error import TLLError
from tll.processor import Loop

ctx = Context()
ctx.load(os.path.join(os.environ.get("BUILD_DIR", "build"), "tll-udev"), 'channel_module')

loop = Loop()

c = ctx.Channel(f'udev://', subsystem='', name='udev', dump='scheme')
c.open()
loop.add(c)

while True:
    loop.step(1)
