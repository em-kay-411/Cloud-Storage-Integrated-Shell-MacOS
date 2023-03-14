const { google } = require('googleapis');
const fs = require('fs');

function getFileID(fileName){
    const credentials = JSON.parse(fs.readFileSync('credentials.json'));


    const { client_secret, client_id, redirect_uris } = credentials.installed;
    const oAuth2Client = new google.auth.OAuth2(
        client_id, client_secret, redirect_uris[0]);


    const token = fs.readFileSync('token.json');
    oAuth2Client.setCredentials(JSON.parse(token));


    const drive = google.drive({ version: 'v3', auth: oAuth2Client });



    // Search for the file with the given name
    return drive.files.list({
        q: `name='${fileName}' and trashed=false`,
        fields: 'nextPageToken, files(id, name)',
    })
        .then(res => {
            const files = res.data.files;
            if (files.length) {
                console.log(files[0].id);
                return files[0].id;
            } else {
                console.log(`No file found with name ${fileName}`);
                return null;
            }
        })
        .catch(err => {
            console.log('The API returned an error: ' + err);
            return null;
        });
}


module.exports = getFileID;



