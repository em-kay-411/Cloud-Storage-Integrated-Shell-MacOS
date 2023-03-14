const fs = require('fs');
const {google} = require('googleapis');
const SCOPES = ['https://www.googleapis.com/auth/drive.readonly'];
const credentials = require('./credentials.json');

// Check if we have previously stored a token
fs.readFile('token.json', (err, token) => {
  if (err) {
    return getNewToken(authClient);
  } else {
    authClient.setCredentials(JSON.parse(token));
  }
});

function getNewToken(oauth2Client) {
  const authUrl = oauth2Client.generateAuthUrl({
    access_type: 'offline',
    scope: SCOPES,
  });
  console.log(`Authorize this app by visiting this URL: ${authUrl}`);
  const readline = require('readline');
  const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });
  rl.question('Enter the code from that page here: ', code => {
    rl.close();
    oauth2Client.getToken(code, (err, token) => {
      if (err) {
        return console.error(`Error retrieving access token: ${err}`);
      }
      oauth2Client.setCredentials(token);
      fs.writeFile('token.json', JSON.stringify(token), err => {
        if (err) {
          return console.error(`Error writing token to file: ${err}`);
        }
        console.log('Token stored to token.json');
      });
    });
  });
}

// Replace with your own credentials


// Replace with your own file ID
const fileId = '1YSb8VDThRIMGwkpobWJfIb3q24wjZaIH';

// Create a new OAuth2 client with the credentials
const authClient = new google.auth.OAuth2(
  credentials.client_id,
  credentials.client_secret,
  credentials.redirect_uris && credentials.redirect_uris.length > 0 ? credentials.redirect_uris[0] : null
);


// Set the credentials on the client
authClient.setCredentials(credentials.token);

// Create a new Drive API client
const drive = google.drive({version: 'v3', auth: authClient});

// Set the file ID and destination path
const destPath = './my-file.pdf';

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
