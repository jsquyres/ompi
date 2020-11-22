.. Open MPI documentation, version 5.0.0.
   .
   You need the Sphinx package in order to build this documentation.
   You probably also want a Python virtual environment.  This might be
   a useful command sequence to get you started:
   .
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

Questions?  Problems?
=====================

Found a bug?  Got a question?  Want to make a suggestion?  Want to
contribute to Open MPI?  Please let us know!

When submitting questions and problems, be sure to include as much
extra information as possible.  `See the community help web page
<https://www.open-mpi.org/community/help/>`_ for details on all the
information that we request in order to provide assistance:

The best way to report bugs, send comments, or ask questions is to
sign up on the user's and/or developer's mailing list (for user-level
and developer-level questions; when in doubt, send to the user's
list):

* users@lists.open-mpi.org
* devel@lists.open-mpi.org

Because of spam, only subscribers are allowed to post to these lists
(ensure that you subscribe with and post from exactly the same e-mail
address -- joe@example.com is considered different than
joe@mycomputer.example.com!).  Visit these pages to subscribe to the
lists:

* `Subscribe to the users mailing list
  <https://lists.open-mpi.org/mailman/listinfo/users>`_
* `Subscribe to the developers mailing list
  <https://lists.open-mpi.org/mailman/listinfo/devel>`_

Make today an Open MPI day!
