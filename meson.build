project('geany', 'c', 'cpp', version: '1.32')

cc = meson.get_compiler('c')
cxx = meson.get_compiler('cpp')

deps_in = [
	['glib-2.0',    '2.32'],
	['gmodule-2.0', '2.32'],
	['gtk+-3.0',    '3.0']
]

deps = []
deps_for_pc = ''
foreach dep : deps_in
	deps += [dependency(dep[0], version: '>= ' + dep[1])]
	deps_for_pc += ' ' + dep[0] + ' >= ' + dep[1]
endforeach

glib = deps[0]

def_cflags  = [ '-DHAVE_CONFIG_H=1', '-DGTK' ]
def_cflags += '-Wno-deprecated-declarations'

geany_cflags = def_cflags
have_gcc4_visibility = cc.has_argument('-fvisibility=hidden')
geany_cflags += '-DGEANY_PRIVATE'
if target_machine.system() == 'windows'
  geany_cflags += '-DGEANY_EXPORT_SYMBOL="__declspec(dllexport)"'
elif have_gcc4_visibility
  geany_cflags += '-fvisibility=hidden'
  geany_cflags += '-DGEANY_EXPORT_SYMBOL=__attribute__((visibility("default")))'
endif
geany_cflags += '-DGEANY_API_SYMBOL=GEANY_EXPORT_SYMBOL'
geany_cflags += '-DHAVE_REGCOMP' # Should always be 1, required for CTags.
geany_cflags += '-DHAVE_PLUGINS' # TODO: configurable

cdata = configuration_data()
check_headers = [
  'dlfcn.h',
  'fcntl.h',
  'fnmatch.h',
  'glob.h',
  'inttypes.h',
  'locale.h',
  'memory.h',
  'stdint.h',
  'stdlib.h',
  'strings.h',
  'string.h',
  'sys/stat.h',
  'sys/time.h',
  'sys/types.h',
  'unistd.h',
]

check_functions = [
# check token ('HAVE_BIND_TEXTDOMAIN_CODESET',)
# check token ('HAVE_CFLOCALECOPYCURRENT',)
# check token ('HAVE_CFPREFERENCESCOPYAPPVALUE',)
# check token ('HAVE_CXX11',)
  ['HAVE_DCGETTEXT', 'dcgettext', '#include <libintl.h>'],
  ['HAVE_FGETPOS', 'fgetpos', '#include <stdio.h>'],
# check token ('HAVE_FNMATCH',)
  ['HAVE_FTRUNCATE', 'ftruncate', '#include<unistd.h>'],
# check token ('HAVE_GETTEXT',)
# check token ('HAVE_LC_MESSAGES',)
# check token ('HAVE_MKSTEMP',)
# check token ('HAVE_PLUGINS',)
  ['HAVE_SOCKET', 'socket', '#include<sys/socket.h>'],
  ['HAVE_STRERROR', 'strerror', '#include<string.h>'],
  ['HAVE_STRSTR', 'strstr', '#include<string.h>'],
# check token ('HAVE_VTE',)
]

foreach h : check_headers
  if cc.has_header(h)
    define = 'HAVE_' + h.underscorify().to_upper()
    cdata.set(define, 1)
  endif
endforeach

foreach f : check_functions
  if cc.has_function(f.get(1), prefix : f.get(2))
    cdata.set(f.get(0), 1)
  endif
endforeach

cdata.set_quoted('PACKAGE', 'geany')
cdata.set_quoted('PACKAGE_BUGREPORT', 'https://github.com/geany/geany/issues')
cdata.set_quoted('PACKAGE_NAME','Geany')
cdata.set_quoted('PACKAGE_STRING', 'Geany ' + meson.project_version())
cdata.set_quoted('PACKAGE_VERSION', meson.project_version())
cdata.set('GETTEXT_PACKAGE', 'PACKAGE')
cdata.set('ENABLE_NLS', 1)

