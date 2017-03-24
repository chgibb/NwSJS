#!/usr/bin/env node
var spawn = require("child_process");
var args = process.argv;
args.splice();
args.splice();
if(process.platform == "linux")
{
    spawn.spawnSync("nwsjslinux",args);
}
if(process.platform == "win32")
{
    spawn.spawnSync("nwsjswin",args);
}