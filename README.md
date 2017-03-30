# NwSJS
## No White Space JavaScript

Simple, lightweight, ES6 compliant cross-platform CLI utility to strip whitespace and comments from Javascript source code.

## Usage
### Running
#### Linux
``` 
$ ./nwsjs srcFile.js > outFile.js
```
#### Windows
``` 
> nwsjs.exe srcFile.js > outFile.js
```
Note: srcFile.js and outFile.js must NOT be the same file!  
### Using From NPM  
### Installing  
#### Linux  
```
$ npm install nwsjs
```
or
```
$ npm install -g nwsjs
```
#### Windows
```
> npm install --no-bin-links nwsjs
```
Note: Bin links for Windows do NOT work properly currently. Therefore, installing NwSJS globally on Windows is NOT supported. NwSJS will therefore be installed to  node_modules\nwsjs\nwsjs.exe on Windows.

### Running
#### Linux
```
$ ./node_modules/.bin/nwsjs
```
or
```
$ ./node_modules/nwsjs/nwsjs
```
or (if installed globally)
```
$ nwsjs
```
#### Windows
```
> node_modules\nwsjs\nwsjs
```

### Building
#### Linux
```
$ bash build.bash
```
#### Windows
```
> build
```
Note: Building requires that ```g++``` be available on your ```PATH``` for both Linux and Windows.

## Testing
Download dependencies for testing
```
npm install
```

Run test suite
```
npm test
```
Note: For Windows users, running tests requires the availability of a valid ```bash.exe``` on your ```%PATH%```.

### Testing Process
NwSJS is tested against Microsoft's Typescript compiler and the Browserify Javascript bundler. The version of tsc downloaded by running ```npm install``` is itself compressed with NwSJS before being used to compile each .ts file. If NwSJS crashes or the compiler raises an error while compiling then the test fails.  

Each compiled .ts file is then bundled using Browserify. The resulting bundles are then compressed. The compressed and uncompressed bundles are run and their return codes are compared. If the return codes differ, then the test fails.

Note: See ```test.bash``` for full process and more notes.