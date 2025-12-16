document.addEventListener('DOMContentLoaded', async () => {
  const track = document.querySelector('.carousel__track');
  const slides = Array.from(document.querySelectorAll('.carousel__slide'));
  if (!track || slides.length === 0) return;

  const originalCount = slides.length;

  // wait for images in the original slides to load so width calculations are correct
  const imgs = slides.map(s => s.querySelector('img')).filter(Boolean);
  await Promise.all(imgs.map(img => new Promise(res => {
    if (img.complete && img.naturalWidth) return res();
    img.addEventListener('load', res);
    img.addEventListener('error', res);
  })));

  // clone originals to create seamless repetition
  slides.forEach(s => track.appendChild(s.cloneNode(true)));

  const calcLoopWidth = () => {
    const firstGroup = Array.from(track.children).slice(0, originalCount);
    return firstGroup.reduce((sum, el) => sum + el.getBoundingClientRect().width, 0);
  };

  let loopWidth = calcLoopWidth();

  // speed in pixels per second (bumped for faster movement)
  const pxPerSecond = 200;

  // create a style element for dynamic keyframes
  let styleEl = document.createElement('style');
  document.head.appendChild(styleEl);

  const applyAnimation = () => {
    loopWidth = calcLoopWidth();
    const duration = Math.max(2, loopWidth / pxPerSecond);
    const name = `scrollLoop_${Date.now()}`;
    const keyframes = `@keyframes ${name} { from { transform: translateX(0); } to { transform: translateX(-${loopWidth}px); } }`;
    styleEl.textContent = keyframes;
    track.style.animation = `${name} ${duration}s linear infinite`;
  };

  applyAnimation();

  // pause on hover
  const viewport = document.querySelector('.carousel__viewport');
  if (viewport) {
    viewport.addEventListener('mouseenter', () => track.style.animationPlayState = 'paused');
    viewport.addEventListener('mouseleave', () => track.style.animationPlayState = 'running');
  }

  // recalc on resize
  let resizeTimer = null;
  window.addEventListener('resize', () => {
    clearTimeout(resizeTimer);
    resizeTimer = setTimeout(() => applyAnimation(), 150);
  });
});