(set -o igncr) 2>/dev/null && set -o igncr; # For Cygwin on Windows compaibility
#For each .js file in the Typescript compiler distribution
for f in node_modules/typescript/lib/*.js
do
    #print its size
    printf "$f original size: $(wc -c < $f)\n"
    #compress it
    ./nwsjs $f --comments --spaces --tabs > tmp
    mv tmp $f
    #print its size after compression
    printf "$f compressed size $(wc -c < $f)\n\n\n\n"

    if [ $(wc -c < $f) == "0" ]; then
        printf "Clobbered $f\n"
        exit 1
    fi
done

#for each of our .ts test files
for f in *.ts
do
    #try to run our now compressed Typescript compiler over them
    ./node_modules/.bin/tsc $f
    #if this fails, that means we've probably broken the Typescript compiler by compressing it
    if [ $? != 0 ]; then
        exit 1
    fi

    #get the file name of the output .js file
    artifact=$(echo $f | awk '{gsub("\\.ts",".js");print}')

    #use browserify to bundle the file and save it to bundleOutput.js
    ./node_modules/.bin/browserify $artifact --node --debug -o bundle$artifact
    if [ $? != 0 ]; then
        exit 1
    fi

    #Print the file size of the bundle and then compress it
    printf "Compressing Browserify bundle\n"
    printf "bundle$artifact original size: $(wc -c < bundle$artifact)\n"
    ./nwsjs bundle$artifact --comments --spaces --tabs > compBundle$artifact
    #If we've failed to compress the bundle then it's likely to too complex and nwsjs has tripped on itself trying to compress it
    if [ $? != 0 ]; then
        exit 1
    fi

    #print the file size of the compressed bundle
    printf "bundle$artifact compressed size $(wc -c < compBundle$artifact)\n\n\n\n"

    #Run the uncompressed bundle
    node bundle$artifact &> "bundle$artifact".log
    #Save it's return value
    retValBundle=$?

    #Run the compressed bundle
    node compBundle$artifact &> "compBundle$artifact".log
    #If the return values differ from running the uncompressed and compressed bundles, then we've probably broken something
    #We test return values between the two because some tests use npm packages that don't work in Node and cause it to crash
    #These are used because they're very large and are good to stress test our compressor.
    #Those tests which don't crash Node under normal circumstances, but crash Node after being compressed means we've broken them 
    #by compressing them. This simple test ensures we test only for behaviour which has changed from uncompressed to compressed bundles. 
    if [ $? != "$retValBundle" ]; then
        exit 1
    fi
done
