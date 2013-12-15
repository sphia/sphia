sphia(1) -- Command line utility for sophia databases
=================================

## SYNOPSIS

`sphia` <command> [--key <key>] [--value <value>] [--path <path>]
                [--config <config>] [--version] [--help]

## OPTIONS

  -V, --version
      output program version

  -h, --help
      output help information

  --verbose
      enable verbose output

  -p, --path <path>
      set the path

  -k, --key <name>
      key to get

  -v, --value <name>
      value to set

  -c, --config <path>
      read a config file

  -s, --strict
      enable strict mode for a command

## COMMANDS

  `init`
      initialize a new database

  `get`
      get a value by key

  `set`
      set a value by key

  `rm|remove`
      remove a value by key

  `ls|list`
      list all keys and values

  `clear`
      clears database of all keys

  `purge`
      purge database of all corrupt and incomplete data

  `reset`
      reset database of all data

  `count`
      get the count

  `search`
      search for a key or value

## AUTHOR

  - Joseph Werle <joseph.werle@gmail.com>

## CONTRIBUTORS

  - Stephen Mathieson <me@stephenmathieson.com>
  - Larz Conwell <larzconwell@gmail.com>

## REPORTING BUGS

  - <https://github.com/sphia/sphia/issues>

## SEE ALSO

  - <https://github.com/sphia/sphia>
  - <http://sphia.org>

## LICENSE

MIT
