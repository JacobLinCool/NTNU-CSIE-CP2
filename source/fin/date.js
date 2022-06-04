const S = new Date("2020/1/22");
const E = new Date("2022/6/3");

const list = [];

for (let i = S.getTime(); i <= E.getTime(); i += 1000 * 60 * 60 * 24) {
    d = new Date(i);
    list.push(
        `"${(d.getMonth() + 1).toString().padStart(2, "0")}-${d
            .getDate()
            .toString()
            .padStart(2, "0")}-${d.getFullYear()}"`
    );
}

console.log("char* dates[] = {" + list.join(",\n") + "};\n");
