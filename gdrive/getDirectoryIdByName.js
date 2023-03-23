const fs = require('fs');
const { google } = require('googleapis');

async function getDirectoryIdByName(auth, name) {
    if (name === "root") {
        return "root";
    }
    let folderId = 'root'; 
    
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

module.exports = getDirectoryIdByName;