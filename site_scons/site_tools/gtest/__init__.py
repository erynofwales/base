# gtestprogram.py
# Eryn Wells <eryn@erynwells.me>

import SCons.Util
import dirs


def build_gtest_program(env, target, source=None, *args, **kwargs):
    if not SCons.Util.is_List(source):
        source = [source]
    source.insert(0, dirs.LIB_DIR.Dir('gtest').File('gtest_main.cc'))
    source.append(env['LIBS']['gtest']['static'])
    return env.Program(target, source, *args, **kwargs)


def generate(env):
    print 'gtestprogram generate()'
    try:
        env.AddMethod(build_gtest_program, 'GTestProgram')
    except AttributeError:
        # Old version of SCons
        from SCons.Script.SConscript import SConsEnvironment
        SConsEnvironment.GTestProgram = build_gtest_program


def exists(env):
    print 'gtestprogram exists()'
    return 'gtest' in env['LIBS']
