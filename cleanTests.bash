for f in $(find tests -name '*.ts'); 
do
    artifact=$(echo $f | awk '{gsub("\\.ts",".js");print}')
	rm $artifact
done

rm -r tests/*.jsbundle.js
rm -r tests/*.jscompBundle.js
rm -r tests/*.log