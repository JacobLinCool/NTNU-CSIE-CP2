const fs = require("node:fs");
const path = require("node:path");
const ora = require("ora");
const { get_config } = require("./config");
const { dirs } = require("./utils");
const { md_to_pdf } = require("./pdf");

const HANDWRITING_SRC_MATCH = /^(hw|mid|fin)\d{2,4}.md$/;

(async () => {
    const targets = dirs();
    for (const dir of targets) {
        const spinner = ora({
            text: `[Conversion] ${path.basename(dir)} Converting Handwriting`,
            spinner: "bouncingBar",
        }).start();
        const config = get_config(dir);
        if (config.autobuild.handwriting) {
            const files = fs.readdirSync(dir).filter((file) => HANDWRITING_SRC_MATCH.test(file));
            for (const file of files) {
                spinner.text = `[Conversion] ${path.basename(dir)} Converting ${file}`;
                const src = path.resolve(dir, file);
                const dest = path.resolve(dir, file.replace(/\.md$/, ".pdf"));
                const pdf = await md_to_pdf(src);
                fs.writeFileSync(dest, pdf);
            }
            spinner.succeed(`[Conversion] ${path.basename(dir)} Handwriting Converted`);
        } else {
            spinner.warn(`[Conversion] ${path.basename(dir)} Handwriting Conversion Disabled`);
        }
    }
})();
