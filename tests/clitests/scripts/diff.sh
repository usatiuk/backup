#!/bin/bash

# TODO: Don't hardcode this
CMD="../../src/backup"

TESTDATADIR=diff

rm -rf "$TESTDATADIR"

mkdir -p "$TESTDATADIR"/testdir
mkdir -p "$TESTDATADIR"/testdata/1
mkdir -p "$TESTDATADIR"/testdata/1/empty
mkdir -p "$TESTDATADIR"/testdata/1/notempty

mkdir -p "$TESTDATADIR"/testdata/1/emptyi
mkdir -p "$TESTDATADIR"/testdata/1/notemptyi

mkdir -p "$TESTDATADIR"/testdata/1/notempty2/empty
mkdir -p "$TESTDATADIR"/testdata/1/notempty2/notemptyi2
mkdir -p "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/ignoredir

echo "testtestasdf1" > "$TESTDATADIR"/testdata/1/notempty/testfile
echo "testtestasdf2" > "$TESTDATADIR"/testdata/1/notemptyi/testfile
echo "testtestasdf3" > "$TESTDATADIR"/testdata/1/notempty2/notempty
echo "testtestasdf4" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/test4
echo "testtestasdf5" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/test5
echo "testtestasdf6" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/test6
echo "testtestasdf7" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/test7
echo "testtestasdf8" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/ignoredir/testa
echo "testtestasdf9" > "$TESTDATADIR"/testdata/1/filexd

echo ".*4" > "$TESTDATADIR"/testdata/1/notempty2/.ignore
echo ".*5" > "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/.ignore
touch "$TESTDATADIR"/testdata/1/notemptyi/.nobackup
touch "$TESTDATADIR"/testdata/1/emptyi/.nobackup
touch "$TESTDATADIR"/testdata/1/notempty2/notemptyi2/ignoredir/.nobackup

cp -a -p "$TESTDATADIR"/testdata/1 "$TESTDATADIR"/testdata/2
echo "testtestasdf4u" > "$TESTDATADIR"/testdata/2/notempty2/notemptyi2/test4
echo "testtestasdf5u" > "$TESTDATADIR"/testdata/2/notempty2/notemptyi2/test5
echo "testtestasdf6u" > "$TESTDATADIR"/testdata/2/notempty2/notemptyi2/test6
echo "testtestasdf7u" > "$TESTDATADIR"/testdata/2/notempty2/notemptyi2/test7
echo "testtestasdf8u" > "$TESTDATADIR"/testdata/2/notempty2/notemptyi2/ignoredir/testa
echo "testtestasdf9u" > "$TESTDATADIR"/testdata/2/filexd

cp -a -p "$TESTDATADIR"/testdata/2 "$TESTDATADIR"/testdata/3
echo "testtestasdf4uu" > "$TESTDATADIR"/testdata/3/notempty2/notemptyi2/test4
echo "testtestasdf5uu" > "$TESTDATADIR"/testdata/3/notempty2/notemptyi2/test5
echo "testtestasdf6uu" > "$TESTDATADIR"/testdata/3/notempty2/notemptyi2/test6
echo "testtestasdf7uu" > "$TESTDATADIR"/testdata/3/notempty2/notemptyi2/test7
echo "testtestasdf8uu" > "$TESTDATADIR"/testdata/3/notempty2/notemptyi2/ignoredir/testa
echo "testtestasdf9uu" > "$TESTDATADIR"/testdata/3/filexd

cp -a -p "$TESTDATADIR"/testdata/2 "$TESTDATADIR"/testdata/4
echo "testtestasdf4uuu" > "$TESTDATADIR"/testdata/4/notempty2/notemptyi2/test4
echo "testtestasdf5uuu" > "$TESTDATADIR"/testdata/4/notempty2/notemptyi2/test5
echo "testtestasdf6uuu" > "$TESTDATADIR"/testdata/4/notempty2/notemptyi2/test6
echo "testtestasdf7uuu" > "$TESTDATADIR"/testdata/4/notempty2/notemptyi2/test7
echo "testtestasdf8uuu" > "$TESTDATADIR"/testdata/4/notempty2/notemptyi2/ignoredir/testa
echo "testtestasdf9uuu" > "$TESTDATADIR"/testdata/4/filexd

