/* LearnCPP front-end: vanilla JS + CodeMirror 5 + marked. */
const $ = (s) => document.querySelector(s);
const esc = (s) => String(s ?? "").replace(/[&<>"']/g, (c) => ({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;","'":"&#39;"}[c]));
const CPP = "text/x-c++src";

const state = { catalog: null, mode: "problem", problem: null, dirty: false, saveTimer: null, running: false };
let editor;

async function api(path, body) {
  const opts = body ? { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify(body) } : undefined;
  const r = await fetch(path, opts);
  const j = await r.json().catch(() => ({ error: `HTTP ${r.status}` }));
  if (!r.ok || j.error) throw new Error(j.error || `HTTP ${r.status}`);
  return j;
}

/* ---------- markdown + code rendering ---------- */
function renderMarkdown(el, md) {
  el.innerHTML = marked.parse(md);
  el.querySelectorAll("pre code").forEach((code) => {
    const lang = (code.className.match(/language-(\w+)/) || [])[1];
    const mode = lang === "python" ? "text/x-python" : lang === "bash" || lang === "sh" || lang === "text" ? null : CPP;
    if (!mode) return;
    const src = code.textContent;
    code.textContent = "";
    code.classList.add("cm-s-nc");
    CodeMirror.runMode(src, mode, code);
  });
}
function codeView(el, src) {
  el.innerHTML = "";
  el.classList.remove("hidden");
  CodeMirror(el, { value: src, mode: CPP, theme: "nc", lineNumbers: true, readOnly: true, viewportMargin: Infinity });
}

/* ---------- problem list drawer + navbar ---------- */
const flat = () => state.catalog.chapters.flatMap((c) => c.problems);
const icon = (name) => `<svg class="i"><use href="#i-${name}"/></svg>`;
const toggleDrawer = (open) => document.body.classList.toggle("drawer-open", open);

