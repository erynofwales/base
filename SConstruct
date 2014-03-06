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


#
# BUILD STUFF BELOW HERE
#

import os
import os.path
import SCons.Errors


if not GetOption('build_cmds'):
    def comstr(action):
        return '{:>18}: $TARGET'.format(action)
    default_env = DefaultEnvironment()
    default_env['ARCOMSTR'] = comstr('Archiving')
    default_env['ASCOMSTR'] = comstr('Assembling')
    default_env['ASPPCOMSTR'] = comstr('Assembling')
    default_env['CCCOMSTR'] = comstr('Building (C)')
    default_env['CXXCOMSTR'] = comstr('Building (C++)')
    default_env['LINKCOMSTR'] = comstr('Linking')
    default_env['RANLIBCOMSTR'] = comstr('Indexing')
    default_env['SHCCCOMSTR'] = comstr('Building (C)')
    default_env['SHCXXCOMSTR'] = comstr('Building (C++)')
    default_env['SHLINKCOMSTR'] = comstr('Linking (Shared)')

build_dir = Dir('#build')
lib_dir = Dir('#lib')
src_dir = Dir('#src')
test_dir = Dir('#test')


def create_env(name, src_dirs, appends=None):
    output_dir = build_dir.Dir(name)
    env = DefaultEnvironment().Clone()
    env['__name'] = name
    env['__build_dir'] = output_dir
    env['__src_dirs'] = []
    env['__output_dirs'] = []
    for d in src_dirs:
        out_dir = output_dir.Dir(d.path)
        env['__src_dirs'].append(d)
        env['__output_dirs'].append(out_dir)
        env.VariantDir(out_dir, d.path, duplicate=0)
        env.Clean('.', out_dir)
    if appends:
        for k, v in appends.iteritems():
            if k.startswith('='):
                env[k[1:]] = v
            else:
                env.Append(**{k: v})
    return env


debug_cflags = ' -O0 -g'
debug_env = create_env('debug', [src_dir], {
    'CPPDEFINES': ['DEBUG'],
    'CFLAGS': debug_cflags,
    'CXXFLAGS': debug_cflags,
})

release_cflags = ' -O2'
release_env = create_env('release', [src_dir], {
    'CPPDEFINES': ['RELEASE'],
    'CFLAGS': release_cflags,
    'CXXFLAGS': release_cflags,
})

test_gtest_dir = Dir('#lib/gtest')
test_gtest_include = test_gtest_dir.Dir('include')
test_env = create_env('test', [src_dir, test_dir, test_gtest_dir], {
    'CPPDEFINES': ['DEBUG'],
    'CPPPATH': [test_gtest_include],
    'LIBPATH': [test_gtest_dir],
    'CFLAGS': debug_cflags,
    'CXXFLAGS': debug_cflags,
})


modes = {
    'debug': debug_env,
    'release': release_env,
    'test': test_env,
}

mode = ARGUMENTS.get('MODE', None)
build_modes = []
if mode:
    # If MODE=foo is specified, build only that mode.
    build_modes.append(mode)
else:
    build_modes = modes.keys()

for mode in build_modes:
    try:
        env = modes[mode]
    except KeyError:
        print 'Skipping invalid mode: {}'.format(mode)
    for d in env['__output_dirs']:
        env.SConscript(d.File('SConscript'), {'env': env})