prefix = get_option('prefix')
cdata.set_quoted('GEANY_PREFIX', get_option('prefix'))
cdata.set('prefix', get_option('prefix'))
foreach d : [ 'includedir', 'libdir', 'libexecdir', 'datadir', 'localedir' ]
  cdata.set_quoted('GEANY_' + d.underscorify().to_upper(), join_paths(prefix, get_option(d)))
  cdata.set(d, join_paths(prefix, get_option(d)))
endforeach
cdata.set_quoted('GEANY_DOCDIR', join_paths(prefix, get_option('datadir'), 'doc'))
# extra vars for geany.pc
cdata.set('exec_prefix', cdata.get('prefix'))
cdata.set('datarootdir', cdata.get('datadir'))
cdata.set('DEPENDENCIES', deps_for_pc)
cdata.set('VERSION', meson.project_version())

git = find_program('git')
ret = run_command(git, 'rev-parse', '--short', '--revs-only', 'HEAD')
cdata.set_quoted('REVISION', ret.stdout().strip())

configure_file(input : 'config.h.meson',
  output : 'config.h',
  configuration : cdata)

configure_file(input: 'geany.pc.in',
               install: true,
               install_dir: join_paths(prefix, 'lib', 'pkgconfig'),
               output: 'geany.pc',
               configuration: cdata)

ictags_main = include_directories('ctags/main')
itagmanager = include_directories('src/tagmanager')
iscintilla = include_directories('scintilla/include')

