# vim: ft=python
env = DefaultEnvironment()

env.Append(
    CPPFLAGS = [
        '-std=c++17',
        '-Wall',
        '-Wextra',
    ],
)

build_vars = Variables()
build_vars.Add(
    EnumVariable(
        'mode',
        allowed_values=['dbg', 'opt'],
        default='dbg',
        help='Build mode: opt or dbg'
    )
)
build_vars.Update(env)
Help(build_vars.GenerateHelpText(env))


if env['mode'] == 'opt':
    env.Append(CPPFLAGS = ['-O2'])
    build_dir = 'build/opt'
else:
    env.Append(CPPFLAGS = ['-g'], CPPDEFINES = ['DEBUG'])
    build_dir = 'build/dbg'


# Out-of-source build:
VariantDir(build_dir, 'src')

# All includes are relative to this src_root:
src_root = Dir(build_dir)
env.Replace(CPPPATH=[src_root])


# Helper to sanely specify static lib dependencies, e.g.:
#
# In src/common/foo/SConscript you might have:
#   env.Library(
#       target='my_lib',
#       sources=['impl.cpp'],
#   )
#
# In src/bar/SConscript you might have:
#   env.Program(
#       target='my_binary',
#       sources=[
#           'main.cpp',
#           Lib('/common/foo:my_lib'),
#       ],
#   )
# 
# It will eval the Lib(...) into {build_dir}/common/foo/libmy_lib.a,
# which SCons will correctly recognize as output of the corresponding
# env.Library (i.e. 'my_lib' here) and will add the latter as a dependency.
def Lib(path):
    if path.startswith('/'):
        # "Absolute" libary path, i.e. relative to codebase root:
        path = path[1:]
        basedir, lib = path.split(':')
        return '#{}/{}/lib{}.a'.format(src_root.path, basedir, lib)
    elif path.startswith(':'):
        # Library defined in same SConscript file (i.e. "local")
        path = path[1:]
        return 'lib{}.a'.format(path)
    else:
        raise "Library path must be ':foo', or '/foo:bar', '/foo/bar:baz', etc"

Export('Lib')

# Find and parse all SConscript files
target_files = Glob(build_dir + '/**/SConscript')
print "+++ Found {} SConscript files".format(len(target_files))

for f in sorted(target_files):
    print "+++    ", f
    SConscript(f, exports='env')
