const { google } = require('googleapis');
const fs = require('fs');
const authorize = require('./oauth');


const CREDENTIALS_PATH = "./credentials.json"

// Load client secrets from a JSON file
fs.readFile(CREDENTIALS_PATH, (err, content) => {
    if (err) return console.error('Error loading client secret file:', err);
    // Authorize a client with credentials, and then call the Google Drive API.
    authorize(JSON.parse(content), changeToParent);
});

async function changeToParent(auth) {
    const drive = google.drive({ version: 'v3', auth });
  
    try {
        //Here we need to read two files, and using it by callbacks would make it messy.
      const folderId = (await fs.promises.readFile('./gid.txt')).toString().trim();
      const path = (await fs.promises.readFile('./gpath.txt')).toString().trim();
  
    //   console.log(path);
      let i = 0;
      if (path && path.length) {
        i = path.length - 1;
      }
      while (i >= 0 && path.charAt(i) != '/') {
        i--;
      }
      let newPath = "";
      if(i < 0){
        newPath = "";
      }
      else{ 
        newPath = path.slice(0, i);
      }
      
    //   console.log(folderId);
    //   console.log(newPath);
      
      const res = await drive.files.get({ fileId: folderId, fields: 'parents' });
      const parents = res.data.parents;
      if (parents && parents.length > 0) {
        const parentId = parents[0];
        await fs.promises.writeFile('./gpath.txt', newPath);
        await fs.promises.writeFile('./gid.txt', parentId);
      } else {
        console.log('No parent directory found.');
      }
    } catch (err) {
      console.error(err);
    }
  }
  