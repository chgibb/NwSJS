//Seeing some issues with Jquery and Electron's fs.* functions getting clobbered for using pre and post Increments
//with spacing like this within loops.
/*var inc = 0;
++ inc
inc ++
if(inc != 2)
    process.exit(1);
console.log("Increments work!");

for(var i = 0; i != 1; ++ i)
{
    for(var k = 0; k != 2; k ++ )
    {
        inc ++
    }
}
if(inc != 4)
    process.exit(1);
console.log("Loop increments work!");*/