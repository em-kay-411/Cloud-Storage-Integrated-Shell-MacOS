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

async function changeDirectoryIdByName(auth, name) {
    if (name === "root") {
        return "root";
    }
    let folderId = 'root'; 
    fs.readFile('./gid.txt', function (err, data) {
        if (err) throw err;
        folderId = data.toString();        
      });
    if(folderId === null || folderId === undefined || folderId === ''){
        folderId = 'root';
    }
    const drive = google.drive({ version: 'v3', auth })
    try {
        const response = await drive.files.list({
            q: `mimeType='application/vnd.google-apps.folder' and trashed=false and name='${name}' and '${folderId}' in parents `,
            fields: 'nextPageToken, files(id)',
        });
        const files = response.data.files;
        if (files.length === 0) {

            return null;
        } else if (files.length >= 1) {
            // console.warn(`Multiple directories found with name '${name}', using first one`);
            return files[0].id;
        }

    } catch (err) {
        console.error('Error finding directory:', err);
        throw err;
    }
}

async function changeDirectoryIdByPath(auth, path) {
    const drive = google.drive({ version: 'v3', auth });
    const parts = path.split('/');    
    let folderId = 'root'; 
    fs.readFile('./gid.txt', function (err, data) {
        if (err) throw err;
        folderId = data.toString();        
      });
    if(folderId === null || folderId === undefined || folderId === ''){
        folderId = 'root';
    }
    for (const part of parts) {
        const query = `'${folderId}' in parents and mimeType='application/vnd.google-apps.folder' and name='${part}' and trashed=false`;
        const response = await drive.files.list({            
            q: query,
            fields: 'files(id)',
        });
        if (response.data.files.length === 0) {
            console.error(`Folder '${part}' not found in '${path}'`);
            return null;
        } else {
            folderId = response.data.files[0].id;
        }
    }
    return folderId;
}

async function chdir(auth){
    const newPath = process.argv[2]; 
    let ID = await changeDirectoryIdByPath(auth, newPath);
    if(ID === null || ID === undefined){
        ID = await changeDirectoryIdByName(auth, newPath);
    }
    if(ID != null){
        fs.writeFile("./gpath.txt", newPath, (err) => {
            if(err){
                console.error(err);
                return;
            }
            fs.writeFile("./gid.txt", ID, (err) => {
                if(err){
                    console.error(err);
                    return;
                }
            })
        })                
    }
    
}