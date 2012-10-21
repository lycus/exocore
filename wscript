#!/usr/bin/env python

import os, shutil, subprocess, sys, tarfile, tempfile
from waflib import Build, Context, Scripting, Utils, Task, TaskGen
from waflib.Tools import ccroot

APPNAME = 'ExoCore'
VERSION = '1.0'

TOP = os.curdir
OUT = 'build'

def options(opt):
    opt.add_option('--mode', action = 'store', default = 'debug', help = 'the mode to compile in (debug/release)')

class kernel(ccroot.link_task):
    "Link object files into a kernel binary"

    run_str = '${LD} ${KERNLINKFLAGS} -o ${TGT} ${SRC}'
    ext_out = ['.bin']
    inst_to = None

    def post_run(self):
        Task.Task.post_run(self)

        shutil.copy(self.outputs[0].abspath(), os.path.join(TOP, 'iso', 'boot'))

@TaskGen.feature('iso')
@TaskGen.after_method('kernel')
def iso(self):
    kernel_output = self.link_task.outputs[0]

    self.iso_task = self.create_task('iso',
                                     src = kernel_output,
                                     tgt = self.path.find_or_declare(kernel_output.change_ext('.iso').name))

class iso(Task.Task):
    "Build a bootable ISO for GRUB 2"

    run_str = '${MKRESCUE} -o ${TGT} ${SRC} ${ISOFLAGS} -- -report-about SORRY > /dev/null 2>&1'
    ext_out = ['.iso']
    inst_to = None
    color = 'PINK'

@TaskGen.feature('syms')
@TaskGen.after_method('kernel')
def syms(self):
    kernel_output = self.link_task.outputs[0]

    self.syms_task = self.create_task('syms',
                                      src = kernel_output,
                                      tgt = self.path.find_or_declare(kernel_output.change_ext('.sym').name))

class syms(Task.Task):
    "Build symbol list for a binary"

    run_str = '${NM} -n ${SRC} | ${GREP} -v \'\( [aUw] \)\|\(__crc_\)\|\( \$[adt]\)\' | ${AWK} \'{print $1, $3}\' > ${TGT}'
    ext_out = ['.sym']
    inst_to = None
    color = 'CYAN'

def configure(conf):
    def add_options(flags, options):
        for option in options:
            if option not in conf.env[flags]:
                conf.env.append_value(flags, option)

    conf.load('ar')
    conf.find_program('clang', var = 'CC', mandatory = True)
    conf.load('gcc')

    conf.check_cc(fragment = r'''int main()
                                 {
                                     #if __clang_major__ < 3 || (__clang_major__ == 3 && __clang_minor__ < 1)
                                     #    error Only Clang versions above 3.1 are supported.
                                     #endif

                                     return 0;
                                 }''',
                  features = 'c',
                  mandatory = True,
                  execute = False,
                  msg = 'Checking for clang 3.1+')

    add_options('CFLAGS',
                ['-m64'])

    add_options('CFLAGS',
                ['-std=gnu11',
                 '-fms-extensions',
                 '-fborland-extensions',
                 '-fblocks'])

    add_options('CFLAGS',
                ['-Wall',
                 '-Wextra',
                 '-Werror',
                 '-Winit-self',
                 '-Wswitch-default',
                 '-Wswitch-enum',
                 '-Wundef',
                 '-Wshadow',
                 '-Wpointer-arith',
                 '-Wbad-function-cast',
                 '-Wcast-qual',
                 '-Wwrite-strings',
                 '-Wconversion',
                 '-Wstrict-prototypes',
                 '-Wold-style-definition',
                 '-Wmissing-declarations',
                 '-Wmissing-format-attribute',
                 '-Wpadded',
                 '-Wredundant-decls',
                 '-Winline'])

    add_options('CFLAGS',
                ['-nostdlib',
                 '-nostdinc',
                 '-ffreestanding'])

    add_options('CFLAGS',
                ['-fwrapv',
                 '-fno-omit-frame-pointer',
                 '-mcmodel=large'])

    conf.find_program('yasm', var = 'AS', mandatory = True)
    conf.load('nasm')

    add_options('ASFLAGS',
                ['-f',
                 'elf64'])

    add_options('ASFLAGS',
                ['-w',
                 '-Worphan-labels',
                 '-Wunrecognized-char'])

    conf.env.kernel_PATTERN = '%s.bin'

    conf.find_program('ld', var = 'LD', mandatory = True)

    add_options('KERNLINKFLAGS',
                ['-b',
                 'elf64-x86-64',
                 '-m',
                 'elf_x86_64'])

    add_options('KERNLINKFLAGS',
                ['-z',
                 'max-page-size=0x1000'])

    add_options('KERNLINKFLAGS',
                ['-T',
                 os.path.abspath(os.path.join('linker', 'x86_64.ld'))])

    conf.find_program('nm', var = 'NM', mandatory = True)
    conf.find_program('grep', var = 'GREP', mandatory = True)
    conf.find_program('awk', var = 'AWK', mandatory = True)

    conf.find_program(['grub-mkrescue', 'grub2-mkrescue'], var = 'MKRESCUE', mandatory = True)

    add_options('ISOFLAGS',
                [os.path.abspath('iso')])

    if conf.options.mode == 'debug':
        add_options('CFLAGS',
                    ['-DEXOCORE_DEBUG'])

        add_options('CFLAGS',
                    ['-fcatch-undefined-behavior'])

        add_options('CFLAGS',
                    ['-g',
                     '-ggdb'])

        add_options('ASFLAGS',
                    ['-DEXOCORE_DEBUG'])
    elif conf.options.mode == 'release':
        add_options('CFLAGS',
                    ['-O3'])
    else:
        conf.fatal('--mode must be either debug or release.')

def build(bld):
    def search_paths(*k, **kw):
        path = os.path.join(*k, **kw)

        return [os.path.join(path, '*.c'), os.path.join(path, 'asm', '*.s')]

    bld(features = 'asm c kernel syms iso',
        includes = 'include',
        source = bld.path.ant_glob(search_paths(os.path.join('src', 'exocore'))),
        target = 'exocore')

def _run_shell(dir, ctx, args, wait = True):
    cwd = os.getcwd()
    os.chdir(dir)

    proc = subprocess.Popen(args, shell = True)

    if wait:
        code = proc.wait()

        if code != 0:
            ctx.fatal(str(args) + ' exited with: ' + str(code))

    os.chdir(cwd)

def docs(ctx):
    '''builds the documentation'''

    def build_docs(targets):
        for x in targets:
            _run_shell('docs', ctx, 'make ' + x)

    build_docs(['html',
                'dirhtml',
                'singlehtml',
                'text',
                'man',
                'changes',
                'linkcheck'])

def dist(dst):
    '''makes a tarball for redistributing the sources'''

    with open('.gitignore', 'r') as f:
        dst.excl = ' '.join(l.strip() for l in f if l.strip())

def qemu(ctx):
    '''runs the kernel in QEMU with GDB server at localhost:1234'''

    _run_shell(TOP, ctx, 'qemu-system-x86_64 -monitor stdio -S -s -cdrom {0}'.format(os.path.join(OUT, 'exocore.iso')))

class QEMUContext(Build.BuildContext):
    cmd = 'qemu'
    fun = 'qemu'

def bochs(ctx):
    '''runs the kernel in Bochs with GDB server at localhost:1234'''

    _run_shell(TOP, ctx, 'bochs -q')

class BochsContext(Build.BuildContext):
    cmd = 'bochs'
    fun = 'bochs'
