.. Open MPI documentation, version 5.0.0.

   You need the Sphinx package in order to build this documentation.
   You probably also want a Python virtual environment.  This might be
   a useful command sequence to get you started:

   # Make a virtual environment
   virtualenv venv
   # Activate the virtual environment
   . ./venv/bin/activate
   # Install Sphinx and others
   pip install `cat requirements.txt`
   # Build the HTML docs
   make html
   # Then you can open _build/html/index.html

Open MPI
========

The Open MPI Project is an open source implementation of the `Message
Passing Interface (MPI) specification <https://www.mpi-forum.org/>`_
that is developed and maintained by a consortium of academic,
research, and industry partners.  Open MPI is therefore able to
combine the expertise, technologies, and resources from all across the
High Performance Computing community in order to build the best MPI
library available.  Open MPI offers advantages for system and software
vendors, application developers and computer science researchers.

:ref:`search`

.. toctree::
   :maxdepth: 2
   :numbered:

   quickstart
   getting-help
   general_notes
   installation
   version-numbering
   validate
   extensions
   java
   examples
   faq/index
   developers
   frameworks
   license

How can I be notified of the latest releases?
=============================================

The best way to hear about new Open MPI releases is via the low-volume
announcement mailing list.  `Sign up here
<https://lists.open-mpi.org/mailman/listinfo/announce>`_.

Additionally, if you are a downstream packager of Open MPI (e.g., you
package Open MPI for an operating system distribution), you may wish
to sign up for the low-volume packagers list.  `Sign up here
<https://lists.open-mpi.org/mailman/listinfo/ompi-packagers>`_.

Questions?  Problems?
=====================

Found a bug?  Got a question?  Want to make a suggestion?  Want to
contribute to Open MPI?  Please let us know!

When submitting questions and problems, be sure to include as much
extra information as possible.  `See the community help web page
<https://www.open-mpi.org/community/help/>`_ for details on all the
information that we request in order to provide assistance:

The best way to report bugs, send comments, or ask questions is to
sign up on the user's and/or developer's mailing list:

* User's mailng list: `Sign up here
  <https://lists.open-mpi.org/mailman/listinfo/users>`_.  This list is
  for general user-level questions.
* Developer's mailing list: `Sign up here
  <https://lists.open-mpi.org/mailman/listinfo/devel>`_.: This list is
  for developer-level / internals-of-Open-MPI level questions.

If you don't know which mailing list to send to, send to the user's
list first.

Because of spam, *only subscribers are allowed to post to these
lists*. Ensure that you subscribe with and post from exactly the same
e-mail address -- joe@example.com is considered different than
joe@mycomputer.example.com!

Make today an Open MPI day!
