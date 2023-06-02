# Backup app

## Key features

- Deduplicated backups with additional compression and encryption

- Files are split into chunks and stored in a repository as a list of
  pointers to these chunks

- These chunks are reused for all files in the repository, based on
  their hash matching.

- If when creating an archive, a file hasn’t changed it is reused from
  a previous archive

- Content defined chunking algorithm based on a rolling hash

    - Unlike when splitting file in fixed size chunks, still works
      when there is an insertion/deletion of bytes in middle/beginning
      of the file

- These Files, Chunks and Archives are stored in object storage
  Repository

    - Default implementation - FileRepository, stores these objects in
      the file system

    - Possible other implementations - database, cloud object storage…

- Multithreading

- Directories are ignored if they have a `.nobackup` file

- Files are ignored in a directory recursively based on a `.ignore`
  file - one line contains one regex rule, which, if a directory entry
  matches, it is ignored

# Quick start

Initialize a repository with compression and encryption (you can also
specify other options like –from here, they will be written to the
repository if it’s possible, so far changing options of an already
existing repo is not implemented)

    sembackup init --repo <target dir> --compression zlib --compression-level 4 --encryption aes --password <password> --salt <random salt>

Run a backup

    sembackup run --from <source dir> --repo <target dir> --password <password>

List avaiable archives

    sembackup list --repo <repo dir> --password <password>

List files in an archive

    sembackup list-files --repo <repo dir> --password <password> --aid <archive id>

Restore a backup

    sembackup restore --repo <repo dir> --password <password> --aid <archive id> --to <destination>

Compare source dir with latest archive

    sembackup diff --repo <repo dir> --password <password> --from <source dir>

Compare source dir with an archive

    sembackup diff --repo <repo dir> --password <password> --from <source dir> --aid <archive id>

Compare two archives

    sembackup diff --repo <repo dir> --password <password> --from <source dir> --aid <archive id> --aid2 <archive id>

Compare subdirectory in the source dir with latest archive

    sembackup diff --repo <repo dir> --password <password> --from <source dir> --prefix <subdir>

# Data format

All data is represented as objects, stored in a repository.

An `Archive` object represents a snapshot of the file system in the
moment of its creation, and consists of a list of pointers (Object ids)
to `File` objectsю

`File` object consists of its basic metadata, and a list of chunks,
identified by their ids, which can be shared between multiple files (and
within the same file) if their MD5 hashes match.

`Chunk` object is a binary blob, identified by its MD5 hash.

These objects are children of `Object`, providing a `getKey()` method,
(name for `Archive`, path for `File`, and MD5 hash for `Chunk`) which is
used by `Repository` to make them easily accessible.

In default (and so far the only) repository implementation
`FileRepository` these objects are grouped together into files of size
approixmately `repo-target` MB, (by default 128), and there exists a
key-value index of indexed objects written into `index`, and an
`offsets` file recording the location and offset of each object in the
file system.
