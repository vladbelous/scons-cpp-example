# vim: ft=python
env = DefaultEnvironment()

# Useful warning flags:
gcc_flags = [
    '-Wnon-virtual-dtor',
    '-Woverloaded-virtual',
    '-Wcast-align',
    '-Wsign-conversion',
    '-Wnull-dereference',
    '-Wlogical-op',
    '-Wduplicated-cond',
    '-Wmisleading-indentation',
    '-Wuseless-cast',
    #'-Wreturn-std-move',
    #'-Wlifetime',
]
clang_flags = [
    '-Wnon-virtual-dtor',
    '-Woverloaded-virtual',
    '-Wcast-align',
    '-Wsign-conversion',
    '-Wnull-dereference',
    #'-Wlogical-op',
    #'-Wduplicated-cond',
    #'-Wmisleading-indentation',
    #'-Wuseless-cast',
    '-Wreturn-std-move',
    #'-Wlifetime',
]

env.Append(
    CPPFLAGS = [
        '-std=c++17',

        '-pedantic',
        '-Wall',
        '-Wextra',

        '-flto',
    ],
    LINKFLAGS = [
        '-flto',
    ],
)

build_vars = Variables()
build_vars.Add(
    EnumVariable(
        'mode',
        allowed_values=['dbg', 'opt', 'dbg-clang', 'opt-clang'],
        default='dbg',
        help='Build mode to use (debug/optimized, gcc/clang)'
    )
)
build_vars.Update(env)
Help(build_vars.GenerateHelpText(env))


if env['mode'] == 'opt':
    env.Append(CPPFLAGS = ['-O2'])
    env.Append(CPPFLAGS = gcc_flags)
elif env['mode'] == 'opt-clang':
    env.Replace(CXX = 'clang++')
    env.Append(CPPFLAGS = ['-O2'])
    env.Append(CPPFLAGS = clang_flags)
elif env['mode'] == 'dbg':
    env.Append(CPPFLAGS = ['-g'], CPPDEFINES = ['DEBUG'])
    env.Append(CPPFLAGS = gcc_flags)
elif env['mode'] == 'dbg-clang':
    env.Replace(CXX = 'clang++')
    env.Append(CPPFLAGS = ['-g'], CPPDEFINES = ['DEBUG'])
    env.Append(CPPFLAGS = clang_flags)
else:
    raise "Unexpected build mode"

build_dir = 'build/' + env['mode']


# Create in-source clang-tidy phony target, invoked as:
# `scons clang-tidy`
env.AlwaysBuild(env.Alias(
    target='clang-tidy',
    sources=[],
    action="clang-tidy "
        "-header-filter=.* "
        "-checks=-*,clang-analyzer*,misc*,performance*,modernize*,"
            "bugprone*,portability*,readability* "
        "$$(git ls-files | grep 'cpp$$') -- -Isrc",
))


# Out-of-source build:
VariantDir(build_dir, 'src')

# All includes are relative to this "codebase root":
env.Replace(CPPPATH=[Dir(build_dir)])  # Better than '#src'

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
#           Lib('//common/foo:my_lib'),
#       ],
#   )
# 
# It will eval the Lib(...) into {build_dir}/common/foo/libmy_lib.a,
# which SCons will correctly recognize as output of the corresponding
# env.Library (i.e. 'my_lib' here) and will add the latter as a dependency.
def Lib(path):
    if path.startswith('//'):
        # "Absolute" libary path, i.e. relative to codebase root:
        path = path[2:]
        basedir, lib = path.split(':')
        return '#{}/{}/lib{}.a'.format(build_dir, basedir, lib)
    elif path.startswith(':'):
        # Library defined in same SConscript file (i.e. "local")
        path = path[1:]
        return 'lib{}.a'.format(path)
    else:
        raise "Library path must be ':foo', or '//foo:bar', '//foo/bar:baz', etc"

Export('Lib')

# Find and parse all SConscript files
target_files = Glob(build_dir + '/**/SConscript')
print "+++ Found {} SConscript files".format(len(target_files))

for f in sorted(target_files):
    print "+++    ", f
    targets = SConscript(f, exports='env')

    # Make only explicitly returned targets part of default
    if targets is not None and len(targets) > 0:
        env.Default(targets)
