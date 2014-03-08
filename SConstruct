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


#test_gtest_dir = Dir('#lib/gtest')
#test_gtest_include = test_gtest_dir.Dir('include')
#test_env = create_env('test', [src_dir, test_dir, test_gtest_dir], {
#    'CPPDEFINES': ['DEBUG'],
#    'CPPPATH': [test_gtest_include],
#    'LIBPATH': [test_gtest_dir],
#    'CFLAGS': debug_cflags,
#    'CXXFLAGS': debug_cflags,
#})


for mode in GetOption('modes'):
    try:
        env = MODES[mode]
    except KeyError:
        print 'Skipping invalid mode: {}'.format(mode)

    # Process libraries
    env.SConscript(LIB_DIR.File('SConscript'), {
        'env': env,
    }, variant_dir=BUILD_DIR.Dir(env['MODE']).Dir('lib'), duplicate=0)

    # Process source
    library, binary = env.SConscript(SRC_DIR.File('SConscript'), {
        'env': env
    }, variant_dir=BUILD_DIR.Dir(env['MODE']).Dir('src'), duplicate=0)
    env.Alias('lib', library)
    env.Alias('bin', binary)

    env.SConscript(TEST_DIR.File('SConscript'), {
        'env': env,
    }, variant_dir=BUILD_DIR.Dir(env['MODE']).Dir('test'), duplicate=0)

Import('LIBS')
print LIBS
