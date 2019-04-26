#!/bin/bash
if [ -f .sconsign.dblite ]; then
  rm .sconsign.dblite
fi
rm -rf build/

scons $@