scintilla = static_library('scintilla',
    'scintilla/lexers/LexAbaqus.cxx',
    'scintilla/lexers/LexAda.cxx',
    'scintilla/lexers/LexAsm.cxx',
    'scintilla/lexers/LexBash.cxx',
    'scintilla/lexers/LexBasic.cxx',
    'scintilla/lexers/LexBatch.cxx',
    'scintilla/lexers/LexCOBOL.cxx',
    'scintilla/lexers/LexCPP.cxx',
    'scintilla/lexers/LexCSS.cxx',
    'scintilla/lexers/LexCaml.cxx',
    'scintilla/lexers/LexCmake.cxx',
    'scintilla/lexers/LexCoffeeScript.cxx',
    'scintilla/lexers/LexD.cxx',
    'scintilla/lexers/LexDiff.cxx',
    'scintilla/lexers/LexErlang.cxx',
    'scintilla/lexers/LexForth.cxx',
    'scintilla/lexers/LexFortran.cxx',
    'scintilla/lexers/LexHTML.cxx',
    'scintilla/lexers/LexHaskell.cxx',
    'scintilla/lexers/LexLaTeX.cxx',
    'scintilla/lexers/LexLisp.cxx',
    'scintilla/lexers/LexLua.cxx',
    'scintilla/lexers/LexMake.cxx',
    'scintilla/lexers/LexMarkdown.cxx',
    'scintilla/lexers/LexMatlab.cxx',
    'scintilla/lexers/LexNsis.cxx',
    'scintilla/lexers/LexNull.cxx',
    'scintilla/lexers/LexPascal.cxx',
    'scintilla/lexers/LexPerl.cxx',
    'scintilla/lexers/LexPowerShell.cxx',
    'scintilla/lexers/LexProps.cxx',
    'scintilla/lexers/LexPython.cxx',
    'scintilla/lexers/LexPO.cxx',
    'scintilla/lexers/LexR.cxx',
    'scintilla/lexers/LexRuby.cxx',
    'scintilla/lexers/LexRust.cxx',
    'scintilla/lexers/LexSQL.cxx',
    'scintilla/lexers/LexTCL.cxx',
    'scintilla/lexers/LexTxt2tags.cxx',
    'scintilla/lexers/LexVHDL.cxx',
    'scintilla/lexers/LexVerilog.cxx',
    'scintilla/lexers/LexYAML.cxx',
    'scintilla/gtk/Converter.h',
    'scintilla/gtk/PlatGTK.cxx',
    'scintilla/gtk/ScintillaGTK.cxx',
    'scintilla/gtk/ScintillaGTK.h',
    'scintilla/gtk/ScintillaGTKAccessible.cxx',
    'scintilla/gtk/ScintillaGTKAccessible.h',
    'scintilla/gtk/scintilla-marshal.c',
    'scintilla/gtk/scintilla-marshal.h',
    'scintilla/lexlib/Accessor.cxx',
    'scintilla/lexlib/Accessor.h',
    'scintilla/lexlib/CharacterCategory.cxx',
    'scintilla/lexlib/CharacterCategory.h',
    'scintilla/lexlib/CharacterSet.cxx',
    'scintilla/lexlib/CharacterSet.h',
    'scintilla/lexlib/LexAccessor.h',
    'scintilla/lexlib/LexerBase.cxx',
    'scintilla/lexlib/LexerBase.h',
    'scintilla/lexlib/LexerModule.cxx',
    'scintilla/lexlib/LexerModule.h',
    'scintilla/lexlib/LexerNoExceptions.cxx',
    'scintilla/lexlib/LexerNoExceptions.h',
    'scintilla/lexlib/LexerSimple.cxx',
    'scintilla/lexlib/LexerSimple.h',
    'scintilla/lexlib/OptionSet.h',
    'scintilla/lexlib/PropSetSimple.cxx',
    'scintilla/lexlib/PropSetSimple.h',
    'scintilla/lexlib/SparseState.h',
    'scintilla/lexlib/StringCopy.h',
    'scintilla/lexlib/StyleContext.cxx',
    'scintilla/lexlib/StyleContext.h',
    'scintilla/lexlib/SubStyles.h',
    'scintilla/lexlib/WordList.cxx',
    'scintilla/lexlib/WordList.h',
    'scintilla/src/AutoComplete.cxx',
    'scintilla/src/AutoComplete.h',
    'scintilla/src/CallTip.cxx',
    'scintilla/src/CallTip.h',
    'scintilla/src/CaseConvert.cxx',
    'scintilla/src/CaseConvert.h',
    'scintilla/src/CaseFolder.cxx',
    'scintilla/src/CaseFolder.h',
    'scintilla/src/Catalogue.cxx',
    'scintilla/src/Catalogue.h',
    'scintilla/src/CellBuffer.cxx',
    'scintilla/src/CellBuffer.h',
    'scintilla/src/CharClassify.cxx',
    'scintilla/src/CharClassify.h',
    'scintilla/src/ContractionState.cxx',
    'scintilla/src/ContractionState.h',
    'scintilla/src/Decoration.cxx',
    'scintilla/src/Decoration.h',
    'scintilla/src/Document.cxx',
    'scintilla/src/Document.h',
    'scintilla/src/Editor.cxx',
    'scintilla/src/Editor.h',
    'scintilla/src/EditModel.cxx',
    'scintilla/src/EditModel.h',
    'scintilla/src/EditView.cxx',
    'scintilla/src/EditView.h',
    'scintilla/src/ExternalLexer.cxx',
    'scintilla/src/ExternalLexer.h',
    'scintilla/src/FontQuality.h',
    'scintilla/src/Indicator.cxx',
    'scintilla/src/Indicator.h',
    'scintilla/src/KeyMap.cxx',
    'scintilla/src/KeyMap.h',
    'scintilla/src/LineMarker.cxx',
    'scintilla/src/LineMarker.h',
    'scintilla/src/MarginView.cxx',
    'scintilla/src/MarginView.h',
    'scintilla/src/Partitioning.h',
    'scintilla/src/PerLine.cxx',
    'scintilla/src/PerLine.h',
    'scintilla/src/Position.h',
    'scintilla/src/PositionCache.cxx',
    'scintilla/src/PositionCache.h',
    'scintilla/src/RESearch.cxx',
    'scintilla/src/RESearch.h',
    'scintilla/src/RunStyles.cxx',
    'scintilla/src/RunStyles.h',
    'scintilla/src/SVector.h',
    'scintilla/src/ScintillaBase.cxx',
    'scintilla/src/ScintillaBase.h',
    'scintilla/src/Selection.cxx',
    'scintilla/src/Selection.h',
    'scintilla/src/SparseVector.h',
    'scintilla/src/SplitVector.h',
    'scintilla/src/Style.cxx',
    'scintilla/src/Style.h',
    'scintilla/src/UniConversion.cxx',
    'scintilla/src/UniConversion.h',
    'scintilla/src/UnicodeFromUTF8.h',
    'scintilla/src/UniqueString.h',
    'scintilla/src/ViewStyle.cxx',
    'scintilla/src/ViewStyle.h',
    'scintilla/src/XPM.cxx',
    'scintilla/src/XPM.h',
    cpp_args: geany_cflags + [ '-Wno-non-virtual-dtor', '-DNDEBUG', '-DSCI_LEXER', '-DNO_CXX11_REGEX' ],
    dependencies: deps,
    include_directories: [iscintilla,
                          include_directories('scintilla/include',
                                              'scintilla/src',
                                              'scintilla/lexlib')]
)
dep_scintilla = declare_dependency(link_with: scintilla,
                                   include_directories: include_directories('scintilla/include'))

