import React from 'react';
import clsx from 'clsx';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';
import styles from './index.module.css';

const sections = [
  { title: 'Disrupt Engine', desc: 'Watch Dogs 1/2/Legion — XBG, HKX, materials, shaders, audio, modding workflow', to: '/docs/disrupt/watch_dogs' },
  { title: 'Dunia / Far Cry', desc: 'FC1–FC6 — XBG, .cgf lineage, shader system, FCModInstaller guides', to: '/docs/farcry/xbg-format' },
  { title: 'Havok', desc: 'HKX packfile, version compat, license keys, parsers & tools, SDK availability', to: '/docs/havok/hkx_format' },
  { title: 'Bethesda', desc: 'FO4/Skyrim/Oblivion/Starfield — archives, NIF, Havok animation, guides', to: '/docs/bethesda/bethesda-formats' },
  { title: 'Reference Hub', desc: 'The full index — every format spec, guide, and XeNTaX knowledge doc', to: '/docs/' },
  { title: 'More Games', desc: 'NFS, L4D, Sims 3, Witcher, Cyberpunk, Zelda, NMS, ETS2, MTG & more', to: '/docs/' },
];

export default function Home() {
  return (
    <Layout
      title="Home"
      description="Reverse engineering and modding research for game engines"
    >
      <header className={styles.hero}>
        <div className="container">
          <h1 className={styles.heroTitle}>Open Source Modding</h1>
          <p className={styles.heroSubtitle}>
            Reverse engineering and modding research for game engines — format specs,
            clean-room documentation, and community knowledge.
          </p>
          <div className={styles.heroActions}>
            <Link className="button button--primary button--lg" to="/docs/">
              Browse the Reference
            </Link>
          </div>
        </div>
      </header>
      <main>
        <div className="container">
          <section className={styles.cards}>
            {sections.map((s) => (
              <Link key={s.title} to={s.to} className={clsx('card', styles.card)}>
                <div className="card__body">
                  <h3>{s.title}</h3>
                  <p>{s.desc}</p>
                </div>
              </Link>
            ))}
          </section>
        </div>
      </main>
    </Layout>
  );
}