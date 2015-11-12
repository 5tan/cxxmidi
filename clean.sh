#!/bin/bash

find . -name "*.pro.user" -print0 | xargs -0 rm -rfv
find . -name "build-*" -print0 | xargs -0 rm -rfv
find . -name "*~" -print0 | xargs -0 rm -rfv
find . -name ".*.swp" -print0 | xargs -0 rm -rfv