ctags_parsers = static_library('ctags_parsers',
    'ctags/parsers/abaqus.c',
    'ctags/parsers/abc.c',
    'ctags/parsers/actionscript.c',
    'ctags/parsers/asciidoc.c',
    'ctags/parsers/asm.c',
    'ctags/parsers/basic.c',
    'ctags/parsers/c.c',
    'ctags/parsers/cobol.c',
    'ctags/parsers/conf.c',
    'ctags/parsers/css.c',
    'ctags/parsers/diff.c',
    'ctags/parsers/docbook.c',
    'ctags/parsers/erlang.c',
    'ctags/parsers/fortran.c',
    'ctags/parsers/go.c',
    'ctags/parsers/haskell.c',
    'ctags/parsers/haxe.c',
    'ctags/parsers/html.c',
    'ctags/parsers/jscript.c',
    'ctags/parsers/json.c',
    'ctags/parsers/latex.c',
    'ctags/parsers/lua.c',
    'ctags/parsers/make.c',
    'ctags/parsers/markdown.c',
    'ctags/parsers/matlab.c',
    'ctags/parsers/nsis.c',
    'ctags/parsers/objc.c',
    'ctags/parsers/pascal.c',
    'ctags/parsers/perl.c',
    'ctags/parsers/php.c',
    'ctags/parsers/powershell.c',
    'ctags/parsers/python.c',
    'ctags/parsers/r.c',
    'ctags/parsers/rest.c',
    'ctags/parsers/ruby.c',
    'ctags/parsers/rust.c',
    'ctags/parsers/sh.c',
    'ctags/parsers/sql.c',
    'ctags/parsers/tcl.c',
    'ctags/parsers/txt2tags.c',
    'ctags/parsers/verilog.c',
    'ctags/parsers/vhdl.c',
    include_directories: [ictags_main],
    c_args: geany_cflags,
)

ctags = static_library('ctags',
    'ctags/main/args.c',
    'ctags/main/entry.c',
    'ctags/main/keyword.c',
    'ctags/main/lcpp.c',
    'ctags/main/lregex.c',
    'ctags/main/main.c',
    'ctags/main/mio.c',
    'ctags/main/nestlevel.c',
    'ctags/main/options.c',
    'ctags/main/parse.c',
    'ctags/main/read.c',
    'ctags/main/routines.c',
    'ctags/main/sort.c',
    'ctags/main/strlist.c',
    'ctags/main/vstring.c',
    'ctags/main/xtag.c',
    c_args: geany_cflags,
    dependencies: [glib]
)
dep_ctags = declare_dependency(link_with: [ctags, ctags_parsers], include_directories: [ictags_main])

