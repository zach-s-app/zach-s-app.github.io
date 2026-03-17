#!/bin/bash

DB="unilog.db"
TABLE="log"

init_db() {
  sqlite3 "$DB" <<EOF
CREATE TABLE IF NOT EXISTS $TABLE (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  timestamp TEXT DEFAULT CURRENT_TIMESTAMP,
  raw_txt TEXT,
  json TEXT,
  xml TEXT
);
EOF
}

convert_and_store() {
  local line="$1"
  [[ -z "$line" ]] && return

  local esc_val
  esc_val=$(echo "$line" | sed 's/"/\\"/g; s/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g')

  local json xml
  json="{\"msg\":\"$esc_val\"}"
  xml="<msg>$esc_val</msg>"

  sqlite3 "$DB" "INSERT INTO $TABLE (raw_txt, json, xml) VALUES ('$line', '$json', '$xml');"
}

read_log() {
  sqlite3 "$DB" "SELECT id, timestamp, raw_txt FROM $TABLE ORDER BY id;" | while IFS='|' read -r id ts txt; do
    echo "[$id] $ts: $txt"
  done
}

reset_log() {
  rm -f "$DB"
  echo "🧹 Database reset."
}

# Main
init_db

case "$1" in
  log)
    while IFS= read -r line; do
      convert_and_store "$line"
    done
    echo "✅ Logged to DB"
    ;;
  read)
    read_log
    ;;
  reset)
    reset_log
    ;;
  *)
    echo "Usage: $0 {log|read|reset}"
    ;;
esac

