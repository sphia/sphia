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

Via curl:

```sh
$ curl -Lo- https://raw.github.com/jwerle/sphia/master/install.sh | bash
```

## Setup

Set the `SPHIA_PATH` environment variable for your default database path. 
It will otherwise attempt to read from your current working database.

```sh
$ export SPHIA_PATH=~/db
```


```sh
$ sphia set -k name -v "Joseph Werle"
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

Get status on a database (will exit with 1 or 0 code):

```sh
$ sphia st --path <path>
```

Purge a database of all corrupt and incomplete data:

```sh
$ sphia purge --path <path>
```

Reset database of all data:

```sh
$ sphia reset --path <path>
```

Get key count:

```sh
$ sphia count --path <path>
```

Search for a key or value:

```sh
$ sphia search --key <key> --value <value>
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
   rm -k <key>                  remove a value by key
   ls                           list all keys and values
   clear                        clears database of all keys
   st                           check status of database
   purge                        purge database of all corrupt and incomplete data
   reset                        reset database of all data
   count                        get key count
   search [-k <arg>] [-v <arg>] search for a key or value

options:
  -V, --version                 output program version
  -h, --help                    output help information
  -V, --verbose                 enable verbose output
  -p, --path <path>             set the path
  -k, --key <name>              key to get
  -v, --value <name>            value to set
  -s, --strict                  strict mode for a command

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

## Contributors

See [contributors](https://github.com/sphia/sphia/graphs/contributors)

### Contributing

1. Fork it
2. Create your feature branch (git checkout -b my-new-feature)
3. Commit your changes (git commit -am "Add some feature")
4. Push to the branch (git push origin my-new-feature)
5. Create new Pull Request

## License

MIT


