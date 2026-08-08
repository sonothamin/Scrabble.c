(() => {
  const toggle = document.querySelector(".nav-toggle");
  const nav = document.querySelector(".nav");
  const desktopNav = window.matchMedia("(min-width: 761px)");

  const setNavOpen = (open) => {
    if (!toggle || !nav) return;
    nav.classList.toggle("is-open", open);
    toggle.setAttribute("aria-expanded", open ? "true" : "false");
  };

  if (toggle && nav) {
    toggle.addEventListener("click", () => {
      setNavOpen(!nav.classList.contains("is-open"));
    });

    nav.querySelectorAll("a").forEach((link) => {
      link.addEventListener("click", () => setNavOpen(false));
    });

    document.addEventListener("keydown", (event) => {
      if (event.key === "Escape") setNavOpen(false);
    });

    desktopNav.addEventListener("change", (event) => {
      if (event.matches) setNavOpen(false);
    });
  }

  const reduceMotion = window.matchMedia("(prefers-reduced-motion: reduce)").matches;
  if (reduceMotion || !("IntersectionObserver" in window)) {
    document.querySelectorAll(".feature-list li, .shot").forEach((el) => el.classList.add("is-in"));
    return;
  }

  const observer = new IntersectionObserver(
    (entries) => {
      entries.forEach((entry) => {
        if (!entry.isIntersecting) return;
        const el = entry.target;
        const siblings = el.parentElement
          ? [...el.parentElement.children].filter((n) => n.matches(".feature-list li, .shot"))
          : [];
        const index = Math.max(0, siblings.indexOf(el));
        el.style.animationDelay = `${index * 0.08}s`;
        el.classList.add("is-in");
        observer.unobserve(el);
      });
    },
    { threshold: 0.12, rootMargin: "0px 0px -6% 0px" }
  );

  document.querySelectorAll(".feature-list li, .shot").forEach((el) => observer.observe(el));
})();
