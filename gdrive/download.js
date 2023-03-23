const fs = require('fs');
const authorize = require('./oauth');
const path = require('path');
const { google } = require('googleapis');
const getDirectoryIdByName = require('./getDirectoryIdByName');
const getFileIdByName = require('./getFileIdByName');
const getDirectoryIdByPath = require('./getDirectoryIdByPath');
const getFileIdByPath = require('./getFileIdByPath');

const CREDENTIALS_PATH = './credentials.json';

// Load client secrets from a JSON file
fs.readFile(CREDENTIALS_PATH, (err, content) => {
    if (err) return console.error('Error loading client secret file:', err);
    // Authorize a client with credentials, and then call the Google Drive API.
    authorize(JSON.parse(content), downloadResource);
});

async function downloadResource(auth) {  
    const drive = google.drive({ version: 'v3', auth});  
    let resourceId = await getDirectoryIdByName(auth, process.argv[2]);
    
    if(resourceId === null){
        resourceId = await getFileIdByName(auth, process.argv[2]);
    }

    if(resourceId === null){
        resourceId = await getDirectoryIdByPath(auth, process.argv[2]);
    }

    if(resourceId === null){
        resourceId = await getFileIdByPath(auth, process.argv[2]);
    }
    console.log(resourceId);
    const resourcePath = process.argv[3];
    
    await download(drive, resourceId, resourcePath);  
}

async function download(drive, resourceId, resourcePath){    
    try {
        const resource = await drive.files.get({
            fileId: resourceId,
            fields: 'id, name, mimeType'
        });

        if (resource.data.mimeType === 'application/vnd.google-apps.folder') {
            // If the resource is a folder, create the local folder and download its contents recursively
            if (!fs.existsSync(resourcePath)) {
                fs.mkdirSync(resourcePath);
            }
            const files = await drive.files.list({
                q: `'${resourceId}' in parents and trashed = false`,
                fields: 'files(id, name, mimeType)',
            });
            for (const file of files.data.files) {
                const filePath = path.join(resourcePath, file.name);
                await download(drive, file.id, filePath);
            }
            console.log('Folder downloaded to', resourcePath);
        } else {
            // If the resource is a file, download it to the local file path
            const dest = fs.createWriteStream(resourcePath);
            const res = await drive.files.get({
                fileId: resourceId,
                alt: 'media',
            }, { responseType: 'stream' });
            res.data.pipe(dest);
            console.log('File downloaded to', resourcePath);
        }
    } catch (err) {
        console.error(err);
    }
}