const { google } = require('googleapis');
const fs = require('fs');
const authorize = require('./oauth');
const getDirectoryIdByName = require('./getDirectoryIdByName');
const getDirectoryIdByPath = require('./getDirectoryIdByPath');
const getFileIdByName = require('./getFileIdByName');
const getFileIdByPath = require('./getFileIdByPath');


const CREDENTIALS_PATH = "./credentials.json"

// Load client secrets from a JSON file
fs.readFile(CREDENTIALS_PATH, (err, content) => {
    if (err) return console.error('Error loading client secret file:', err);
    // Authorize a client with credentials, and then call the Google Drive API.
    authorize(JSON.parse(content), moveFile);
});


async function moveFile(auth) {
    const drive = google.drive({version: 'v3', auth});
    let ID = await getDirectoryIdByName(auth, process.argv[2]);
    if(ID === null || ID === undefined){
        ID = await getDirectoryIdByPath(auth, process.argv[2]);
    }

    if(ID === null || ID === undefined){
        ID = await getFileIdByName(auth, process.argv[2]);
    }

    if(ID === null || ID === undefined){
        ID = await getFileIdByPath(auth, process.argv[2]);
    }

    const oldFolderId = (await fs.promises.readFile('./gid.txt')).toString().trim();
    
    let newFolderId = await getDirectoryIdByName(auth, process.argv[3]);
    if(newFolderId === null || newFolderId === undefined){
        newFolderId = await getDirectoryIdByPath(auth, process.argv[3]);
    }
    drive.files.update({
      fileId: ID,
      addParents: newFolderId,
      fields: 'id, parents'
    }, (err, file) => {
      if (err) {
        // Handle error
        console.log("Wrong path or folder entered");
      } else {
        console.log(`File ${ID} copied to folder ${newFolderId}.`);
      }
    });
  }