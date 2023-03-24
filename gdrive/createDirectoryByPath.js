const { google } = require('googleapis');
const fs = require('fs');
const authorize = require('./oauth');
const getDirectoryIdByPath = require('./getDirectoryIdByPath');


const CREDENTIALS_PATH = "./credentials.json"

// Load client secrets from a JSON file
fs.readFile(CREDENTIALS_PATH, (err, content) => {
    if (err) return console.error('Error loading client secret file:', err);
    // Authorize a client with credentials, and then call the Google Drive API.
    authorize(JSON.parse(content), createDirectory);
});

async function createDirectory(auth){
    const drive = google.drive({version: 'v3', auth});
    const directoryName = process.argv[2];
    let parentID = await getDirectoryIdByPath(auth, process.argv[3]);
    console.log(parentID);

    const directoryDetails = {
        name: `${directoryName}`,
        mimeType: 'application/vnd.google-apps.folder',
        parents: [`${parentID}`]
    }

    try{
        drive.files.create({
            resource: directoryDetails,
            fields: 'id',
        })
        console.log("Folder created sucessfully");
    } catch(err){
        console.log(err);
    }
}