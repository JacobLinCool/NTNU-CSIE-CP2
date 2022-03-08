const fs = require("node:fs");
const path = require("node:path");
const yaml = require("js-yaml");

const configs = new Map();

/**
 * Get config of target directory.
 * @param {string} dir Target directory.
 * @returns {object} Config object.
 */
function get_config(dir) {
    const filepath = path.resolve(dir, ".config.yml");
    if (!configs.has(filepath)) {
        if (fs.existsSync(filepath)) {
            configs.set(filepath, yaml.load(fs.readFileSync(filepath, "utf8")));
        } else {
            configs.set(filepath, {});
        }
    }
    return configs.get(filepath);
}

module.exports = { configs, get_config };
