#!/usr/bin/env bash

# Make sure we have clang-format v11
check() {
    which $1 2>&1 > /dev/null
    if test $? -eq 0; then
        version=$($1 --version | grep "version 11")
        if test -n "$version"; then
            CF=$1
        fi
    fi
}

check clang-format
if test -z "$CF"; then
    check clang-format-11
fi
if test -z "$CF"; then
    # This is a GitHub Action-formatted error message
    echo "::error :: Cannot find clang-format v11"
    exit 1
fi

#####################################

files=($(git ls-tree -r HEAD --name-only | grep -v '3rd-party/' | grep -v 'contrib' | grep -v 'ompi/mpi/fortran' | grep -e '.*\.[ch]$'))

# JMS limited set for testing
files=($(git ls-tree -r HEAD --name-only | grep 'opal/util' | grep -e '.*\.[ch]$'))

st=0
for file in "${files[@]}" ; do
    # Only show the ouptut if there is an error.  Otherwise, we'll show output # for
    # every single file in the tree, which will be overwhelming. # clang-format
    # writes directly to the tty, so we can't use regular redirection -- must use
    # "script" instead.  :-(
    #
    # NOTE: This is Linux-specific!  The MacOS "script" uses different options.
    script --return --quiet --command "$CF --style=file --Werror --dry-run ${file}" tmp.txt > /dev/null
    if test $? -ne 0; then
        st=1
        echo "::error :: clang-format failed for ${file}"
        echo "::group::{${file}}"
        cat tmp.txt
        echo "::endgroup::"
    fi
    rm -f tmp.txt
done

#####################################

if test "$st" -ne 0; then
    echo "::error :: clang-format CI failed"
else
    echo "clang-format CI succeeded -- huzzah!"
fi
exit $st
