import { defineConfig } from '@docusaurus/types';

export default defineConfig({
  title: 'Open Source Modding',
  tagline: 'Reverse engineering and modding research for game engines',
  url: 'https://open-source-modding.github.io',
  baseUrl: '/',
  projectName: 'open-source-modding.github.io',
  organizationName: 'Open-Source-Modding',
  onBrokenLinks: 'throw',
  presets: [
    ['classic', { docs: { sidebarPath: './sidebars.ts' }, theme: { customCss: './src/css/custom.css' } }],
  ],
  markdown: {
    // .md = CommonMark (plain Markdown, allows indented code blocks & angle brackets),
    // .mdx = MDX (JSX). Matches the phased MDX migration.
    format: 'detect',
    hooks: { onBrokenMarkdownLinks: 'throw' },
  },
  plugins: [['@easyops-cn/docusaurus-search-local', { hashed: true, indexDocs: true }]],
});