const fs = require("node:fs");
const { mdToPdf } = require("md-to-pdf");

/**
 * @param {string} markdown_path
 * @param {string} output_path
 * @returns {Promise<Buffer>}
 */
async function md_to_pdf(markdown_path) {
    if (!fs.existsSync(markdown_path)) {
        return;
    }
    const pdf = await mdToPdf(
        { path: markdown_path },
        {
            highlight_style: "nord",
            css: `
                body { font-family: "Noto Sans TC", sans-serif !important; background: #ECEFF4 !important; color: #2E3440 !important; } 
                pre > code, code, code:not([class]) { color: #ECEFF4 !important; background: #2E3440 !important; font-family: "MesloLGS NF", "Cascadia Code", "Ubuntu Mono", monospace !important; }
                td, th { border: 1px solid #D8DEE9 !important; }
                thead > tr { background: #ECEFF4 !important; color: #2E3440 !important; }
                tbody > tr:nth-child(odd) { background: #ECEFF4 !important; color: #2E3440 !important; }
                tbody > tr:nth-child(even) { background: #E5E9F0 !important; color: #2E3440 !important; }
                `,
            pdf_options: {
                format: "A4",
                orientation: "portrait",
                margin: "20mm",

                headerTemplate: `<img style="position: absolute; top: 0; left: 0; width: 100%; height: 100%; z-index: -1;" src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mN88/7LfwAJbgPQ0oMMRAAAAABJRU5ErkJggg==">`,
                footerTemplate: `<span style="width: 100%; text-align: right; color: #2E3440; font-size: 10px; padding: 24px;">41047029S</span>`,
                printBackground: true,
                displayHeaderFooter: true,
            },
        }
    );
    if (pdf) return pdf.content;
    else throw new Error("PDF Conversion Failed.");
}

module.exports = { md_to_pdf };
