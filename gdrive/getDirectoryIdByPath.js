const fs = require('fs');
const { google } = require('googleapis');

async function getDirectoryIdByPath(auth, path) {
    const drive = google.drive({ version: 'v3', auth });
    const parts = path.split('/');    
    let folderId = 'root'; 
    
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

module.exports = getDirectoryIdByPath
