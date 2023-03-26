const { google } = require('googleapis');

async function getFileIdByPath(auth, path) {
    const drive = google.drive({ version: 'v3', auth });

    // Split the path into parts and get the file name
    const parts = path.split('/');
    const fileName = parts.pop();

    // Search for the file by name and parent folder
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

    // Search for the file by name and parent folder
    const query = `'${folderId}' in parents and name='${fileName}' and trashed=false`;
    const response = await drive.files.list({
        q: query,
        fields: 'files(id)',
    });

    if (response.data.files.length === 0) {
        // console.error(`File '${fileName}' not found in '${path}'`);
        return null;
    } else {
        return response.data.files[0].id;
    }
}

module.exports = getFileIdByPath
