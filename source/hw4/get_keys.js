(() => {
    const titles = [...document.querySelectorAll("h3[dir=auto]")];
    const tables = [...document.querySelectorAll("table")];

    const result = `
typedef enum Language {
${titles.map((title) => "    " + title.textContent.split(" ")[0].trim().toLowerCase() + ",").join("\n")}
} Language;

char* keywords[][] = {
${tables
    .map((table, i) => {
        const keywords = [...table.querySelectorAll("td")].map((td) => td.textContent.trim()).filter((x) => x);
        const code = `/* ${titles[i].textContent.trim()} */ { ${keywords
            .map((keyword) => `"${keyword.replace(/\\/g, "\\\\")}"`)
            .join(", ")} }`;
        return `    ${code},`;
    })
    .join("\n")}
}

int keyword_sizes[] = {
${tables
    .map((table) => [...table.querySelectorAll("td")].map((td) => td.textContent.trim()).filter((x) => x).length)
    .join(", ")}
};

int is_keyword(Language lang, char* keyword) {
    for (int i = 0; i < keyword_sizes[lang]; i++) {
        if (strcmp(keywords[lang][i], keyword) == 0) {
            return i;
        }
    }
    return -1;
}
`;
    return result;
})();
