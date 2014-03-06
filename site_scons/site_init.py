# site_init.py
#
# This file is read before every SConstruct and SConscript. So, anything that
# should be available to every build script should go here.
#
# Eryn Wells <eryn@erynwells.me>

import os.path
import SCons.Defaults
import binaries


BUILD_DIR = Dir('#build')
LIB_DIR = Dir('#lib')
SRC_DIR = Dir('#src')
TEST_DIR = Dir('#test')

#
# SCons Command Line Options
#

AddOption('--show-build-cmds',
          dest='build_cmds',
          action='store_true',
          help='Show build commands instead of friendly build messages')
AddOption('--mode',
          dest='mode',
          metavar='MODE',
          help='The build mode')

#
# Environment Configuration
#

def has_clang(env):
    _, cc_tail = os.path.split(env['CC'])
    _, cxx_tail = os.path.split(env['CXX'])
    return any([cc_tail.startswith('clang'), cxx_tail.startswith('clang')])

default_env = SCons.Defaults.DefaultEnvironment()
default_env['CC'] = binaries.first(['clang', 'gcc'])
default_env['CXX'] = binaries.first(['clang++', 'g++'])

default_env.Append(CCFLAGS=['-Wall', '-Wextra', '-pedantic'])
if has_clang(default_env):
    # Only clang supports color.
    default_env.Append(CCFLAGS=['-fcolor-diagnostics'])
default_env.Append(CFLAGS=['-std=c99'])
default_env.Append(CXXFLAGS=['-std=c++11'])


