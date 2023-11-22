#!/usr/bin/python
import base64
import lzma
import subprocess
import sys
import tempfile

with tempfile.NamedTemporaryFile() as exe_file:
    compile_command = f'g++-12 {" ".join(sys.argv[1:])} -o {exe_file.name}'
    strip_command = f'strip {exe_file.name}'

    res = subprocess.run(compile_command, shell=True)
    assert res.returncode == 0
    res = subprocess.run(strip_command, shell=True)
    assert res.returncode == 0

    payload = base64.b85encode(lzma.compress(exe_file.read())).decode()

print(f'''
import os
import sys
import lzma
import base64
f=os.memfd_create('',os.MFD_CLOEXEC)
os.write(f,lzma.decompress(base64.b85decode('{payload}')))
os.execve(f,sys.argv,os.environ)
'''.strip())

