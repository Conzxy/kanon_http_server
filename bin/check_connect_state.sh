#!/bin/bash
awk -f connected.awk $1/*> connected && awk -f disconn.awk $1/*> disconnected
icdiff connected disconnected
rm connected disconnected