echo "Data created"

if ! $CMD init --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --compression zlib --compression-level 4 --encryption aes --password asdff --salt e; then
  echo "Error creating repo!"
  exit 1
fi
echo "Repo created"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/1 --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'Copied: notempty/testfile' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notempty' )  \
    && ( echo "$OUT" | grep -q 'Copied: filexd' )  \
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4 ' )\
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5 ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi ' )\
    && ! ( echo "$OUT" | grep -q 'emptyi ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi/testfile' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test6' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test7' ) ); then
  echo "Error backing up 1 dir!"
  exit 1
fi
echo "Backup 1 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/2 --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4 ' )\
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5 ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa ' )\
    && ( echo "$OUT" | grep -q 'Copied: filexd' )  \
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test6' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test7' ) ); then
  echo "Error backing up 2 dir!"
  exit 1
fi
echo "Backup 2 ok"

OUT=$($CMD run --from "$TESTDATADIR"/testdata/3 --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --password asdff --progress simple --verbose 1)
echo "$OUT"
if ! ( ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4 ' )\
    && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5 ' )\
    && ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa ' )\
        && ( echo "$OUT" | grep -q 'Copied: filexd' )  \
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test6' )\
    && ( echo "$OUT" | grep -q 'Copied: notempty2/notemptyi2/test7' ) ); then
  echo "Error backing up 3 dir!"
  exit 1
fi
echo "Backup 3 ok"


i=$((0))
AIDS=()
OUT=$($CMD list --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --password asdff)
echo "$OUT"
while IFS= read -r l; do
  ((i++))
  aid=$(echo $l | grep -Eo '[0-9]+' | tail -1)
  AIDS+=("$aid")
done <<< "$OUT"

OUT=$($CMD diff --from "$TESTDATADIR"/testdata/4 --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[0]} --aid2 ${AIDS[1]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' )  &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive 1 and 2!"
  exit 1
fi
echo "OK comparing archive 1 and 2"
OUT=$($CMD diff --from "$TESTDATADIR"/testdata/4 --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[1]} --aid2 ${AIDS[2]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' ) &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive 2 and 3!"
  exit 1
fi
echo "OK comparing archive 2 and 3"

OUT=$($CMD diff --from "$TESTDATADIR"/testdata/4 --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[2]})
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' ) &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive 3 and current!"
  exit 1
fi
echo "OK comparing archive 3 and current"

OUT=$($CMD diff --from "$TESTDATADIR"/testdata/4 --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --password asdff --progress none --verbose 1 )
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' ) &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive last (3) and current!"
  exit 1
fi
echo "OK comparing archive last (3) and current"

OUT=$($CMD diff --from "$TESTDATADIR"/testdata/4 --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --password asdff --progress none --verbose 1 --aid ${AIDS[2]} --prefix notempty2/notemptyi2 )
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ! ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' ) &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive 3 and current!"
  exit 1
fi
echo "OK comparing archive 3 and current with prefix "

OUT=$($CMD diff --from "$TESTDATADIR"/testdata/4 --repo "$"$TESTDATADIR"/testdataDIR"/testdir/to1 --password asdff --progress none --verbose 1 --prefix notempty2/notemptyi2 )
echo "$OUT"
if ! ( ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test6 is different' ) && ! ( echo "$OUT" | grep -q 'filexd is different' ) && ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test7 is different' ) \
 && ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test4' ) &&  ! ( echo "$OUT" | grep -q 'notempty2/notemptyi2/test5' ) &&  ! ( echo "$OUT" | grep -q 'notemptyi2/ignoredir/testa' ) ); then
  echo "Error comparing archive last (3) and current!"
  exit 1
fi
echo "OK comparing archive last (3) and current with prefix "


rm -rf "$"$TESTDATADIR"/testdataDIR"

exit 0