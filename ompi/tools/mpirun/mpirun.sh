#!/usr/bin/env bash

# Search to see if the unsupported options were used
found=
for token in "$@"; do
    # Heuristic: look for "--" to know when to stop scanning the
    # command line.  This is far from perfect, but hopefully good enough.
    if test "$token" = "--"; then
        break

    # In the future, we need to read ompirun.in or ask ompirun what options
    # it supports exclusively (i.e., option that we don't support here)
    # rather than hard-coding them here.
    elif test "$token" = "--net" -o "$token" = "--tune"; then
        found=$token
    fi
done

if test -n "$found"; then
    cat <<EOF
-----------------------------------------------------------------------
You used an $0 option that is not supported: $found

This option is only supported on systems that have Python v3 or higher.
To fix this issue, please re-install Open MPI with Python v3 (or higher)
available in your PATH.
-----------------------------------------------------------------------
EOF
    exit 1
fi

# No unsupported options were used, so pass all the options down to prte.
# Note the Bash-specific syntax here: "$@" -- all 4 characters are important.
# This preserves tokens that include spaces.  See bash(1) for details.
exec prte "$@"
