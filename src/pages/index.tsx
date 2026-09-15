import React from 'react';
import Layout from '@theme/Layout';
import Link from '@docusaurus/Link';

export default function Home() {
  return (
    <Layout
      title="Home"
      description="Reverse engineering and modding research for game engines"
    >
      <main style={{ padding: '3rem 1rem', textAlign: 'center' }}>
        <h1>Open Source Modding</h1>
        <p>Reverse engineering and modding research for game engines.</p>
        <Link to="/docs">Browse the documentation</Link>
      </main>
    </Layout>
  );
}