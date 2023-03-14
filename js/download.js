const { google } = require('googleapis');
const fs = require('fs');
const readline = require('readline');
const { promisify } = require('util');
const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
});

const SCOPES = ['https://www.googleapis.com/auth/drive.readonly'];
const TOKEN_PATH = 'token.json';

// Load client secrets from a local file.
fs.readFile('credentials.json', async (err, content) => {
  if (err) return console.error('Error loading client secret file:', err);
  // Authorize a client with credentials, then call the Drive API.
  try {
    const credentials = JSON.parse(content);
    const { client_secret, client_id, redirect_uris } = credentials.installed;
    const oAuth2Client = new google.auth.OAuth2(
      client_id,
      client_secret,
      redirect_uris[0]
    );
    // Check if we have previously stored a token.
    let token = null;
    try {
      token = await promisify(fs.readFile)(TOKEN_PATH);
      oAuth2Client.setCredentials(JSON.parse(token));
    } catch (err) {
      token = await getAccessToken(oAuth2Client);
    }
    const drive = google.drive({ version: 'v3', auth: oAuth2Client });
    const fileId = '1YSb8VDThRIMGwkpobWJfIb3q24wjZaIH';
    const destPath = './new.pdf';
    const dest = fs.createWriteStream(destPath);
    const res = await drive.files.get(
      { fileId, alt: 'media' },
      { responseType: 'stream' }
    );
    res.data
      .on('end', () => console.log('File download complete.'))
      .on('error', err => console.error('Error downloading file:', err))
      .pipe(dest);
  } catch (err) {
    console.error('Error:', err);
  }
});

function getAccessToken(oAuth2Client) {
  return new Promise((resolve, reject) => {
    const authUrl = oAuth2Client.generateAuthUrl({
      access_type: 'offline',
      scope: SCOPES,
    });
    console.log('Authorize this app by visiting this url:', authUrl);
    rl.question('Enter the code from that page here: ', async code => {
      rl.close();
      try {
        const { tokens } = await oAuth2Client.getToken(code);
        oAuth2Client.setCredentials(tokens);
        // Store the token to disk for later program executions
        await promisify(fs.writeFile)(TOKEN_PATH, JSON.stringify(tokens));
        console.log('Token stored to', TOKEN_PATH);
        resolve(JSON.stringify(tokens));
      } catch (err) {
        console.error('Error retrieving access token:', err);
        reject(err);
      }
    });
  });
}
