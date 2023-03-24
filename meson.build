project('edit', 'c',
	version : '0.1',
	license : 'GPL-3.0-only',
	meson_version : '>=0.60.0')

if build_machine.system() != 'linux'
	error('This project only builds on linux')
endif

release_args = ['-DNDEBUG', '-O1', '-std=c99']

compiler_id = meson.get_compiler('c').get_id()
if compiler_id == 'gcc'
	debug_args = ['-DDEBUG', '-Wall', '-Wextra', '-g', '-O0', '-std=c99', '-Wpedantic',
	'-Wshadow', '-Wvla', '-Wpointer-arith', '-Wwrite-strings', '-Wfloat-equal',
	'-Wcast-align', '-Wcast-qual', '-Wbad-function-cast', '-Wstrict-overflow=4',
	'-Wunreachable-code', '-Wformat=2', '-Wundef', '-Wstrict-prototypes',
	'-Wold-style-definition', '-Wconversion', '-Wshift-overflow=2', '-Wredundant-decls',
	'-Wnested-externs', '-Wmissing-include-dirs', '-Wlogical-op', '-Wcast-align=strict',
	'-Wduplicated-cond', '-Wjump-misses-init', '-Wstringop-overflow']
elif compiler_id == 'clang'
	debug_args = ['-DDEBUG', '-Wall', '-Wextra', '-g', '-O0', '-std=c99', '-Wpedantic',
	'-Wshadow', '-Wvla', '-Wpointer-arith', '-Wwrite-strings', '-Wfloat-equal', '-Wcast-align',
	'-Wcast-qual', '-Wbad-function-cast', '-Wstrict-overflow=4', '-Wunreachable-code',
	'-Wformat=2', '-Wundef', '-Wstrict-prototypes', '-Wold-style-definition', '-Wconversion',
	'-Wshift-overflow', '-Wredundant-decls', '-Wnested-externs', '-Wmissing-include-dirs']
else
	error('Unknown compiler (GCC or Clang are didn\'t detected). Unable to build')
endif

build_type = get_option('buildtype')
if build_type == 'debug'
	args = debug_args
elif build_type == 'release'
	args = release_args
endif

incdir = include_directories('include')
src = ['src/buffer.c', 'src/parser.c', 'src/exec.c', 'src/io.c', 'src/main.c', 'src/misc.c']

executable('edit',
	sources : src,
	include_directories : incdir,
	c_args : args,
	install : true)
