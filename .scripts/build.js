const { execSync } = require("node:child_process");

execSync("pnpm i");
execSync("pnpm -s makefile", { stdio: "inherit" });
execSync("pnpm -s convert", { stdio: "inherit" });
execSync("pnpm -s readme", { stdio: "inherit" });
// execSync("pnpm -s test", { stdio: "inherit" });
