const fs = require("node:fs");
const path = require("node:path");

/**
 * Get target directories.
 * @returns {string[]} Absolute paths of target directories.
 */
function dirs() {
    const dirs = [];
    if (path.resolve() == path.resolve(__dirname, "..")) {
        fs.readdirSync(path.resolve("source"))
            .filter((f) => fs.statSync(path.resolve("source", f)).isDirectory())
            .forEach((dir) => {
                dirs.push(path.resolve("source", dir));
            });
    } else {
        dirs.push(path.resolve());
    }
    return dirs;
}

module.exports = { dirs };
