const { google } = require('googleapis');
const authorize = require('./oauth');
const fs = require('fs');
const getDirectoryIdByName = require('./getDirectoryIdByName')

const CREDENTIALS_PATH = './credentials.json';

// Load client secrets from a JSON file
fs.readFile(CREDENTIALS_PATH, (err, content) => {
  if (err) return console.error('Error loading client secret file:', err);
  // Authorize a client with credentials, and then call the Google Drive API.
  authorize(JSON.parse(content), listFiles);
});

async function listFiles(auth) {
  const drive = google.drive({ version: 'v3', auth });
  const DIRECTORY_ID = await getDirectoryIdByName(auth, process.argv[2]);
  if (DIRECTORY_ID != null) {
    drive.files.list({
      q: `'${DIRECTORY_ID}' in parents and trashed = false`,
      fields: 'nextPageToken, files(id, name)',
    }, (err, res) => {
      if (err) return console.error('Error listing files', err);
      const files = res.data.files;
      if (files.length) {

        files.forEach((file) => {
          console.log(`${file.name}`);
        });
      } else {
        console.log('No files found in the directory');
      }
    });
  }
  else {
    console.log("No directory of such name found");
  }

}
