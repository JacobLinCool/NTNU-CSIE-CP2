import fs from "node:fs";
import path from "node:path";
import { createInterface } from "node:readline";

const tui = createInterface({ input: process.stdin, output: process.stdout });
const ask = (q: string) => new Promise<string>((resolve) => tui.question(q, resolve));
const log = (...args: unknown[]) => console.log(...args);
const done = () => tui.close();

export { fs, path, ask, log, done };
