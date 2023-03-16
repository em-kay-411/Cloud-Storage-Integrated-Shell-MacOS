const { google } = require('googleapis');
const fs = require('fs');

// Set up OAuth2 client
const credentials = JSON.parse(fs.readFileSync('./credentials.json')); // replace with your credentials file path
const { client_secret, client_id, redirect_uris } = credentials.installed;
const oAuth2Client = new google.auth.OAuth2(
    client_id, client_secret, redirect_uris[0]
);

// Set up the Drive API
const drive = google.drive({
    version: 'v3',
    auth: oAuth2Client
});

// Get the name of the directory to list files from
const readline = require('readline').createInterface({
    input: process.stdin,
    output: process.stdout
});

const directoryName = process.argv[2];
// Find the directory ID based on its name
drive.files.list({
    q: `mimeType='application/vnd.google-apps.folder' and name='${directoryName}' and trashed=false`,
    fields: 'files(id, name)',
}, (err, res) => {
    if (err) return console.log(`Error finding directory: ${err}`);
    const files = res.data.files;
    if (files.length === 0) {
        return console.log('No directory found with that name.');
    }
    if (files.length > 1) {
        return console.log('Multiple directories found with that name. Please specify the directory ID manually.');
    }
    const directoryId = files[0].id;
    console.log(`Found directory '${directoryName}' with ID '${directoryId}'.`);
    listFiles(drive, directoryId);
});


// Authorize the client with credentials and user consent
fs.readFile('token.json', (err, token) => { // replace with your token file path
    if (err) return getAccessToken(oAuth2Client);
    oAuth2Client.setCredentials(JSON.parse(token));
});

// Function to retrieve a new access token if needed
function getAccessToken(oAuth2Client) {
    const authUrl = oAuth2Client.generateAuthUrl({
        access_type: 'offline',
        scope: ['https://www.googleapis.com/auth/drive.readonly']
    });
    console.log('Authorize this app by visiting this URL:', authUrl);
    const readline = require('readline').createInterface({
        input: process.stdin,
        output: process.stdout
    });
    readline.question('Enter the code from that page here: ', (code) => {
        readline.close();
        oAuth2Client.getToken(code, (err, token) => {
            if (err) return console.error('Error retrieving access token', err);
            oAuth2Client.setCredentials(token);
            fs.writeFile('token.json', JSON.stringify(token), (err) => { // replace with your token file path
                if (err) return console.error(err);
                console.log('Token stored to', 'token.json'); // replace with your token file path
            });
            
            listFiles(drive, 0);
        });
    });
}

// Use the Drive API to list files in the directory
function listFiles(drive, directoryId) {
    drive.files.list({
        q: `'${directoryId}' in parents`,
        fields: 'files(id, name)',
    }, (err, res) => {
        if (err) return console.log(`Error retrieving files: ${err}`);
        const files = res.data.files;
        if (files.length) {
            console.log('Files:');
            files.map((file) => {
                console.log(`${file.name} (${file.id})`);
            });
        } else {
            console.log('No files found.');
        }
    });
}
