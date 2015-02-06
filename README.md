# tssyncbytefix

This tiny tool helps to fix Mpeg-TS files which are out of 0x47 sync.
A Mpeg-TS requires an 0x47 byte each 188 bytes. This marks an TS Pkg start.

Some tools relying on the sync byte.

If a files get out of sync by missing some bytes in the middle we can fix it with this tool.

compile: g++ tssyncbytefix.c -o tssyncbytefix

usage:   ./tssyncbytefix test.ts test_sync_fixed.ts

