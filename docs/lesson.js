(() => {
  const currentId = document.body.dataset.doc;
  const docs = window.MATRIXFLASH_DOCS || [];
  const docIndex = docs.findIndex((item) => item.id === currentId);
  const doc = docs[docIndex] || docs[0];
  const groups = [...new Set(docs.map((item) => item.group))];
  const esc = (value) => String(value).replace(/[&<>"']/g, (char) => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#039;' }[char]));
  const accentTitle = (value) => {
    const parts = value.split(' ');
    const last = parts.pop();
    return `${esc(parts.join(' '))} <span>${esc(last)}</span>`;
  };
  const fileName = `lesson-${doc.id}.html`;
  const previous = docs[docIndex - 1];
  const next = docs[docIndex + 1];
  const nav = groups.map((group) => {
    const items = docs.filter((item) => item.group === group);
    return `<section class="lesson-nav-group"><div class="lesson-nav-group-title"><span>${esc(group)}</span><b>${String(items[0].id).replace(/^0/, '')}–${items[items.length - 1].id}</b></div>${items.map((item) => `<a class="lesson-nav-link ${item.id === doc.id ? 'current' : ''}" href="lesson-${item.id}.html"><span>${item.id}</span>${esc(item.title)}</a>`).join('')}</section>`;
  }).join('');
  const pageLink = (item) => item ? `lesson-${item.id}.html` : 'index.html#home';
  const pageText = (item, fallback) => item ? item.title : fallback;

  document.title = `${doc.title} | MatrixFlash Docs`;
  document.body.className = 'lesson-body';
  document.body.innerHTML = `
    <header class="lesson-topbar">
      <a class="brand" href="index.html#home" aria-label="MatrixFlash ana sayfa"><span class="brand-mark" aria-hidden="true"><i></i><i></i><i></i><i></i></span><span>Matrix<span>Flash</span></span></a>
      <a class="back-home" href="index.html#home">← Ana dokümantasyon</a>
      <span class="lesson-breadcrumb">${esc(doc.group)} / ${doc.id} / ${esc(doc.title)}</span>
      <button class="lesson-menu" type="button" aria-label="Ders menüsünü aç" aria-expanded="false">☰</button>
    </header>
    <div class="lesson-layout">
      <aside class="lesson-sidebar" id="lessonSidebar" aria-label="40 derslik müfredat">${nav}</aside>
      <main class="lesson-content">
        <section class="lesson-hero">
          <div class="lesson-hero-inner">
            <div class="lesson-eyebrow">${esc(doc.group)} · Ders ${doc.id} / 40</div>
            <h1>${accentTitle(doc.title)}</h1>
            <p class="lesson-summary">${esc(doc.summary)}</p>
            <div class="lesson-meta"><span>${esc(doc.slug)}</span><span>C++17</span><span>MatrixFlash API</span></div>
          </div>
        </section>
        <section class="lesson-article">
          <div class="lesson-grid">
            <div class="lesson-main">
              <article class="lesson-block"><h2>Neden önemli?</h2><p>${esc(doc.why)}</p><div class="lesson-check"><article><b>01 / Kavram</b><span>${esc(doc.summary)}</span></article><article><b>02 / Bağlantı</b><span>Bu fikir, MatrixFlash içindeki ${esc(doc.group.toLocaleLowerCase('tr-TR'))} araçlarıyla doğrudan uygulanır.</span></article><article><b>03 / Hedef</b><span>Boyutu, veri akışını ve fonksiyonun yan etkisini açıklayabilmek.</span></article></div></article>
              <article class="lesson-block"><h2>Matematiksel model</h2><p>${esc(doc.math)}</p><div class="lesson-formula">${esc(doc.math)}</div></article>
              <article class="lesson-block"><h2>Koddaki karşılığı</h2><p>İşlemi küçük bir örnekle çalıştır. Kodun hangi satırının hangi matematiksel adımı temsil ettiğini izlemeye çalış.</p><div class="lesson-code-wrap"><span class="code-label">C++17</span><pre><code>${esc(doc.code)}</code></pre><button class="lesson-copy" type="button">kodu kopyala</button></div></article>
              <article class="lesson-block"><h2>Kontrol noktası</h2><div class="lesson-note"><div><strong>Çalışırken düşün:</strong> ${esc(doc.note)}</div></div></article>
            </div>
            <aside class="lesson-side-card"><h3>Bu derste</h3><ul><li>Konseptin gerçek hayattaki karşılığı</li><li>Formül ve boyut ilişkisi</li><li>MatrixFlash ile C++ örneği</li><li>Sonraki derse hazırlık</li></ul><a class="lesson-source" href="../include/Matrix/Matrix.hpp">↗ Public API'yi incele</a><a class="lesson-source" href="../README.md">↗ Proje README'si</a></aside>
          </div>
        </section>
        <nav class="lesson-pagination" aria-label="Ders navigasyonu">
          <a href="${pageLink(previous)}"><small>← ÖNCEKİ DERS</small><strong>${esc(pageText(previous, 'Ana sayfa'))}</strong></a>
          <a href="${pageLink(next)}"><small class="next-label">SONRAKİ DERS →</small><strong>${esc(pageText(next, 'Müfredatı tamamladın'))}</strong></a>
        </nav>
        <footer class="lesson-footer"><span>MatrixFlash Docs · Ders ${doc.id} / 40</span><span>C++17 ile öğrenmek için tasarlandı <b>□</b></span></footer>
      </main>
    </div>`;

  const sidebar = document.querySelector('#lessonSidebar');
  const menu = document.querySelector('.lesson-menu');
  menu?.addEventListener('click', () => {
    const open = sidebar.classList.toggle('open');
    menu.setAttribute('aria-expanded', String(open));
  });
  document.querySelectorAll('.lesson-nav-link').forEach((link) => link.addEventListener('click', () => {
    sidebar.classList.remove('open');
    menu?.setAttribute('aria-expanded', 'false');
  }));
  document.querySelector('.lesson-copy')?.addEventListener('click', async (event) => {
    const button = event.currentTarget;
    const code = document.querySelector('.lesson-code-wrap code').textContent;
    const original = button.textContent;
    try {
      await navigator.clipboard.writeText(code);
      button.textContent = 'kopyalandı ✓';
    } catch {
      button.textContent = 'seçip kopyala';
    }
    window.setTimeout(() => { button.textContent = original; }, 1600);
  });
})();
