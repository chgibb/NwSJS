for f in node_modules/typescript/lib/*.js
do

    printf "$f original size: $(wc -c < $f)\n"
    ./nwsjs $f --comments --spaces --tabs > tmp
    mv tmp $f
    printf "$f compressed size $(wc -c < $f)\n\n\n\n"
done

for f in *.ts
do
    ./node_modules/.bin/tsc $f
    if [ $? != 0 ]; then
        exit 1
    fi
    artifact=$(echo $f | awk '{gsub("\\.ts",".js");print}')

    ./node_modules/.bin/browserify $artifact --node --debug -o bundle$artifact
    if [ $? != 0 ]; then
        exit 1
    fi
    printf "Compressing Browserify bundle\n"
    printf "bundle$artifact original size: $(wc -c < bundle$artifact)\n"
    ./nwsjs bundle$artifact --comments --spaces --tabs > compBundle$artifact
    if [ $? != 0 ]; then
        exit 1
    fi
    printf "bundle$artifact compressed size $(wc -c < compBundle$artifact)\n\n\n\n"

    node bundle$artifact
    retValBundle=$?

    node compBundle$artifact
    if [ $? != "$retValBundle" ]; then
        exit 1
    fi

done
