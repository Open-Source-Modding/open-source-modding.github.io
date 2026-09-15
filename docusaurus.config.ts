import type {Config} from '@docusaurus/types';

const config: Config = {
  title: 'Open Source Modding',
  tagline: 'Reverse engineering and modding research for game engines',
  url: 'https://open-source-modding.github.io',
  baseUrl: '/',
  projectName: 'open-source-modding.github.io',
  organizationName: 'Open-Source-Modding',
  onBrokenLinks: 'throw',
  themeConfig: {
    navbar: {
      title: 'Open Source Modding',
      items: [
        { to: '/docs/', label: 'Reference', position: 'left' },
        {
          href: 'https://github.com/Open-Source-Modding/open-source-modding.github.io',
          label: 'GitHub',
          position: 'right',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Docs',
          items: [{ label: 'Reference', to: '/docs/' }],
        },
        {
          title: 'Community',
          items: [
            {
              label: 'GitHub',
              href: 'https://github.com/Open-Source-Modding/open-source-modding.github.io',
            },
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} Open Source Modding`,
    },
  },
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
};

export default config;