# SConstruct
# vim: set ft=python:
#
# Toplevel Scons build script. This should be mostly complete and generic enough
# for most builds.
#
# Eryn Wells <eryn@erynwells.me>


#
# DEFAULT CONFIGURATION VALUES
#

# Settings for the default toolchain binaries. Setting these overrides the
# defaults, provided your the given binary exists.
CC = None
CXX = None
AS = None
LINK = None

# Same as above but for default CFLAGS. These are appended to both CFLAGS and
# CXXFLAGS.
CFLAGS = '-Wall -Wextra -pedantic'

# Enabling debugging does the following things:
#   1. Turns on debugging symbols
#   2. Turns off all optimization
#   3. Sets the DEBUG define
DEBUG = True

# Show build commands ("cc [args] -o [out] [file]", etc). If this is False, show
# some nice messages for each step of the build.
BUILD_CMDS = False

#
# BUILD STUFF BELOW HERE
#

import os
import os.path
import SCons.Errors


def which(program):
    def is_executable(path):
        return os.path.exists(path) and os.access(path, os.X_OK)

    path, name = os.path.split(program)
    if path:
        if is_executable(program):
            return program
    else:
        pathext = [''] + os.environ.get('PATHEXT', '').split(os.pathsep)
        for path in os.environ.get('PATH', '').split(os.pathsep):
            exe = os.path.join(path, program)
            for ext in pathext:
                candidate = exe + ext
                if is_executable(candidate):
                    return candidate
    return None


def get_bool_argument(arg):
    try:
        return bool(int(arg))
    except ValueError:
        pass
    if arg in ('False', 'FALSE', 'false', ''):
        return False
    return True


def set_toolchain_binary(env, var, user_binary, binaries=()):
    if user_binary and which(user_binary):
        env[var] = user_binary
        return
    for c in binaries:
        if which(c):
            env[var] = c
            break

common_env = Environment()
set_toolchain_binary(common_env, 'CC', CC, ('clang', 'gcc'))
set_toolchain_binary(common_env, 'CXX', CXX, ('clang++', 'g++'))
set_toolchain_binary(common_env, 'AS', AS)
set_toolchain_binary(common_env, 'LINK', LINK)
common_env.Append(CFLAGS='{} -std=c99'.format(CFLAGS))
common_env.Append(CXXFLAGS='{} -std=c++11'.format(CFLAGS))

# Add color error messages for clang
if 'clang' in common_env['CC']:
    common_env.Append(CFLAGS=' -fcolor-diagnostics')
if 'clang' in common_env['CXX']:
    common_env.Append(CXXFLAGS=' -fcolor-diagnostics')

BUILD_CMDS = get_bool_argument(ARGUMENTS.get('BUILD_CMDS', BUILD_CMDS))
if not BUILD_CMDS:
    def generate_comstr(action):
        return '{:>25}: $TARGET'.format(action)
    common_env['ASCOMSTR'] = generate_comstr('Assembling')
    common_env['CCCOMSTR'] = generate_comstr('Building (C)')
    common_env['SHCCCOMSTR'] = generate_comstr('Building (C, Shared)')
    common_env['CXXCOMSTR'] = generate_comstr('Building (C++)')
    common_env['SHCXXCOMSTR'] = generate_comstr('Building (C++, Shared)')
    common_env['LINKCOMSTR'] = generate_comstr('Linking')
    common_env['SHLINKCOMSTR'] = generate_comstr('Linking (Shared)')
    common_env['ARCOMSTR'] = generate_comstr('Archiving')
    common_env['RANLIBCOMSTR'] = generate_comstr('Indexing')

src_dir = Dir('#src')


def create_env(variant_dir, appends=None):
    env = common_env.Clone()
    env.VariantDir(variant_dir, src_dir, duplicate=0)
    env.Clean('.', variant_dir)
    for k, v in appends.iteritems():
        if k.startswith('='):
            env[k[1:]] = v
        else:
            env.Append(**{k: v})
    return env


debug_cflags = ' -O0 -g'
debug_env = create_env(os.path.join('build', 'debug'), {
    'CPPDEFINES': ['DEBUG'],
    'CFLAGS': debug_cflags,
    'CXXFLAGS': debug_cflags,
})

release_cflags = ' -O2'
release_env = create_env(os.path.join('build', 'release'), {
    'CPPDEFINES': ['RELEASE'],
    'CFLAGS': release_cflags,
    'CXXFLAGS': release_cflags,
})

modes = {
    'debug': debug_env,
    'release': release_env,
}

mode = ARGUMENTS.get('MODE', None)
if mode:
    # If MODE=foo is specified, build only that mode.
    try:
        env = modes[mode]
    except KeyError:
        raise SCons.Errors.UserError('Invalid mode: {}'.format(mode))
    env.SConscript(os.path.join('build', mode, 'SConscript'), {'env': env})
else:
    # Build all modes.
    for mode, env in modes.iteritems():
        env.SConscript(os.path.join('build', mode, 'SConscript'), {'env': env})
