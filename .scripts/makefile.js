const fs = require("node:fs");
const path = require("node:path");
const ora = require("ora");
const { get_config } = require("./config");
const { dirs } = require("./utils");

const SOURCE_MATCH = /^.*?\.c$/;

dirs().forEach((dir) => {
    const spinner = ora({
        text: `[Makefile] ${path.basename(dir)} Generating Makefile`,
        spinner: "bouncingBar",
    }).start();
    const config = get_config(dir);
    if (config.autobuild.makefile) {
        const Makefile = generate_makefile(dir);
        fs.writeFileSync(path.resolve(dir, "Makefile"), Makefile);
        spinner.succeed(`[Makefile] ${path.basename(dir)} Makefile Generated`);
    } else {
        spinner.warn(`[Makefile] ${path.basename(dir)} Makefile Generation Disabled`);
    }
});

function generate_makefile(dir) {
    let all = "",
        clean = "";
    const sources = fs.readdirSync(dir).filter((file) => SOURCE_MATCH.test(file));
    for (let i = 0; i < sources.length; i++) {
        const filename = path.basename(sources[i], ".c");
        all += `\tgcc ${filename}.c -lm -o ${filename}\n`;
        clean += `\trm -f ${filename}\n`;
    }
    return `all: \n${all}\nclean: \n${clean}\trm -f ${path.basename(dir)}.zip\n\nzip: \n\tzip ${path.basename(
        dir
    )} *.c *.h **/*.h *.pdf Makefile\n`;
}
