const { google } = require('googleapis');
const fs = require('fs');
const path = require('path');
const authorize = require('./oauth');
const getDirectoryIdByPath = require('./getDirectoryIdByPath');
const getDirectoryIdByName = require('./getDirectoryIdByName');

const CREDENTIALS_PATH = "./credentials.json"

// Load client secrets from a JSON file
fs.readFile(CREDENTIALS_PATH, (err, content) => {
    if (err) return console.error('Error loading client secret file:', err);
    // Authorize a client with credentials, and then call the Google Drive API.
    authorize(JSON.parse(content), chdir);
});

async function chdir(auth){
    const newPath = process.argv[2]; 
    const ID = await getDirectoryIdByPath(auth, newPath);
    if(ID === null){
        ID = await getDirectoryIdByName(auth, newPath);
    }
    if(ID != null){
        fs.writeFile("./gpath.txt", newPath, (err) => {
            if(err){
                console.error(err);
                return;
            }
        })        
    }
    
}