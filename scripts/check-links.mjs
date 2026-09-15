import { readdirSync, readFileSync, statSync } from 'fs';
import { join, resolve } from 'path';
import { globSync } from 'glob';

const root = resolve('docs');
const files = globSync('**/*.md*', { cwd: root });
let failures = 0;
const exists = (p) => { try { return statSync(join(root, p)).isFile(); } catch { return false; } };
// Match markdown links, handling balanced parens inside the URL (e.g. filenames with (FC4)).
const linkRe = /\[[^\]]*\]\(((?:[^()]|\([^()]*\))+)\)/g;
for (const f of files) {
  const content = readFileSync(join(root, f), 'utf8');
  for (const m of content.matchAll(linkRe)) {
    const target = m[1].split('#')[0].split('?')[0];
    if (!target || target.startsWith('http')) continue;
    if (!exists(target) && !exists(join(f.split('/').slice(0,-1).join('/'), target))) {
      console.error(`BROKEN: ${f} -> ${target}`);
      failures++;
    }
  }
}
if (failures) process.exit(1);
console.log(`All links OK across ${files.length} files`);