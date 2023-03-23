const { google } = require('googleapis');
const fs = require('fs');
const path = require('path');
const authorize = require('./oauth');
const getDirectoryIdByPath = require('./getDirectoryIdByPath')

const CREDENTIALS_PATH = "./credentials.json"

// Load client secrets from a JSON file
fs.readFile(CREDENTIALS_PATH, (err, content) => {
    if (err) return console.error('Error loading client secret file:', err);
    // Authorize a client with credentials, and then call the Google Drive API.
    authorize(JSON.parse(content), uploadFileOrFolder);
});

async function uploadFileOrFolder(auth) {
    const drive = google.drive({ version: 'v3', auth });
    const localPath = process.argv[2];
    const drivePath = process.argv[3];

    // Get the file/folder name from the local path
    const fileName = path.basename(localPath);

    // Get the folder ID for the target folder in Google Drive
    const folderId = await getDirectoryIdByPath(auth, drivePath);

    if (!folderId) {
        console.error(`Folder '${drivePath}' not found`);
        return;
    }

    // Check if the local path is a file or folder
    const stat = fs.statSync(localPath);
    if (stat.isFile()) {
        // Upload a file
        const media = {
            mimeType: 'application/octet-stream',
            body: fs.createReadStream(localPath),
        };
        const fileMetadata = {
            name: fileName,
            parents: [folderId],
        };
        const res = await drive.files.create({
            resource: fileMetadata,
            media: media,
            fields: 'id',
        });
        console.log(`File '${fileName}' uploaded to '${drivePath}' with ID: ${res.data.id}`);
    } else if (stat.isDirectory()) {
        // Upload a folder
        const folderMetadata = {
            name: fileName,
            mimeType: 'application/vnd.google-apps.folder',
            parents: [folderId],
        };
        const res = await drive.files.create({
            resource: folderMetadata,
            fields: 'id',
        });
        console.log(`Folder '${fileName}' uploaded to '${drivePath}' with ID: ${res.data.id}`);

        // Recursively upload files in the folder
        const files = fs.readdirSync(localPath);
        for (const file of files) {
            const filePath = path.join(localPath, file);
            await uploadFileOrFolder(auth, filePath, path.join(drivePath, fileName));
        }
    } else {
        console.error(`Path '${localPath}' is not a file or folder`);
    }
}

