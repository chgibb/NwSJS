# NwSJS
## No White Space JavaScript

Simple, lightweight, cross-platform CLI utility to strip whitespace and comments from Javascript source code.
Approximately 75x faster, with 25% less CPU usage than UglifyJS at the same task.
Currently tends to choke on files > 500kb.

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

### Testing
NwSJS is tested against Ecma International, Technical Committee 39's test suite. See https://github.com/tc39/test262 for more information. Tests are run using test262-node-harness

Clone test262 repo.
```
bash refreshTests.bash
```

Download test262-node-harness (through npm. Declared in package.json).
See https://github.com/bterlson/test262-harness for more information.
```
npm install
```

Run NwSJS on the test files.
```
bash compressTests.bash
```

Run the test harness on the compressed test files.
```
bash run.bash > log
```

Get total number of errors caused by NwSJS
```
grep SyntaxError: log | wc -l
```
Output should (hopefuly) be 0.
