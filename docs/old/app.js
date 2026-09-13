const navLinks = [...document.querySelectorAll('.nav-link')];
const sections = [...document.querySelectorAll('.page-section')];
const sidebar = document.querySelector('#sidebar');
const menuToggle = document.querySelector('.menu-toggle');

function setActiveSection(id) {
  navLinks.forEach((link) => {
    link.classList.toggle('active', link.getAttribute('href') === `#${id}`);
  });
}

const sectionObserver = new IntersectionObserver((entries) => {
  const visible = entries
    .filter((entry) => entry.isIntersecting)
    .sort((a, b) => b.intersectionRatio - a.intersectionRatio)[0];
  if (visible) setActiveSection(visible.target.id);
}, { rootMargin: '-20% 0px -65% 0px', threshold: [0, .2, .5] });

sections.forEach((section) => sectionObserver.observe(section));

navLinks.forEach((link) => link.addEventListener('click', () => {
  sidebar.classList.remove('open');
  menuToggle?.setAttribute('aria-expanded', 'false');
}));

menuToggle?.addEventListener('click', () => {
  const open = sidebar.classList.toggle('open');
  menuToggle.setAttribute('aria-expanded', String(open));
});

document.querySelectorAll('.copy-button').forEach((button) => {
  button.addEventListener('click', async () => {
    const original = button.textContent;
    try {
      await navigator.clipboard.writeText(button.dataset.copy || '');
      button.textContent = 'kopyalandı ✓';
    } catch {
      button.textContent = 'seçip kopyala';
    }
    window.setTimeout(() => { button.textContent = original; }, 1600);
  });
});

const searchInput = document.querySelector('#searchInput');
searchInput?.addEventListener('input', (event) => {
  const query = event.target.value.trim().toLocaleLowerCase('tr-TR');
  document.querySelectorAll('.api-group').forEach((group) => {
    const matches = !query || group.textContent.toLocaleLowerCase('tr-TR').includes(query);
    group.hidden = !matches;
  });
});

document.addEventListener('keydown', (event) => {
  if (event.key === '/' && document.activeElement !== searchInput) {
    event.preventDefault();
    searchInput?.focus();
  }
});

document.querySelectorAll('.math-tab').forEach((tab) => {
  tab.addEventListener('click', () => {
    const selected = tab.dataset.tab;
    document.querySelectorAll('.math-tab').forEach((item) => item.classList.toggle('active', item === tab));
    document.querySelectorAll('.math-content').forEach((content) => {
      content.classList.toggle('hidden', content.id !== `tab-${selected}`);
    });
  });
});

function numberFrom(id) {
  const value = Number.parseFloat(document.querySelector(`#${id}`)?.value);
  return Number.isFinite(value) ? value : 0;
}

document.querySelector('#calculateMatrix')?.addEventListener('click', () => {
  const a = [[numberFrom('a00'), numberFrom('a01')], [numberFrom('a10'), numberFrom('a11')]];
  const b = [[numberFrom('b00'), numberFrom('b01')], [numberFrom('b10'), numberFrom('b11')]];
  const result = [
    [a[0][0] * b[0][0] + a[0][1] * b[1][0], a[0][0] * b[0][1] + a[0][1] * b[1][1]],
    [a[1][0] * b[0][0] + a[1][1] * b[1][0], a[1][0] * b[0][1] + a[1][1] * b[1][1]],
  ];
  const format = (value) => Number.isInteger(value) ? value : value.toFixed(2).replace(/0+$/, '').replace(/\.$/, '');
  document.querySelector('#matrixResult').innerHTML = `[ ${format(result[0][0])}, ${format(result[0][1])} ]<br>[ ${format(result[1][0])}, ${format(result[1][1])} ]`;
});

if (window.location.hash) {
  const target = document.querySelector(window.location.hash);
  if (target) window.setTimeout(() => target.scrollIntoView(), 0);
}
