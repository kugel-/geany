geany_datadir = join_paths(get_option('datadir'), 'geany')

install_subdir('filedefs',     install_dir: geany_datadir)
install_subdir('templates',    install_dir: geany_datadir)
install_subdir('tags',         install_dir: geany_datadir)
install_subdir('colorschemes', install_dir: geany_datadir)

misc = [
    'filetype_extensions.conf',
    'snippets.conf',
    'ui_toolbar.xml',
    'geany.glade',
    'geany-3.0.css',
    'geany-3.20.css',
    'geany.css',
]
install_data(sources: misc, install_dir: geany_datadir)
