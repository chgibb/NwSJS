//Seeing some issues with Jquery and Electron's fs.* functions getting clobbered for using pre and post Increments
//with spacing like this within loops.
var inc = 0;
++ inc
inc ++
if(inc != 2)
    process.exit(1);
console.log("Increments work!");