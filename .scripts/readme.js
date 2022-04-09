const fs = require("node:fs");
const path = require("node:path");
const ora = require("ora");
const { get_config } = require("./config");
const { dirs } = require("./utils");
const { md_to_pdf } = require("./pdf");

const SOURCE_MATCH = /^hw\d{4}.c$/;
const HANDWRITING_MATCH = /^hw\d{4}.pdf$/;

/**
 * This function will generate README.md for target directory.
 * @param {string} dir Absolute path of target directory.
 * @param {string[]} files Filename list.
 * @returns
 */
const TEMPLATE = (dir, files) => {
    const sources = files.filter((file) => SOURCE_MATCH.test(file));
    const handwritings = files.filter((file) => HANDWRITING_MATCH.test(file));
    return `
# 程式設計二 ${path.basename(dir).toUpperCase()}

作者：師大資工 114 林振可 (41047029S)

*建議使用 Linux 系統執行。*

## 1. 編譯程式

請於此目錄執行：

\`\`\`bash
make
\`\`\`

\`make\` 應產生 ${sources.length} 個可執行檔。

## 2. 執行程式

請分別執行：

${sources
    .map((file) => {
        let code = "```bash\n./" + path.basename(file).replace(/\.c$/, "") + "\n```\n\n";
        const header = path.resolve(dir, path.basename(file).replace(/\.c$/, ".h"));
        if (fs.existsSync(header)) {
            const { title, description } = extract_docstring(fs.readFileSync(header, "utf8"));
            code += `**${title}**\n\n${description}\n\n`;
        }
        return code;
    })
    .join("")}

## 3. 手寫作業

${handwritings.length ? `手寫作業包含 ${handwritings.length} 個檔案。` : "這次沒有手寫作業。"}

${handwritings.map((f) => `- **${path.basename(f)}**`).join("\n")}
`;
};

(async () => {
    const targets = dirs();
    for (const dir of targets) {
        const spinner = ora({
            text: `[Readme] ${path.basename(dir)} Generating Readme`,
            spinner: "bouncingBar",
        }).start();
        const config = get_config(dir);
        if (config.autobuild.readme) {
            const README = generate_readme(dir);
            fs.writeFileSync(path.resolve(dir, "README.md"), README);
            spinner.text = `[Readme] ${path.basename(dir)} README.md Generated`;
        } else {
            spinner.warn(`[Readme] ${path.basename(dir)} README.md Generation Skipped`);
        }
        if (fs.existsSync(path.resolve(dir, "README.md"))) {
            spinner.text = `[Readme] ${path.basename(dir)} Converting README.md to PDF`;
            const pdf = await md_to_pdf(path.resolve(dir, "README.md"));
            fs.writeFileSync(path.resolve(dir, "README.pdf"), pdf);
            spinner.succeed(`[Readme] ${path.basename(dir)} README.pdf Converted`);
        }
    }
})();

function generate_readme(dir) {
    const files = fs.readdirSync(dir).filter((file) => !file.startsWith("."));
    return TEMPLATE(dir, files).replace(/\n\n\n+/g, "\n\n");
}

/**
 *
 * @param {string} code
 * @returns {{title: string; description: string}}
 */
function extract_docstring(code) {
    const first_docstring = code.match(/\/\*\*(.*?@brief.*?)\*\//s)?.[1];
    if (first_docstring) {
        const [_, title, description] = first_docstring.match(/@brief\s*([^\n]*)\s*(.*)/s);
        return {
            title: title.trim(),
            description: description
                .replace(/^\s*?\*/gm, "")
                .split("\n")
                .map((line) => line.trim())
                .join("\n"),
        };
    } else {
        return { title: "", description: "" };
    }
}
