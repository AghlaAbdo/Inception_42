const http = require('http');
const fs = require('fs');
const path = require('path');
const url = require('url');

const PORT = 5050;
// console.log("__dirname: ", __dirname);
const PUBLIC_DIR = path.join(__dirname, '../');

const server = http.createServer((req, res) => {
    const parsedUrl = url.parse(req.url);
    // console.log("parsedUrl: ", parsedUrl);
    let pathname = path.join(PUBLIC_DIR, parsedUrl.pathname);
    // console.log("parsedUrl.pathname: ", parsedUrl.pathname);
    
    fs.stat(pathname, (err, stats) => {
        if (err) {
            res.statusCode = 404;
            res.end(`File ${pathname} not found!`);
            return;
        }

        if (stats.isDirectory()) {
            pathname = path.join(pathname, 'index.html');
        }

        fs.readFile(pathname, (err, data) => {
            if (err) {
                res.statusCode = 500;
                res.end(`Error getting the file: ${err}.`);
            } else {
                const ext = path.parse(pathname).ext;
                const contentType = getContentType(ext);
                res.setHeader('Content-type', contentType || 'text/plain');
                res.end(data);
            }
        });
    });
});

function getContentType(ext) {
    const mimeTypes = {
        '.html': 'text/html',
        '.css': 'text/css',
        '.js': 'text/javascript',
        '.json': 'application/json',
        '.png': 'image/png',
        '.jpg': 'image/jpg',
        '.jpeg': 'image/jpeg',
        '.gif': 'image/gif',
        '.svg': 'image/svg+xml',
        '.ico': 'image/x-icon'
    };
    return mimeTypes[ext];
}

server.listen(PORT, () => {
    console.log(`Server running on http://localhost:${PORT}/`);
});