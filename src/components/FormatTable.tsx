import React from 'react';

export type FormatColumn = {
  header: string;
  /** Optionally render bytes as hex. */
  hex?: boolean;
};

export type FormatRow = Record<string, string | number>;

export default function FormatTable({
  columns,
  rows,
}: {
  columns: FormatColumn[];
  rows: FormatRow[];
}) {
  return (
    <table>
      <thead>
        <tr>
          {columns.map((c) => (
            <th key={c.header}>{c.header}</th>
          ))}
        </tr>
      </thead>
      <tbody>
        {rows.map((row, i) => (
          <tr key={i}>
            {columns.map((c) => {
              const v = row[c.header];
              const display =
                c.hex && typeof v === 'number' ? `0x${v.toString(16).toUpperCase()}` : String(v);
              return <td key={c.header}>{display}</td>;
            })}
          </tr>
        ))}
      </tbody>
    </table>
  );
}