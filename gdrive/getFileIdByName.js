const { google } = require('googleapis');

async function getFileIdByName(auth, name) {
    const drive = google.drive({ version: 'v3', auth });

    try {
        const response = await drive.files.list({
            q: `name = '${name}' and trashed = false`,
            fields: 'nextPageToken, files(id)',
            spaces: 'drive'
        });

        // Check if the file was found
        const files = response.data.files;
        if (files.length === 0) {
            console.log(`File not found: ${name}`);
            return null;
        } else {
            // Return the first file ID
            return files[0].id;
        }
    }
    catch {
        console.error("No such file found", err);
        throw err;
    }
}

module.exports = getFileIdByName;