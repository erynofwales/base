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
# Environment Configuration
#

def has_clang(env):
    _, cc_tail = os.path.split(env['CC'])
    _, cxx_tail = os.path.split(env['CXX'])
    return any([cc_tail.startswith('clang'), cxx_tail.startswith('clang')])

default_env = SCons.Defaults.DefaultEnvironment()
default_env.Replace(CC=binaries.first(['clang', 'gcc']),
                    CXX=binaries.first(['clang++', 'g++']))

default_env.Append(CCFLAGS=['-Wall', '-Wextra', '-pedantic'],
                   CFLAGS=['-std=c99'],
                   CXXFLAGS=['-std=c++11'])
if has_clang(default_env):
    # Only clang supports color.
    default_env.Append(CCFLAGS=['-fcolor-diagnostics'])


debug_env = default_env.Clone(MODE='debug',
                              CCFLAGS=['-O0', '-g'],
                              CPPDEFINES=['DEBUG'])
release_env = default_env.Clone(MODE='release',
                                CCFLAGS=['-O2'],
                                CPPDEFINES=['RELEASE'])

MODES = {
    'debug': debug_env,
    'release': release_env
}

#
# Command Line Options
#

def process_modes_option(option, opt, value, parser):
    modes = value.split(',')
    for m in modes:
        setattr(parser.values, 'modes', set(modes))

AddOption('--show-build-cmds',
          dest='build_cmds',
          action='store_true',
          help='Show build commands instead of friendly build messages')
AddOption('--modes',
          type='string',
          action='callback',
          dest='modes',
          metavar='MODES',
          default=set(['debug']),
          callback=process_modes_option,
          help=('A comma separated list of modes. Choose from: {}. Default is '
                'debug.').format(', '.join(MODES.keys())))
