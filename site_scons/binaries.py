# files.py
#
# Utilities for working with files and paths in SCons.
#
# Eryn Wells <eryn@erynwells.me>

import os


def which(program):
    '''
    Given a program name, search the system environment's $PATH for a binary of
    that name. If one exists, return its name. If not, return None. This
    function will also use the system environment's $PATHEXT to find binaries
    with appropriate extensions (i.e., .exe on Windows).
    '''
    is_executable = lambda path: (    os.path.exists(path)
                                  and os.access(path, os.X_OK))

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


def first(binaries):
    '''
    Given a list of binaries, return the first one found.
    '''
    for binary in binaries:
        if which(binary):
            return binary
    return None
