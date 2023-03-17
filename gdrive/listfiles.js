const {google} = require('googleapis');
const {OAuth2Client} = require('google-auth-library');
const fs = require('fs');
const readline = require('readline');

const CREDENTIALS_PATH = './credentials.json';
const TOKEN_PATH = './token.json';
const REDIRECT_URI = 'http://localhost:3000/oauth2callback';

// Load client secrets from a JSON file
fs.readFile(CREDENTIALS_PATH, (err, content) => {
  if (err) return console.error('Error loading client secret file:', err);
  // Authorize a client with credentials, and then call the Google Drive API.
  authorize(JSON.parse(content), listFiles);
});

function authorize(credentials, callback) {
  const {client_id, client_secret, redirect_uris} = credentials.installed;
  const oAuth2Client = new OAuth2Client(client_id, client_secret, redirect_uris[0]);

  // Check if we have previously stored a token.
  fs.readFile(TOKEN_PATH, (err, token) => {
    if (err) {
      return getNewToken(oAuth2Client, callback);
    }
    oAuth2Client.setCredentials(JSON.parse(token));
    callback(oAuth2Client);
  });
}

function getNewToken(oAuth2Client, callback) {
  const authUrl = oAuth2Client.generateAuthUrl({
    access_type: 'offline',
    scope: ['https://www.googleapis.com/auth/drive.readonly']
  });
  console.log('Authorize this app by visiting this url:', authUrl);
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });
  rl.question('Enter the code from that page here: ', (code) => {
    rl.close();
    oAuth2Client.getToken(code, (err, token) => {
      if (err) return console.error('Error retrieving access token', err);
      oAuth2Client.setCredentials(token);
      // Store the token to disk for later program executions
      fs.writeFile(TOKEN_PATH, JSON.stringify(token), (err) => {
        if (err) return console.error(err);
        console.log('Token stored to', TOKEN_PATH);
      });
      callback(oAuth2Client);
    });
  });
}

async function getDirectoryIdByName(auth, name) {
    const drive = google.drive({version: 'v3', auth})
    const response = await drive.files.list({
      q: `mimeType='application/vnd.google-apps.folder' and trashed=false and name='${name}'`,
      fields: 'nextPageToken, files(id)',
    });
    const files = response.data.files;
    if (files.length === 0) {
      throw new Error(`No directory found with name '${name}'`);
    } else if (files.length > 1) {
      console.warn(`Multiple directories found with name '${name}', using first one`);
    }
    return files[0].id;
}

async function listFiles(auth) {
  const drive = google.drive({version: 'v3', auth});
  const DIRECTORY_ID = await getDirectoryIdByName(auth, process.argv[2]);
  drive.files.list({
    q: `'${DIRECTORY_ID}' in parents and trashed = false`,
    fields: 'nextPageToken, files(id, name)',
  }, (err, res) => {
    if (err) return console.error('Error listing files', err);
    const files = res.data.files;
    if (files.length) {
      console.log('Files in the directory:');
      files.forEach((file) => {
        console.log(`${file.name} (${file.id})`);
      });
    } else {
      console.log('No files found in the directory');
    }
  });
}
