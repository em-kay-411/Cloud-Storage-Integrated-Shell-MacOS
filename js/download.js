const fs = require('fs');
const {google} = require('googleapis');

// Replace with your own credentials
const credentials = require('./credentials.json');

// Replace with your own file ID
const fileId = '1YSb8VDThRIMGwkpobWJfIb3q24wjZaIH';

// Create a new OAuth2 client with the credentials
const authClient = new google.auth.OAuth2(
  credentials.client_id,
  credentials.client_secret,
  credentials.redirect_uris[0]
);

// Set the credentials on the client
authClient.setCredentials(credentials.token);

// Create a new Drive API client
const drive = google.drive({version: 'v3', auth: authClient});

// Set the file ID and destination path
const destPath = './my-file.txt';

// Use the Drive API to download the file
const dest = fs.createWriteStream(destPath);
drive.files.get({fileId: fileId, alt: 'media'}, {responseType: 'stream'})
  .then(res => {
    res.data
      .on('end', () => {
        console.log('File downloaded.');
      })
      .on('error', err => {
        console.error(`Error downloading file: ${err}`);
      })
      .pipe(dest);
  })
  .catch(err => {
    console.error(`Error retrieving file metadata: ${err}`);
  });
