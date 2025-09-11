const CACHE_NAME = 'ttt-mobile-v1';
const urlsToCache = [
    './', './ttt-mobile.html',
    './main.js',
    './main.wasm',
    './manifest.json',
    './icon.png'
];

self.addEventListener('install', e => {
    e.waitUntil(
        caches.open(CACHE_NAME).then(cache => cache.addAll(urlsToCache))
    );
});

self.addEventListener('fetch', e => {
    e.respondWith(
        caches.match(e.request).then(r => r || fetch(e.request))
    );
});

