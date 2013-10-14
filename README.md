sphia(1) - cli tool for sophia databases
=====

Command utility for operations on a [sophia](https://github.com/pmwkaa/sophia) database

## Install

With [clib](https://github.com/clibs/clib):

```sh
$ clib install jwerle/sphia
```

With git:

```sh
$ git clone git@github.com:jwerle/sphia.git /tmp/sphia
$ cd /tmp/sphia && make install
```

## Usage


```sh
$ sphia
```

### Commands


Initialize a new database with a given path:

```sh
$ sphia init --path <path>
```                         

Get a value by key:

```sh
$ sphia get -k <key>
```

Set a value by key:

```sh
$ sphia set -k <key -v <value>
```

List all keys and values:

```sh
$ sphia ls --path <path>
```

Clear database of all keys:

```sh
$ sphia clear --path <path>
```

### Options

Output `sphia` version:

```sh
$ sphia --version
0.0.1
```

Output help information:

```sh
$ sphia --help
usage: sphia <command> [--key <key>] [--value <value>] [--path <path>]
                       [--version] [--help]

commands:
   init                         initialize a new database
   get -k <key>                 get a value by key
   set -k <key> -v <value>      set a value by key
   ls                           list all keys and values
   clear                        clears database of all keys

options:
  -V, --version                 output program version
  -h, --help                    output help information
  -V, --verbose                 enable verbose output
  -p, --path <path>             set the path
  -k, --key <name>              key to get
  -v, --value <name>            value to set
```

Turn on verbose output:

```sh
$ sphia ls --path ~/db --verbose
path set to '/Users/jwerle`
name = 'Joe'
age = 23
```

Set the database path (defaults to working directory):

```sh
$ sphia clear --path ~/db
Cleared sophia database at path '/Users/jwerle'
```

Set the key and/or value to get or set command:

```sh
$ sphia set --key name --value "Joseph Werle"
name = 'Joseph Werle'
```

## License

MIT


