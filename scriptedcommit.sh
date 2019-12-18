#!/bin/bash
make maintainer-clean
git add -A
git commit -m $1
git push
