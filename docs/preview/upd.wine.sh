#!/bin/bash
# -*- coding: utf-8, tab-width: 2 -*-
#
# Using wine and the win32 version as fallback in case of
# https://github.com/chgibb/NwSJS/issues/14

NWSJS_CMD=nwsjs
<<<';' $NWSJS_CMD &>/dev/null
[ $? == 126 ] && NWSJS_CMD="wine $(which nwsjs.exe)"

for FN in ../../tests/*.[tj]s; do
  $NWSJS_CMD "$FN" >nws."$(basename "$FN")"
done
