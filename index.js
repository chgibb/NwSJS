#!/usr/bin/env node
let spawn = require("child_process");
let args = process.argv;
args.splice();
args.splice();
let proc;
if(process.platform == "linux")
{
    proc = spawn.spawn("nwsjs",args);
}
if(process.platform == "win32")
{
    proc = spawn.spawn("nwsjs.exe",args);
}

proc.stdout.on('data',function(data){
    console.log(data);
});
