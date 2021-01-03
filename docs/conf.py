# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- Project information -----------------------------------------------------

project = 'Open MPI'
copyright = '2020, The Open MPI Community'
author = 'The Open MPI Community'

# The full version, including alpha/beta/rc tags
# Open MPI: we'll see this value below, in the Open MPI-specific section.
#release = 'v5.0.0'


# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ['recommonmark']

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store', 'venv', 'py*/**']


# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
# Open MPI: comment this out so that we use the default readthedocs theme.
#html_theme = 'alabaster'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']


# -- Open MPI-specific options -----------------------------------------------

# This prolog is included in every file.  Put common stuff here.

# Read the Open MPI version from the VERSION file
with open("../VERSION") as fp:
    ompi_lines = fp.readlines()

ompi_data = dict()
for ompi_line in ompi_lines:
    if '#' in ompi_line:
        ompi_line, _ = ompi_line.split("#")
    ompi_line = ompi_line.strip()

    if '=' not in ompi_line:
        continue

    ompi_key, ompi_val = ompi_line.split("=")
    ompi_data[ompi_key.strip()] = ompi_val.strip()

ompi_ver = f"{ompi_data['major']}.{ompi_data['minor']}.{ompi_data['release']}{ompi_data['greek']}"

# Release is a sphinx variable
release = ompi_ver

rst_prolog = f"""
.. |mdash|  unicode:: U+02014 .. Em dash
.. |rarrow| unicode:: U+02192 .. Right arrow
.. |ompi_ver| replace:: {ompi_ver}
"""
