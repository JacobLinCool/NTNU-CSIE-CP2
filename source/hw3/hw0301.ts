import { fs, path, ask, log, done } from "./utils";

(async () => {
    const bible: Item[] = fs
        .readFileSync(path.resolve(__dirname, "bible.txt"), "utf8")
        .split("\n")
        .filter((line) => line.trim().length > 0)
        .map((line) => JSON.parse(line.trim()));

    const target = await ask("Please enter the search target: ");
    const result = bible.filter((item) => item.text.toLowerCase().includes(target.toLowerCase()));
    log("Found", result.length, "time(s)");
    result.forEach((item, i) => console.log(`${i + 1}. ${item.book_id} ${item.chapter}:${item.verse} ${item.text}`));

    done();
})();

interface Item {
    chapter: number;
    verse: number;
    text: string;
    translation_id: string;
    book_id: string;
    book_name: string;
}

/**
 * Here is the explanation for the code above:
 * 1. We use the fs module to read the bible.txt file.
 *
 * 2. We split the file into lines and filter the empty lines.
 *
 * 3. We then map each line into an Item object.
 *
 * 4. We then filter the items based on the search target.
 *
 * 5. We then log the number of results.
 *
 * 6. We then log the results.
 *
 * 7. We then call the done function to terminate the script.
 */
