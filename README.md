# Greeter #

Greeter is a sample application server for use with Clearwater.  It is intended to act as a starting point for development of new application servers.

## What it does ##

Greeter's only function is to add a header "Subject: Hello world!" on each transaction-initiating message that it sees.

## Developing on Greeter ##

### Getting started ###

- Check out the Sprout code into `~sprout`:
  - `git clone --recursive https://github.com/Metaswitch/sprout.git ~/sprout`
- Checkout the Greeter code into `~sprout/plugins/greeter`:
  - `git clone https://github.com/Metaswitch/greeter.git ~/sprout/plugins/greeter`
- Compile everything:
  - `cd ~sprout && make`

This will compile Sprout and all of the third-party code that it uses, and will also automatically discover and compile Greeter.  

This initial compilation may take some time.  However, you only need to do this once.  Further development on Greeter has a much faster cycle.

### Making changes ###

Further development can all take place in the `~sprout/plugins/greeter` directory.  From there you can compile, test and package your code via the provided `Makefile`.  

The most important targets are:

- `make build` will compile the Greeter shared object
- `make test` will compile and run the Greeter unit tests
- `make deb` will build a Debian package for Greeter

Take a look at the `Makefile` to see what else is available.
