ret = run_command('sh', '-c', 'ls *.po 2>/dev/null | gawk \'BEGIN { FS="."; ORS=" " } { print $1 }\'')
if ret.returncode() == 0
    langs = ret.stdout().split()
    i18n.gettext('geany', languages: langs)
else
    error(ret.stderr())
endif
