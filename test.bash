for f in node_modules/typescript/lib/*.js
do
    printf "Compressing $f\n"
    ./nwsjs $f --comments --spaces --tabs > tmp
    mv tmp $f
    rm tmp
done

./node_modules/.bin/tsc index.ts

./node_modules/.bin/browserify index.js --node --debug -o bundle.js

./nwsjs bundle.js --comments --spaces --tabs > compBundle.js