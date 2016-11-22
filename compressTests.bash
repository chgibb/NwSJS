for f in test262/test/**/*.js ; do
  printf "Compressing $f\n"
  cp $f tmp
  rm $f
  time ./nwsjs tmp > $f
  rm tmp
done
