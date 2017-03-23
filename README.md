# NwSJS
## No White Space JavaScript

Simple, lightweight, ES6 compliant cross-platform CLI utility to strip whitespace and comments from Javascript source code.

## Usage
### Running
``` 
./nwsjs srcFile.js --comments --spaces --tabs > outFile.js
```
Note: srcFile.js and outFile.js must NOT be the same file!

#### Options
Strip comments from srcFile.js
```
--comments
```

Strip spaces from srcFile.js
```
--spaces
```

Strip tabs from srcFile.js
```
--tabs
```

### Building
```
bash build.bash
```

## Testing
Download dependencies for testing
```
npm install
```

Run test suite
```
npm test
```

### Testing Process
NwSJS is tested against Microsoft's Typescript compiler and the Browserify Javascript bundler. The version of tsc downloaded by running ```npm install``` is itself compressed with NwSJS before being used to compile each .ts file. If NwSJS crashes or the compiler raises an error while compiling then the test fails.  

Each compiled .ts file is then bundled using Browserify. The resulting bundles are then compressed. The compressed and uncompressed bundles are run and their return codes are compared. If the return codes differ, then the test fails.