tagmanager = static_library('tagmanager',
    'src/tagmanager/tm_ctags_wrappers.c',
    'src/tagmanager/tm_tag.c',
    'src/tagmanager/tm_parser.c',
    'src/tagmanager/tm_workspace.c',
    'src/tagmanager/tm_source_file.c',
    c_args: geany_cflags,
    dependencies: [dep_ctags, glib])
dep_tagmanager = declare_dependency(link_with: tagmanager, include_directories: [itagmanager])

# Generate signallist.i
sh = find_program('sh')
gen_src = custom_target('gen-signallist',
                        input : [ 'scripts/gen-signallist.sh', 'data/geany.glade' ],
                        output : [ 'signallist.i' ],
                        command : [sh, '@INPUT0@', '@INPUT1@', '@OUTPUT@' ])

libgeany = shared_library('geany',
    'src/about.c',
    'src/build.c',
    'src/callbacks.c',
    'src/dialogs.c',
    'src/document.c',
    'src/editor.c',
    'src/encodings.c',
    'src/filetypes.c',
    'src/gb.c',
    'src/geanyentryaction.c',
    'src/geanymenubuttonaction.c',
    'src/geanyobject.c',
    'src/geanywraplabel.c',
    'src/highlighting.c',
    'src/keybindings.c',
    'src/keyfile.c',
    'src/libmain.c',
    'src/log.c',
    'src/main.c',
    'src/msgwindow.c',
    'src/navqueue.c',
    'src/notebook.c',
    'src/osx.c',
    'src/plugins.c',
    'src/pluginutils.c',
    'src/prefix.c',
    'src/prefs.c',
    'src/printing.c',
    'src/project.c',
    'src/sciwrappers.c',
    'src/search.c',
    'src/sidebar.c',
    'src/socket.c',
    'src/spawn.c',
    'src/stash.c',
    'src/symbols.c',
    'src/templates.c',
    'src/toolbar.c',
    'src/tools.c',
    'src/ui_utils.c',
    'src/utils.c',
    'src/vte.c',
    'src/win32.c',
    gen_src,
    soversion: '0',
    c_args: geany_cflags,
    include_directories: [iscintilla],
    dependencies: [dep_tagmanager, dep_ctags, dep_scintilla] + deps,
    install: true)

executable('geany', 'src/main.c',
            link_with: libgeany,
            c_args: geany_cflags,
            dependencies: [dep_ctags] + deps,
            build_rpath: meson.build_root(),
            install_rpath: join_paths(prefix, 'lib'),
            install: true)


i18n = import('i18n')

i18n.merge_file(
  input: 'geany.desktop.in',
  output: 'geany.desktop',
  type: 'desktop',
  po_dir: 'po',
  install: true,
  install_dir: join_paths(get_option('datadir'), 'applications')
)

subdir('po')
subdir('data')
subdir('plugins')

sh = find_program('sh')
extra_data = [['COPYING', 'GPL-2']]
if host_machine.system() == 'windows'
extra_data += [['AUTHORS', 'Authors.txt']]
extra_data += [['ChangeLog', 'Changelog.txt']]
extra_data += [['COPYING', 'Copying.txt']]
extra_data += [['README', 'Readme.txt']]
extra_data += [['NEWS', 'News.txt']]
extra_data += [['THANKS', 'Thanks.txt']]
extra_data += [['TODO', 'Todo.txt']]
endif
message(extra_data)
foreach file : extra_data
    custom_target('gen-'+ file[0].to_lower(),
                  input: file[0],
                  output: file[1],
                  command: [ 'cp', '@INPUT@', '@OUTPUT@' ],
                  install: true,
                  install_dir: join_paths(get_option('datadir'), 'geany'))
endforeach
geany_cflags += '-DGTK'
