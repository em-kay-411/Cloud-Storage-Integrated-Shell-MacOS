const { google } = require('googleapis');
const { OAuth2Client } = require('google-auth-library');
const credentials = require('./credentials.json');

const CLIENT_ID = 'id';
const CLIENT_SECRET = 'secret';
const REDIRECT_URI = 'http://localhost:3000/oauth2callback';
const SCOPES = ['https://www.googleapis.com/auth/drive.readonly'];

async function authorize() {
  const oAuth2Client = new OAuth2Client(CLIENT_ID, CLIENT_SECRET, REDIRECT_URI);

  // If the user has previously authorized your application, you can use the refresh token
  // to obtain a new access token without prompting the user to authenticate again.
  if (credentials.refresh_token) {
    oAuth2Client.setCredentials(credentials);
  } else {
    // If this is the first time the user is authorizing your application, generate a URL
    // for the user to grant permission to your application to access their Google Drive.
    const authUrl = oAuth2Client.generateAuthUrl({
      access_type: 'offline',
      scope: SCOPES,
    });
    console.log('Authorize this app by visiting this url:', authUrl);

    // Once the user has granted permission to your application, they will be redirected
    // back to the redirect URI specified in the Google Cloud Console. Extract the code
    // from the query string and exchange it for an access token and refresh token.
    const code = '4/0AWtgzh6O8sXDzrr-0jZSKrvgYCIk4Z8hfCUZxmnDsvcoZZjCZbFUlT6PHqZEDjRiBGwGTQ'; // replace with the actual code obtained from the redirect URL
    const { tokens } = await oAuth2Client.getToken(code);
    oAuth2Client.setCredentials(tokens);
    console.log('Access token:', tokens.access_token);
    console.log('Refresh token:', tokens.refresh_token);
  }

  return oAuth2Client;
}

async function getAuth() {
  const auth = await authorize();
  const drive = google.drive({ version: 'v3', auth });
  const response = await drive.files.list();
  console.log(response.data);
}

getAuth();
