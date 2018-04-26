#!/bin/sh

lcov -d . -c -o picasso.info
genhtml -o result picasso.info
