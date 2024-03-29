# pifantuner

An i2c Raspberry Pi 4 fan tuner.  

Supports the Argon Fan Hat.

## Build

### Dependencies

* CMake (Version 3.21 or greater)
* libi2c-dev

### Building

```
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
```

### Installing

```
sudo make install
```

### Running `pifantunerd`

A systemd service is installed. Alternatively the daemon may be invoked directly:
```
Usage: ./pifantunerd temperature:speed temperature:speed ..
```

### Contributing

Install the pre-commit hook prior to performing your first code commit.  
For more information about pre-commit, [See the pre-commit documentation](https://pre-commit.com/).
