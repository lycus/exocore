import sys, os

extensions = ['sphinx.ext.todo']

templates_path = ['_templates']
exclude_patterns = ['_build']

project = u'ExoCore Operating System Kernel'
copyright = u'The Lycus Foundation'

version = '1.0'
release = '1.0'

source_suffix = '.rst'
master_doc = 'index'

pygments_style = 'sphinx'
highlight_language = 'd'

html_theme = 'nature'
html_static_path = ['_static']
htmlhelp_basename = 'ExoCoredoc'

latex_documents = [('index', 'ExoCore.tex', u'ExoCore Operating System Kernel', u'The Lycus Foundation', 'manual')]

man_pages = [('index', 'exocore', u'ExoCore Operating System Kernel', [u'The Lycus Foundation'], 1)]

epub_title = u'ExoCore Operating System Kernel'
epub_author = u'The Lycus Foundation'
epub_publisher = u'The Lycus Foundation'
epub_copyright = u'The Lycus Foundation'