function renderNav() {
  const nav = $("#nav");
  nav.innerHTML = "";
  const inProblem = state.mode === "problem" && state.problem;
  for (const ch of state.catalog.chapters) {
    const h = document.createElement("div");
    h.className = "chapter";
    h.title = ch.blurb.replace(/`/g, "");
    h.innerHTML = `<span>${esc(ch.title)}</span><span class="count">${ch.problems.filter((p) => p.status === "solved").length} / ${ch.problems.length}</span>`;
    nav.appendChild(h);
    for (const p of ch.problems) {
      const b = document.createElement("button");
      b.className = `sidebar-item ${p.status}` + (inProblem && state.problem.id === p.id ? " active" : "");
      b.innerHTML = icon(p.status === "solved" ? "check" : "circle") +
                    `<span class="label">${esc(p.title)}</span><span class="diff ${p.difficulty}">${p.difficulty}</span>`;
      b.onclick = () => { toggleDrawer(false); openProblem(p.id); };
      nav.appendChild(b);
    }
  }
  $("#progress-text").textContent = `${state.catalog.solved} / ${state.catalog.total}`;
  $("#progress-fill").style.width = state.catalog.total ? `${(100 * state.catalog.solved) / state.catalog.total}%` : "0";
  $("#nav-playground").classList.toggle("active", state.mode === "playground");
  const all = flat(), i = inProblem ? all.findIndex((p) => p.id === state.problem.id) : -1;
  $("#prev-btn").disabled = i <= 0;
  $("#next-btn").disabled = i >= all.length - 1;
  $("#prev-btn").onclick = () => openProblem(all[i - 1].id);
  $("#next-btn").onclick = () => openProblem(all[i + 1].id);
  if (state.mode === "playground" || state.problem) renderHeader();
}
function renderHeader() {
  $("#q-check").classList.add("hidden");
  if (state.mode === "playground") { $("#q-title").textContent = "Playground"; $("#q-pills").innerHTML = ""; return; }
  const p = state.problem;
  const ch = state.catalog.chapters.find((c) => c.problems.some((q) => q.id === p.id));
  const solved = ch.problems.find((q) => q.id === p.id).status === "solved";
  $("#q-title").textContent = p.meta.title;
  $("#q-check").classList.toggle("hidden", !solved);
  $("#q-pills").innerHTML = `<span class="pill ${esc(p.meta.difficulty)}">${esc(p.meta.difficulty)}</span><span class="pill tag">${esc(ch.title)}</span>` +
    ((p.meta.hints || []).length ? `<button class="pill tag" onclick="toHints()">${icon("help")}Hints</button>` : "");
}
function setStatus(id, status) {
  for (const ch of state.catalog.chapters) for (const p of ch.problems) if (p.id === id) p.status = status;
  state.catalog.solved = flat().filter((p) => p.status === "solved").length;
  renderNav();
}

/* ---------- tabs ---------- */
function setTab(name) {
  document.querySelectorAll("#tabs button").forEach((b) => b.classList.toggle("active", b.dataset.tab === name));
  document.querySelectorAll(".tab-pane").forEach((p) => p.classList.toggle("hidden", p.id !== `tab-${name}`));
  // CodeMirror instances created while hidden render nothing until refreshed.
  document.querySelectorAll(`#tab-${name} .CodeMirror`).forEach((el) => el.CodeMirror && el.CodeMirror.refresh());
}
$("#tabs").onclick = (e) => { if (e.target.dataset.tab) setTab(e.target.dataset.tab); };

/* ---------- problem view ---------- */
async function openProblem(id) {
  await flushSave();
  const p = await api(`/api/problem/${id}`);
  state.mode = "problem";
  state.problem = p;
  state.dirty = false;
  $("#save-state").textContent = "";
  runLabel();
  $("#tabs").classList.remove("hidden");
  renderMarkdown($("#q-body"), p.problem_md.replace(/^# .*\n+/, ""));  // the title is rendered by renderHeader
  codeView($("#tests-view"), p.test_src);
  renderHints();
  $("#solution-view").classList.add("hidden");
  $("#solution-btn").textContent = "Show solution";
  $("#solution-btn").dataset.armed = "";
  $("#solution-btn").classList.remove("hidden");
  editor.setValue(p.code);
  editor.clearHistory();
  editor.focus();
  setTab("problem");
  $("#tab-problem").scrollTop = 0;
  $("#results").innerHTML = `<div class="muted pad">${p.status === "solved" ? "Solved. Submit again any time, or pick the next problem." : "Run your code to see results here."}</div>`;
  renderNav();
}

function toHints() { setTab("problem"); $("#hints-list").scrollIntoView({ behavior: "smooth" }); }
function renderHints() {
  const p = state.problem, list = $("#hints-list");
  list.innerHTML = "";
  (p.meta.hints || []).forEach((h, i) => {
    const d = document.createElement("details");
    d.className = "accordion";
    d.innerHTML = `<summary>Hint ${i + 1}</summary>`;
    const body = document.createElement("div");
    body.className = "accordion-body markdown";
    renderMarkdown(body, h);
    d.appendChild(body);
    d.ontoggle = () => {
      if (!d.open || i < p.hints_revealed) return;
      p.hints_revealed = i + 1;
      api("/api/save", { id: p.id, hints_revealed: p.hints_revealed });
    };
    list.appendChild(d);
  });
}
$("#solution-btn").onclick = async (e) => {
  const b = e.currentTarget;
  if (!b.dataset.armed) { b.dataset.armed = "1"; b.textContent = "Really show it? Click again."; return; }
  const { solution } = await api(`/api/solution/${state.problem.id}`);
  codeView($("#solution-view"), solution);
  b.classList.add("hidden");
};

/* ---------- saving ---------- */
function scheduleSave() {
  state.dirty = true;
  $("#save-state").textContent = "unsaved";
  clearTimeout(state.saveTimer);
  state.saveTimer = setTimeout(flushSave, 800);
}
async function flushSave() {
  clearTimeout(state.saveTimer);
  if (!state.dirty) return;
  state.dirty = false;
  const code = editor.getValue();
  try {
    if (state.mode === "playground") await api("/api/playground", { code });
    else {
      const r = await api("/api/save", { id: state.problem.id, code });
      const cur = flat().find((p) => p.id === state.problem.id);
      if (cur && cur.status !== r.status) setStatus(state.problem.id, r.status);
    }
    $("#save-state").textContent = "saved";
  } catch (err) { $("#save-state").textContent = "save failed: " + err.message; state.dirty = true; }
}
window.addEventListener("beforeunload", () => {
  if (!state.dirty) return;
  const code = editor.getValue();
  const body = state.mode === "playground" ? { code } : { id: state.problem.id, code };
  navigator.sendBeacon(state.mode === "playground" ? "/api/playground" : "/api/save", new Blob([JSON.stringify(body)], { type: "application/json" }));
});

/* ---------- running ---------- */
// Run tests the code and shows the result; Submit also records the attempt (and can mark the problem solved).
async function run(submit = true) {
  if (state.running) return;
  state.running = true;
  const btn = $(state.mode === "playground" || !submit ? "#run" : "#submit"), label = btn.textContent;
  $("#run").disabled = $("#submit").disabled = true;
  btn.textContent = "Running…";
  toggleConsole(true);
  const key = chatKey();
  $("#results").innerHTML = `<div class="muted pad">Compiling with clang++ -fsanitize=address,undefined …</div>`;
  try {
    const code = editor.getValue();
    if (state.mode === "playground") {
      state.dirty = false;
      renderPlaygroundResults(lastResults[key] = await api("/api/playground", { code, run: true }));
    } else {
      state.dirty = false;
      const r = await api("/api/run", { id: state.problem.id, code, submit });
      renderResults(lastResults[key] = r, submit);
      setStatus(state.problem.id, r.status);
    }
    $("#save-state").textContent = "saved";
  } catch (err) {
    $("#results").innerHTML = `<div class="block err"><h4>Error</h4><pre>${esc(err.message)}</pre></div>`;
  } finally {
    state.running = false;
    $("#run").disabled = $("#submit").disabled = false;
    btn.textContent = label;
  }
}

function runLabel() {
  const pg = state.mode === "playground";
  $("#submit").classList.toggle("hidden", pg);
  $("#file-chip").textContent = pg ? "main.cpp" : "solution.cpp";
  const noHints = pg || !(state.problem.meta.hints || []).length;
  $("#hint-btn").classList.toggle("hidden", noHints);
  $("#hint-btn").nextElementSibling.classList.toggle("hidden", noHints);
  renderChat();
}
function toggleConsole(open) {
  const c = $("#console"), show = open ?? c.classList.contains("hidden");
  c.classList.toggle("hidden", !show);
  $("#console-toggle").classList.toggle("closed", !show);
  editor.refresh();
}
const verdict = (cls, text, sub = "") => `<div class="banner ${cls}"><span class="verdict">${text}</span><span class="sub">${sub}</span>`;

function renderResults(r, submit) {
  const out = [];
  if (!r.compile_ok) {
    out.push(verdict("bad", "Compile Error") + `</div>`);
    out.push(`<div class="block err"><h4>clang++ says</h4><pre>${esc(r.compile_output)}</pre></div>`);
    out.push(`<div class="pad muted">Line numbers like <code>solution.cpp:12:5</code> refer to your editor. Read the first error first.</div>`);
  } else {
    if (r.solved && !submit) {
      out.push(verdict("ok", "Passed", `All ${r.total} test cases pass. Submit to mark it solved.`) + `</div>`);
    } else if (r.solved) {
      out.push(verdict("ok", "Accepted", `Passed test cases: ${r.total} / ${r.total}`) +
        (r.next_id ? `<button class="btn run" onclick="openProblem('${r.next_id}')">Next problem ${icon("right")}</button>` : `<span class="muted">That was the last one.</span>`) + `</div>`);
    } else if (r.timed_out) {
      out.push(verdict("bad", "Time Limit Exceeded", "Stopped after 5 s. Infinite loop?") + `</div>`);
    } else if (r.crashed) {
      out.push(verdict("bad", "Runtime Error", r.cases.length ? `Crashed after ${r.passed} / ${r.cases.length} test cases` : "Crashed before the first test case finished") + `</div>`);
    } else {
      out.push(verdict("bad", "Wrong Answer", `Passed test cases: ${r.passed} / ${r.total}`) + `</div>`);
    }
    if (r.runtime_error) {
      out.push(`<div class="block err"><h4>${r.crashed ? "Runtime error (sanitizer report)" : "stderr"}</h4><pre>${esc(r.runtime_error)}</pre></div>`);
    }
    for (const c of r.cases) {
      out.push(`<div class="case ${c.pass ? "pass" : "fail"}"><span class="mark">${c.pass ? "✓" : "✗"}</span><span>${esc(c.name)}</span><span class="detail">${esc(c.detail)}</span></div>`);
    }
    if (r.program_output) out.push(`<div class="block"><h4>Your output (std::cout)</h4><pre>${esc(r.program_output)}</pre></div>`);
    if (r.compile_output) out.push(`<div class="block warnpre"><details><summary>Compiler warnings</summary><pre>${esc(r.compile_output)}</pre></details></div>`);
  }
  $("#results").innerHTML = out.join("");
}

/* ---------- tutor ---------- */
// Chats and last results are kept per problem ("playground" for the playground) until the page reloads.
const chats = {}, lastResults = {};
const chatKey = () => (state.mode === "playground" ? "playground" : state.problem.id);
function toggleTutor(open) {
  const show = open ?? $("#tutor").classList.contains("hidden");
  $("#tutor").classList.toggle("hidden", !show);
  $("#tdragger").classList.toggle("hidden", !show);
  $("#tutor-btn").classList.toggle("active", show);
  editor.refresh();
  if (show) $("#ask-input").focus();
}
function renderChat() {
  const box = $("#chat"), msgs = chats[chatKey()] || [];
  box.innerHTML = msgs.length ? "" : `<div class="muted">The tutor sees this problem, your code and your last result. It gives hints rather than the answer, unless you ask for the answer.</div>`;
  for (const m of msgs) {
    const d = document.createElement("div");
    d.className = `msg ${m.role}`;
    if (m.role === "tutor") { d.classList.add("markdown"); renderMarkdown(d, m.text || "…"); }
    else d.textContent = m.text;
    box.appendChild(d);
  }
  box.scrollTop = box.scrollHeight;
}
async function ask(question) {
  const key = chatKey(), msgs = (chats[key] ||= []), history = msgs.slice(-20), reply = { role: "tutor", text: "" };
  msgs.push({ role: "student", text: question }, reply);
  renderChat();
  $("#ask-send").disabled = true;
  try {
    const r = await fetch("/api/ask", { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify({
      id: state.mode === "playground" ? null : state.problem.id, code: editor.getValue(), result: lastResults[key], history, question }) });
    if (!r.ok) throw new Error((await r.json().catch(() => ({}))).error || `HTTP ${r.status}`);
    const reader = r.body.getReader(), dec = new TextDecoder();
    for (;;) {
      const { done, value } = await reader.read();
      if (done) break;
      reply.text += dec.decode(value, { stream: true });
      if (chatKey() === key) renderChat();
    }
  } catch (err) {
    reply.text += `\n\nTutor error: ${err.message}`;
  } finally {
    $("#ask-send").disabled = false;
    if (chatKey() === key) renderChat();
  }
}

/* ---------- playground ---------- */
const PLAYGROUND_MD = `Free-form scratch space. Write any program with a \`main()\` and run it. Same compiler flags as the problems: \`-std=c++17 -Wall -Wextra -fsanitize=address,undefined\`, so memory bugs and undefined behaviour get reported instead of silently corrupting things.

Good things to poke at:

- \`std::cout << 7 / 2 << " " << 7.0 / 2 << "\\n";\`
- \`sizeof(int)\`, \`sizeof(long)\`, \`sizeof(char*)\`
- what happens with \`int x = 2147483647; x += 1;\`
- \`int* p = nullptr; std::cout << *p;\`

Your code here is saved automatically, like a problem.`;

async function openPlayground() {
  await flushSave();
  const { code } = await api("/api/playground");
  state.mode = "playground";
  state.dirty = false;
  $("#save-state").textContent = "";
  $("#tabs").classList.add("hidden");
  renderMarkdown($("#q-body"), PLAYGROUND_MD);
  $("#hints-list").innerHTML = "";
  setTab("problem");
  editor.setValue(code);
  editor.clearHistory();
  editor.focus();
  runLabel();
  $("#results").innerHTML = `<div class="muted pad">Output appears here.</div>`;
  renderNav();
}
function renderPlaygroundResults(r) {
  const out = [];
  if (!r.compile_ok) {
    out.push(verdict("bad", "Compile Error") + `</div><div class="block err"><h4>clang++ says</h4><pre>${esc(r.compile_output)}</pre></div>`);
  } else {
    if (r.timed_out) out.push(verdict("bad", "Time Limit Exceeded", "Stopped after 5 s") + `</div>`);
    else if (r.exit_code === 0) out.push(verdict("ok", "Finished", "Exited with code 0") + `</div>`);
    else if (r.exit_code > 0) out.push(verdict("warn", "Finished", "Exited with code " + r.exit_code) + `</div>`);
    else out.push(verdict("bad", "Runtime Error", "Crashed (signal " + -r.exit_code + ")") + `</div>`);
    out.push(`<div class="block"><h4>stdout</h4><pre>${esc(r.stdout) || '<span class="muted">(empty)</span>'}</pre></div>`);
    if (r.stderr) out.push(`<div class="block err"><h4>stderr</h4><pre>${esc(r.stderr)}</pre></div>`);
    if (r.compile_output) out.push(`<div class="block warnpre"><details><summary>Compiler warnings</summary><pre>${esc(r.compile_output)}</pre></details></div>`);
  }
  $("#results").innerHTML = out.join("");
}

/* ---------- init ---------- */
function init() {
  editor = CodeMirror.fromTextArea($("#editor"), {
    mode: CPP, theme: "nc", lineNumbers: true, indentUnit: 4, tabSize: 4, indentWithTabs: false,
    matchBrackets: true, autoCloseBrackets: true, lineWrapping: false,
    extraKeys: {
      "Cmd-Enter": () => run(), "Ctrl-Enter": () => run(),
      Tab: (cm) => cm.somethingSelected() ? cm.indentSelection("add") : cm.replaceSelection("    ", "end"),
      "Shift-Tab": (cm) => cm.indentSelection("subtract"),
    },
  });
  editor.on("change", scheduleSave);
  let activeLine = null;
  editor.on("cursorActivity", () => {
    const c = editor.getCursor();
    $("#ln-col").textContent = `Ln ${c.line + 1}, Col ${c.ch + 1}`;
    if (activeLine) editor.removeLineClass(activeLine, "gutter", "active-line");
    activeLine = editor.addLineClass(c.line, "gutter", "active-line");
  });
  $("#run").onclick = () => run(false);
  $("#submit").onclick = () => run(true);
  $("#hint-btn").onclick = toHints;
  $("#tutor-btn").onclick = () => toggleTutor();
  $("#tutor-close").onclick = () => toggleTutor(false);
  drag($("#tdragger"), (e) => {
    const right = $("#main").getBoundingClientRect().right - 19;  // 19 = #main's right padding
    $("#tutor").style.flexBasis = Math.min(Math.max(right - e.clientX, 260), innerWidth * 0.5) + "px";
  });
  $("#ask-form").onsubmit = (e) => {
    e.preventDefault();
    const q = $("#ask-input").value.trim();
    if (!q || $("#ask-send").disabled) return;
    $("#ask-input").value = "";
    ask(q);
  };
  $("#ask-input").onkeydown = (e) => { if (e.key === "Enter" && !e.shiftKey) { e.preventDefault(); $("#ask-form").requestSubmit(); } };
  $("#reset").onclick = () => {
    if (state.mode === "playground") { editor.setValue(PLAYGROUND_RESET); return; }
    if (confirm("Replace your code with the original starter?")) editor.setValue(state.problem.starter);
  };
  $("#nav-playground").onclick = openPlayground;
  $("#list-btn").onclick = () => toggleDrawer(true);
  $("#list-close").onclick = $("#backdrop").onclick = () => toggleDrawer(false);
  document.addEventListener("keydown", (e) => { if (e.key === "Escape") toggleDrawer(false); });
  $("#console-toggle").onclick = () => toggleConsole();
  drag($("#dragger"), (e) => { $("#left").style.flexBasis = Math.min(Math.max(e.clientX - 10, 320), innerWidth - 360) + "px"; });
  drag($("#vdragger"), (e) => {
    if ($("#console").classList.contains("hidden")) return;
    const r = $("#right").getBoundingClientRect(), bar = $(".submit-toolbar").offsetHeight;
    $("#console").style.height = Math.min(Math.max(r.bottom - bar - e.clientY, 80), r.height - 200) + "px";
  });
  api("/api/problems").then((cat) => {
    state.catalog = cat;
    renderNav();
    const first = flat()[0];
    const start = cat.last_problem && cat.chapters.some((c) => c.problems.some((p) => p.id === cat.last_problem)) ? cat.last_problem : first && first.id;
    if (start) openProblem(start);
  }).catch((err) => { $("#q-body").innerHTML = `<pre>${esc(err.message)}</pre>`; });
}
// Pane resizing: while the handle is held, every pointer move goes to onMove.
function drag(handle, onMove) {
  handle.onpointerdown = (e) => {
    handle.setPointerCapture(e.pointerId);
    handle.onpointermove = onMove;
    handle.onpointerup = () => { handle.onpointermove = null; editor.refresh(); };
  };
}
const PLAYGROUND_RESET = '#include <iostream>\n#include <string>\n#include <vector>\n\nint main() {\n    std::cout << "Hello, C++!\\n";\n    return 0;\n}\n';
init();
