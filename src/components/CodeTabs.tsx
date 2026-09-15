import React from 'react';
import Tabs from '@theme/Tabs';
import TabItem from '@theme/TabItem';

export default function CodeTabs({
  items,
}: {
  items: { label: string; language?: string; code: string }[];
}) {
  return (
    <Tabs>
      {items.map((item) => (
        <TabItem key={item.label} value={item.label} label={item.label}>
          <pre>
            <code className={item.language ? `language-${item.language}` : undefined}>
              {item.code}
            </code>
          </pre>
        </TabItem>
      ))}
    </Tabs>
  );
}