import { defineConfig } from '@docusaurus/types';

export default defineConfig({
  title: 'Open Source Modding',
  tagline: 'Reverse engineering and modding research for game engines',
  url: 'https://open-source-modding.github.io',
  baseUrl: '/',
  projectName: 'open-source-modding.github.io',
  organizationName: 'Open-Source-Modding',
  onBrokenLinks: 'throw',
  onBrokenMarkdownLinks: 'throw',
  presets: [
    ['classic', { docs: { sidebarPath: './sidebars.ts' }, theme: { customCss: './src/css/custom.css' } }],
  ],
  plugins: [['@easyops-cn/docusaurus-search-local', { hashed: true, indexDocs: true }]],
});